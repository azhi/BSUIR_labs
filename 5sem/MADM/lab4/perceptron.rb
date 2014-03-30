require 'mathn'

class Vector
  def []=(i,val)
    @elements[i] = val
  end
end

class Matrix
  def []=(i, j, val)
    @rows[i][j] = val
  end
end

class Perceptron
  attr_reader :w

  def initialize class_count, attribute_count, c=1
    @class_count = class_count
    @attr_count = attribute_count
    @c = c
    @w = Matrix.zero(@class_count, @attr_count + 1)
    def @w.punish ind, c, x
      self.column_size.times do |j|
        @rows[ind][j] -= c * x[j]
      end
    end
    def @w.compl ind, c, x
      self.column_size.times do |j|
        @rows[ind][j] += c * x[j]
      end
    end
  end

  def recognize vec
    d = (@w * Vector[ *vec, 1]).to_a
    d.index d.max
  end

  def train src_vec, real_class
    vec = Vector[ *src_vec, 1 ]
    d = (@w * vec).to_a
    recognized_class = d.index d.max

    res = recognized_class == real_class

    unless res
      to_punish = d.to_a.map.with_index{ |el, i| [i, el] }.find_all{ |(i, el)| el >= d[real_class] && i != real_class }
      @w.compl(real_class, @c, vec)
      to_punish.each do |(i, el)|
        @w.punish(i, @c, vec)
      end
    end
    res
  end

  def self.try_build_by_samples(class_count, attr_count, samples, train_attempts = 1000)
    perc = self.new(class_count, attr_count)
    valid = false
    train_attempts.times do
      valid = samples.each.with_index.inject(true) do |res, (cl_samples, c_ind)|
        cl_samples.inject(true){ |in_res, sample| in_res && perc.train(sample, c_ind) } && res
      end
      break if valid
    end
    valid && perc
  end
end
