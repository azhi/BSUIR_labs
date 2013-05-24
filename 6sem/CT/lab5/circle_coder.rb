require 'mathn'

class CircleCoder
  attr_reader :p, :size, :k, :r, :n

  class << self
    def c1(k) new(11,  k) end
    def c2(k) new(7,   k) end
    def c3(k) new(457, k) end
    def c4(k) new(279, k) end
    def test(size = 5)
      (1..4).map do |i|
        print "c#{i}(#{size}) ... "
        inst = send(:"c#{i}", size)
        res = (0...1 << size).all?{ |j| j == inst.decode(inst.encode(j)) }
        puts res ? "OK" : "FAIL"
        res
      end.all?
    end
  end

  def initialize(p, k)
    @p = p
    @k = k
    @r = p.to_s(2).size
    @n = k + r
    @g = Matrix[*(0...k).map do |i|
      [0] * i +
      p.to_s(2).each_char.map(&:to_i).reverse +
      [0] * (k-i-1)
    end]
  end

  def encode(m)
    raise Exception, "Input does not fit the coder." if Math.log2(m  + 1) > k
    vec_m = Matrix.row_vector(m.to_s(2).rjust(k, '0').each_char.map(&:to_i).reverse)
    vec_c = encode_vector(vec_m)
    c = vec_c.to_a.join.reverse.to_i(2)
  end

  def encode_vector(vec_m)
    (vec_m * @g).map{ |a| a%2 }
  end

  def decode(c)
    m = 0
    (k).downto(0) do |i|
      next if c & (1 << i+r-1) == 0
      c ^= p << i
      m ^= 1 << i
    end
    m
  end
end
