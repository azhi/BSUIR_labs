require File.join(File.dirname(__FILE__), 'rng')

class Channel
  def initialize(pi)
    @pi = pi
    @is_busy = false
  end

  def add
    raise 'adding to channel which is already busy' if @is_busy
    @is_busy = true
  end

  def step
    @is_busy = false if RNG.rand < (1 - @pi)
  end

  def busy?
    @is_busy
  end
end
