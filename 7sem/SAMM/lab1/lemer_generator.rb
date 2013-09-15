require 'set'
require 'mathn'

class LemerGenerator
  attr_accessor :current_general_pos, :current_periodic_pos
  attr_reader :sequence, :aperiodic_sequence, :periodic_sequence

  NEXT_TYPES = {general: {pos: '@current_general_pos', sequence: '@sequence'},
                periodic: {pos: '@current_periodic_pos', sequence: '@periodic_sequence'}}

  def initialize(r0, a, m)
    @r0 = r0
    @a = a
    @m = m
    generate_sequences

    @current_general_pos = 0
    @current_periodic_pos = 0
  end

  def next(type = :general)
    pos = instance_variable_get(NEXT_TYPES[type][:pos])
    sequence = instance_variable_get(NEXT_TYPES[type][:sequence])
    res = sequence[pos]
    instance_variable_set(NEXT_TYPES[type][:pos], (pos + 1) % sequence.size)
    res
  end

  private

    def generate_sequences
      @sequence = []
      set = Set.new
      current_int = (@r0 * @a) % @m
      current_fl = current_int / @m
      while !set.include?(current_fl)
        set << current_fl
        @sequence << current_fl
        current_int = (current_int * @a) % @m
        current_fl = current_int / @m
      end
      split_index = @sequence.index(current_fl)
      @aperiodic_sequence = @sequence[0...split_index]
      @periodic_sequence = @sequence[split_index..-1]
    end
end
