require 'mathn'

class Integer
  def to_vector(k)
    Vector[*self.to_s(2).rjust(k, ?0).split('').map(&:to_i)]
  end

  class << self
    def from_vector(vec)
      vec.to_a.map{ |i| i % 2 }.join.to_i(2)
    end
  end
end

class MatrixChiper
  attr_reader :k, :r, :h, :g

  def encode(m)
    Integer.from_vector((g * m.to_vector(k)).collect{ |e| e % 2 })
  end

  def calc_syndrom(c)
    Integer.from_vector((h * c.to_vector(k+r)).collect{ |e| e % 2 })
  end

  def decode(c)
    c.to_s(2).rjust(r+k, ?0).split('').first(k).join.to_i(2)
  end
end

class RepeatChiper < MatrixChiper
  def initialize(k)
    @k = k
    @r = k

    @h = Matrix.build(k, 2 * k) do |i, j|
      j % k == i ? 1 : 0
    end

    @g = @h.t
  end
end

class OddChiper < MatrixChiper
  def initialize(k)
    @k = k
    @r = 1

    @g = Matrix.build(k, k+1) do |i, j|
      if j == i || j == k+1
        1
      else
        0
      end
    end.t

    @h = Matrix[[1] * (k+1)]
  end
end

class RectangleChiper < MatrixChiper
  def initialize(a, b)
    @a = a
    @b = b
    @k = a * b
    @r = a + b

    ga = (0...b).map{ |i|
      z = (0...a).map{ |j| (1 << (i*a + j)).to_vector(k).to_a }
      z << z.reduce(&:zip).map(&:flatten).map{ |x| x.reduce(:+) }
    }.reduce(:+)
    ga += (0...a).map do |i|
      (0...b).map{ |j| 1 << (i + j*a) }.reduce(:+).to_vector(k).to_a
    end
    @g = Matrix.rows(ga)

    a1 = ("1" * (a+1)).to_i(2)
    ha = (0...b).map{ |i| (a1 << (i * (a+1))).to_vector(@k + @r).to_a.reverse }
    ha += (0...a).map{ |i| (0..b).map{ |j| 1 << (j * (a+1) + i) }.reduce(:+).to_vector(@k + @r).to_a.reverse }
    @h = Matrix.rows(ha)
  end

  def decode(c)
    Integer.from_vector(c.to_vector(@k + @r).to_a.find_all.with_index{ |e, i| (i+1) % (@a+1) != 0 && i < (@a+1)*@b }.reverse)
  end
end

class TriangleChiper < MatrixChiper
  def initialize(a)
    @a = a
    @k = a * (a + 1) / 2
    @r = a + 1

    t = 0
    ga = (0...a).map{ |i|
      z = (0...(a-i)).map{ |j| r = 1 << t; t += 1; r.to_vector(@k).to_a }
      if z.size != 1
        z << z.reduce(&:zip).map(&:flatten).map{ |x| x.reduce(:+) }
      else
        z << z.first
      end
    }.reduce(:+)
    ga << (0...a).map{ |i| "1" + "0" * (a-i-1) }.join.reverse.split('').map(&:to_i)
    @g = Matrix.rows(ga)

    ha = (0...a).map{ |i|
      (
       "0" * ((@r-i+1..@r).reduce(:+) || 0) +
       "1" * (@r - i) +
       "0" * ((1...@r-i).reduce(:+) || 0)
      ).split('').map(&:to_i)
    } << (0..a).map{ |i| "1" + "0" * i }.reverse.join.split('').map(&:to_i)
    @h = Matrix.rows(ha)

  end

  def decode(c)
    v = c.to_vector(@k+@r).to_a
    r = []
    (1..@a).map{ |i| r << v[0..(@a-i)]; v = v[@r-i+1..-1] }
    Integer.from_vector(r.flatten.reverse)
  end
end

class HammingChiper < MatrixChiper
  def initialize(k)
    @k = k
    @r = (1..2**k).find{ |r| k+r >= 1 << (r-1) and k+r < 1 << r }

    res = []
    i = 0
    while res.size <= k
      res += ([1] * (r-i) + [0] * i).permutation(r).to_a.uniq.reverse
      i += 1
    end
    @ha = Matrix.columns(res.first(k))
    @a = @ha.t

    # @g = Matrix.build(k, k + r) do |i, j|
    #   if j < k
    #     i == j ? 1 : 0
    #   else
    #     @a[i, j - k]
    #   end
    # end

    # @h = Matrix.build(r, k + r) do |i, j|
    #   if j > k - 1
    #     i + k == j ? 1 : 0
    #   else
    #     @ha[i - r, j]
    #   end
    # end

    of = 0
    gv = (1..(k+r)).map{ |j|
      if j.to_s(2).count('1') == 1
        of += 1
        (1..(k+r)).select{ |i| i.to_s(2).count('1') != 1 }.
          map{ |i| i | j == i ? 1 : 0 }
      else
        (1..k).map{ |i| i == j - of ? 1 : 0 }
      end
    }
    @g = Matrix.columns(gv).t

    @h = Matrix.columns((1..(k+r)).map{ |j| j.to_vector(r).to_a.reverse })
  end

  def decode(c)
    c.to_s(2).rjust(k+r, ?0).split('').select.
      with_index{ |t, i| (i+1).to_s(2).count('1') != 1 }.join.to_i(2) ^
      calc_syndrom(c)
  end
end

class ExtendedHammingChiper < HammingChiper
  def initialize(k)
    super
    @r += 1

    @g = Matrix.build(k, k + r) do |i, j|
      if j < k+r-1
        g[j, i]
      else
        i == k-1 ? 0 : 1
      end
    end.t

    @h = Matrix.build(r, k + r) do |i, j|
      if i == r - 1
        1
      elsif j == k + r - 1
        0
      else
        h[i, j]
      end
    end
  end
end

def calc_ext_ham_vel
  (1..14).map{ |k|
    eh = ExtendedHammingChiper.new(k)
    (1...(1 << k)).map{ |i|
      k.to_f / Math.log2(eh.encode(i)).ceil
    }.reduce(:+) / (1 << k)
  }.reduce(:+) / 14
end
