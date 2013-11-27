require File.join(File.dirname(__FILE__), 'task_source')

class ExponentialContinuousTaskSource < ContinuousTaskSource
  def initialize(params = {})
    @lambda = params.delete(:lambda) || 2.5
    super()
  end

  def next_time_to_task
    raise "trying to overwrite time_to_task while event didn't happen" unless @time_to_task.zero?
    @time_to_task = -1 / @lambda * Math::log(RNG.rand)
  end
end

