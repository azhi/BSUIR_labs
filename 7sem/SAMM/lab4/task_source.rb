require File.join(File.dirname(__FILE__), 'rng')

class ContinuousTaskSource
  attr_reader :tasks_count, :time_to_task

  def initialize
    @tasks_count = 0
    @time_to_task = 0
    next_time_to_task
  end

  def next_time_to_task
    raise NotImplementedError
  end

  def move_time(delta)
    raise "missing an event when moving time" if @time_to_task < delta
    @time_to_task -= delta
    if @time_to_task.zero?
      @tasks_count += 1
      next_time_to_task
      true
    else
      false
    end
  end
end
