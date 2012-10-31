#! /usr/bin/env ruby

def extended_euclid num, modul
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

class SessionKeyGenerator
  class << self
    def generate_session_keys k, type
      @key = []
      7.times do
        s = k.to_s(2)
        s = '0' * (128 - s.size) + s if (128 - s.size) > 0
        @key << s.scan(/.{16}/).map{ |el| el.to_i(2)}
        k = (s[25..-1] + s[0...25]).to_i(2)
      end
      @key = @key.flatten
      if type == :decode
        @u = @key.dup
        @key.clear
        @key << process_6_indexes([48, 49, 50, 51, 46, 47])
        i = 42
        while ( i >= 6 ) do
          @key << process_6_indexes([i, i+2, i+1, i+3, i-2, i-1])
          i -= 6
        end
        @key << process_6_indexes([0, 1, 2, 3, 4, 5])
      end
      @key = @key.flatten
      @key
    end

    private

    def process_6_indexes arg
      res1 = extended_euclid @u[ arg[0] ], 65537
      res2 = 65536 - @u[ arg[1] ]
      res3 = 65536 - @u[ arg[2] ]
      res4 = extended_euclid @u[ arg[3] ], 65537
      res5 = @u[ arg[4] ]
      res6 = @u[ arg[5] ]
      res = [res1, res2, res3, res4, res5, res6]
      unless res.find{ |el| el == Float::NAN }.nil?
        throw Float::NAN
      else
        return res
      end
    end
    
  end
end

class IDEA
  def initialize k, type
    @k = k
    @z = SessionKeyGenerator.generate_session_keys @k, type
  end

  def set_type type
    @z = SessionKeyGenerator.generate_session_keys @k, type
  end

  def do_lap hash
    i1 = multiply hash[:X][0], hash[:Z][0]
    i2 = add hash[:X][1], hash[:Z][1]
    i3 = add hash[:X][2], hash[:Z][2]
    i4 = multiply hash[:X][3], hash[:Z][3]
    inMAl = xor i1, i3
    inMAr = xor i2, i4

    mal = multiply inMAl, hash[:Z][4]
    mar = add inMAr, mal
    outMAr = multiply mar, hash[:Z][5]
    outMAl = add mal, outMAr

    w = []
    w[0] = xor i1, outMAr
    w[2] = xor i2, outMAl
    w[1] = xor i3, outMAr
    w[3] = xor i4, outMAl
    return w
  end

  def do_last_lap hash
    w = []
    w[0] = multiply hash[:X][0], hash[:Z][0]
    w[1] = add hash[:X][1], hash[:Z][1]
    w[2] = add hash[:X][2], hash[:Z][2]
    w[3] = multiply hash[:X][3], hash[:Z][3]
    return w
  end

  def do_64_bits input
    input = '0' * (64 - input.size) + input if (64 - input.size) > 0
    x = input.scan(/.{16}/).map{ |el| el.to_i(2) }

    i = 0
    8.times do
      x = do_lap({ :X => x, :Z => @z[i..i+5]})
      i += 6
    end

    x[1], x[2] = x[2], x[1]
    x = do_last_lap({:X => x, :Z => @z[48..51]})
    res = x.map do |el|
      str = el.to_s(2)
      str = '0' * (16 - str.size) + str
    end.join
    return res
  end

  def process_file file
    while (!file.eof?) do
      _64_bits = ''
      8.times do
        byte = file.getbyte
        unless byte.nil?
          byte_s = byte.to_s(2)
          byte_s = '0' * (8 - byte_s.size) + byte_s
          _64_bits += byte_s
        end
      end

      _64_bits = '0' * (64 - _64_bits.size) + _64_bits
      res = do_64_bits _64_bits
      print res.scan(/.{8}/).map{ |byte| byte.to_i(2).chr }.join
    end
  end

  private

  def multiply arg1, arg2
    arg1 = 65536 if arg1 == 0
    arg2 = 65536 if arg2 == 0
    res = (arg1 * arg2) % 65537 % 65536
    res
  end

  def add arg1, arg2
    (arg1 + arg2) % 65536
  end

  def xor arg1, arg2
    arg1 ^ arg2
  end
end

key = File.open(ARGV[1]) do |file|
  key = file.read(16).bytes.to_a.map do |el|
    s = el.to_s(2)
    s = '0' * (8 - s.size) + s
  end.join.to_i(2)
end

if ARGV[0] == '--encode'
  idea = IDEA.new(key, :encode)
elsif ARGV[0] == '--decode'
  idea = IDEA.new(key, :decode)
end

input = ARGV[2].nil? ? $stdin : File.open(ARGV[2])
idea.process_file input
