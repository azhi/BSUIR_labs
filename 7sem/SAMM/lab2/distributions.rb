require File.join(File.dirname(__FILE__), '../lab1/sequence_stats/math_statistics.rb')
require File.join(File.dirname(__FILE__), '../lab1/sequence_stats/diagramm.rb')

N = 32768

class Distribution
  def self.by_type(type)
    class_name = type.capitalize.to_s + 'Distribution'
    raise 'Distribution type not found' unless Object.const_defined?(class_name)
    Object.const_get(class_name)
  end

  def initialize
    @math_statistics = SequenceStats::MathStatistics.new(@sequence)
    @diagramm = SequenceStats::Diagramm.new(@sequence, 20)
  end

  def mx
    @m_mx ||= @math_statistics.expected_value
  end

  def D
    @m_D ||= @math_statistics.variance
  end

  def dx
    @m_dx ||= @math_statistics.standard_deviation
  end

  def diagramm
    @diagramm.gnuplot_diagram
  end

  def random_number
    SecureRandom.random_number
  end
end

class UniformDistribution < Distribution
  def initialize(a, b)
    @a = a
    @b = b
    @sequence = N.times.map{ @a + (@b - @a) * random_number }
    super()
  end
end

class GaussDistribution < Distribution
  def initialize(mx, dx, group_n = 6)
    @mx = mx
    @dx = dx
    @sequence = N.times.map do
      rsum = group_n.times.inject(0){ |sum| sum + random_number }
      @mx + @dx * ((12 / group_n) ** 0.5) * (rsum - group_n / 2.0)
    end
    super()
  end
end

class ExpDistribution < Distribution
  def initialize(lmb)
    @lambda = lmb
    @sequence = N.times.map{ -1 / @lambda * Math::log(random_number) }
    super()
  end
end

class GammaDistribution < Distribution
  def initialize(n, lmb)
    @n = n.to_i
    @lambda = lmb
    @sequence = N.times.map{ -1 / @lambda * Math::log(@n.times.inject(1){ |sum, el| sum * random_number }) }
    super()
  end
end

class TriangleDistribution < Distribution
  def initialize(a, b)
    @a = a
    @b = b
    @sequence = N.times.map{ @a + (@b - @a) * [random_number, random_number].min }
    super()
  end
end

class SimpsonDistribution < Distribution
  def initialize(a, b)
    @a = a
    @b = b
    @sequence = N.times.map{ (@a / 2 + (@b / 2 - @a / 2) * random_number) + (@a / 2 + (@b / 2 - @a / 2) * random_number) }
    super()
  end
end
