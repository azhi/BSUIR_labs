require File.join(File.dirname(__FILE__), 'math_extention.rb')

Point = Struct.new(:x, :y)
class Point
  def to_s
    "(#{self.x}; #{self.y})"
  end
end

class EllipticGroup
  attr_reader :elements, :zero, :m, :a, :b

  def initialize a, b, m
    raise "m is not a prime number" unless Math::prime? m
    raise "bad a and b" if 4 * a ** 3 + 27 * b ** 2 == 0
    @m = m
    @a = a
    @b = b
    @elements = []
    (0...m).map{ |x| [x, x ** 3 + a * x + b] }.each do |x, y|
      sq_r1, sq_r2 = Math::square_root_mod y, m
      unless sq_r1.is_a?(Float) && !sq_r1.finite?
        if sq_r1 == sq_r2
          @elements << Point.new(x, sq_r1)
          @zero = Point.new(x, sq_r1)
        else
          @elements << Point.new(x, sq_r1)
          @elements << Point.new(x, sq_r2)
        end
      end
    end

    raise "can't find a zero point" unless @zero
  end

  def add p, q
    raise ArgumentError unless @elements.include?(p) && @elements.include?(q)

    return p if q == @zero
    return q if p == @zero
    return @zero if ((p.y + q.y) % @m).zero?
    return sq2 p if p == q

    div = Math::extended_euclid((p.x - q.x) % @m, @m)
    l = (p.y - q.y) * div % @m
    xr = (l ** 2 - p.x - q.x) % @m
    yr = (l * (p.x - xr) - p.y) % @m
    res = Point.new(xr, yr)

    raise ArgumentError unless @elements.include?(res)
    res
  end

  def sq2 p
    return @zero if (p.y % @m).zero?
    div = Math::extended_euclid (2 * p.y) % @m, @m
    raise ArgumentError if div.is_a?(Float) && !div.finite?
    l = (3 * p.x ** 2 + @a) * div
    xr = (l ** 2 - 2 * p.x) % @m
    yr = (l * (p.x - xr) - p.y) % @m
    res = Point.new(xr, yr)

    raise ArgumentError unless @elements.include?(res)
    res
  end

  def mul p, k
    @mul_cache ||= {}
    @mul_cache[p] ||= {}
    @mul_cache[p][k] ||=
      if k == 0
        @zero
      elsif p == @zero
        @zero
      elsif k == 1
        p
      else
        pk = mul(p, k / 2)
        res = sq2(pk)
        res = add p, res if k.odd?
        res
      end
  end

  def order g
    (1...@m).drop_while{ |c| mul(g, c) != @zero }.first
  end

  def self.get_all_existing_elliptic_groups m
    res = []
    (0...m).each do |a|
      (0...m).each do |b|
        begin
          res << new(a, b, m)
        rescue Object
        end
      end
    end
    res
  end
end
