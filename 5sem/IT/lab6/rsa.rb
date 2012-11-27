module Math
  def self.extended_euclid num, modul
    r1 = modul; r2 = num;
    t1 = 0; t2 = 1;
    while ( r2 > 0 ) do
      q = r1/r2
      r = r1 - q * r2
      r1 = r2
      r2 = r

      t = t1 - q * t2
      t1 = t2
      t2 = t
    end

    t1 += modul if t1 < 0
    return t1 if r1 == 1
    return Float::NAN
  end

  def self.get_primes n
    primes = [2]
    (3..n).each do |x|
      primes << x if primes.all?{ |el| x % el != 0 }
    end
    primes
  end

  def self.calc_phi n
    primes = get_primes n
    mult = primes.find_all{ |pr| n % pr == 0 }
    raise "Bad ko" if mult.size != 2
    (mult[0] - 1) * (mult[1] - 1)
  end
end

class RSA
  attr_reader :r, :ko, :ks

  def initialize r, ko, ks
    @r = r
    @ko = ko
    @ks = ks
  end

  def encode str, k = @ko
    raise "Uninitalized RSA usage" unless k
    res = ""
    str.each_byte do |b|
      enc_bytes = b ** k % @r
      enc_hi = (enc_bytes >> 8) & 0xFF
      enc_lo = enc_bytes & 0xFF
      res += enc_hi.chr + enc_lo.chr
    end
    res
  end

  def decode str, k = @ks
    raise "Uninitalized RSA usage" unless k
    res = ""
    str.each_byte.to_a.each_slice(2).map{ |(hi, lo)| (hi << 8) | lo }.each do |_2bytes|
      sym = _2bytes ** k % @r
      raise "Got symbol out of 256 when decoding. Possibly bad ks or r" if sym > 256
      res += sym.chr
    end
    res
  end

  class << self
    def create_encoding p, q, ks
      r = p * q
      raise "p * q if out of range (65536)" if r > 65535

      phi = (p - 1) * (q - 1)
      if ks.nil?
        ks = rand((phi/2 - phi/10)..(phi/2 + phi/10))
        while ( ks.gcd(phi) != 1 ) do
          ks = rand((phi/2 - phi/10)..(phi/2 + phi/10))
        end
      end

      ko = Math::extended_euclid ks, phi
      raise "Bad p and q" if ko.is_a?(Float) && !ko.finite?
      raise "Bad p, q or ks" if ko.gcd(phi) != 1

      new r, ko, ks
    end

    def create_decoding r, ko, ks
      new r, ko, ks
    end

    def create_hacking r, ko
      phi = Math::calc_phi r
      raise "Bad ko" if ko.gcd(phi) != 1
      ks = Math::extended_euclid ko, phi

      raise "Bad ko or r" if ks.is_a?(Float) && !ks.finite?
      STDERR.puts "Secret key: #{ks}"

      new r, ko, ks
    end
  end
end

