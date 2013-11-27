require File.join(File.dirname(__FILE__), 'task_source')

class UniformContinuousTaskSource < ContinuousTaskSource
  def initialize(params = {})
    @a = params.delete(:a) || 0.05
    @b = params.delete(:b) || 0.75
    super()
  end

  def next_time_to_task
    raise "trying to overwrite time_to_task while event didn't happen" unless @time_to_task.zero?
    @time_to_task = @a + (@b - @a) * RNG.rand
  end
end

