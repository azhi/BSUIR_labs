require File.join(File.dirname(__FILE__), 'rng')

class ContinuousChannel
  attr_reader :time_to_process

  def initialize(mu)
    @mu = mu
    @time_to_process = Float::INFINITY
    @is_busy = false
  end

  def next_time_to_process
    raise "trying to overwrite time_to_process while event didn't happen" unless @time_to_process.infinite?
    @time_to_process = -1 / @mu * Math::log(RNG.rand)
  end

  def add
    raise 'adding to channel which is already busy' if @is_busy
    @is_busy = true
    next_time_to_process
  end

  def move_time(delta)
    raise "missing an event when moving time" if @time_to_process < delta
    @time_to_process -= delta
    @time_to_process = Float::INFINITY if @time_to_process.zero?
    @is_busy = !@time_to_process.infinite?
  end

  def busy?
    @is_busy
  end
end
