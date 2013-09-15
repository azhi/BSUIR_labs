require 'mathn'

module SequenceStats
  class MathStatistics
    attr_accessor :sequence

    def initialize(sequence)
      raise ArgumentError, 'must be kind of enumerable' unless sequence.kind_of?(Enumerable)
      @sequence = sequence

      @min = sequence.min
      @max = sequence.max
      @length = @max - @min
    end

    def expected_value
      @expected_value ||= sequence.inject(0){ |res, el| res + el } / sequence.size
    end

    def variance
      @variance ||= sequence.inject(0){ |res, el| res + (el - expected_value) ** 2 } / (sequence.size - 1)
    end

    def standard_deviation
      @standard_deviation ||= variance ** 0.5
    end

    def indirect_uniformity_check
      @indirect_uniformity_check ||=
        sequence.each_slice(2).map{ |el1, el2| el1 ** 2 + el2 ** 2 }.count{ |el| el < 1 } / sequence.size * 2
    end
  end
end
