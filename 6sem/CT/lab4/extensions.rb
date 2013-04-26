class Vector
  def []=(i,val)
    @elements[i] = val
  end

  def mul(vec)
    if vec.kind_of?(Vector)
      res_ar = []
      (0...self.size).each{ |i| res_ar << self[i] * vec[i] }
      Vector[*res_ar]
    else
      raise "mul can be applied to vectors only"
    end
  end

  def ^(vec)
    if vec.kind_of?(Vector)
      res_ar = []
      (0...self.size).each{ |i| res_ar << (self[i] + vec[i]) % 2 }
      Vector[*res_ar]
    else
      raise "^ can be applied to vectors only"
    end
  end

  def negate
    res_ar = []
    self.each{ |el| res_ar << (el == 1 ? 0 : 1) }
    Vector[*res_ar]
  end
end

class Matrix
  def []=(i, j, val)
    @rows[i][j] = val
  end
end

module Math
  def self.cnk n, k
    fact(n) / fact(k) / fact(n-k)
  end

  def self.fact n
    if n == 0 || n == 1
      1
    else
      (2..n).inject(1){ |sum, el| sum * el }
    end
  end
end

