require File.join(File.dirname(__FILE__), '../lab3/queue.rb')

class Queue
  attr_reader :time_states

  alias_method :old_initialize, :initialize
  def initialize(max_n = Float::INFINITY)
    @time_states = {}
    old_initialize(max_n)
  end

  remove_method :step

  def move_time(delta)
    @summary_time_in_queue += @current_n * delta
    @time_states[@current_n] ||= 0.0
    @time_states[@current_n] += delta
  end
end
