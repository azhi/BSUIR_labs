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
    return 0
  end

  def self.jacobi a, p
    return 0 if a == 0
    return 1 if a == 1

    b = a
    e = 0
    while b & 1 == 0 do
      b >>= 1
      e += 1
    end

    m = p % 8
    if e & 1 == 0
      s = 1
    elsif m == 1 || m == 7
      s = 1
    elsif m == 3 || m == 5
      s = -1
    end

    s = -s if p % 4 == 3 && b % 4 == 3

    n1 = b == 1 ? 1 : p % b
    
    return s * jacobi(n1, b)
  end

  def self.square_root_mod a, p
    return Float::NAN if jacobi(a,p) == -1

    begin ; b = rand(1...p) ; end while jacobi(b,p) != -1

    t = p - 1
    s = 0
    while ( t & 1 == 0 ) do
      t >>= 1
      s += 1
    end

    ai = extended_euclid a, p
    c = b ** t % p
    r = a ** ((t + 1) / 2) % p
    (1...s).each do |i|
      e = 2 ** (s - i - 1) % p
      d = (r * r  * ai % p) ** e % p
      r = r * c % p if d == p - 1
      c = c * c % p
    end
    return r, -r % p
  end

  def self.prime? n
    res = true
    (2..(n ** 0.5).floor).each{ |el| break unless res; res &= n % el != 0 }
    res
  end
end
