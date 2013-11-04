class Queue
  attr_reader :summary_time_in_queue, :fault_count, :tasks_in_queue_count, :current_n

  def initialize(max_n = 1)
    @max_n = max_n
    @current_n = 0
    @summary_time_in_queue = 0
    @tasks_in_queue_count = 0
    @fault_count = 0
  end

  def add
    if @current_n + 1 <= @max_n
      @current_n += 1
      @tasks_in_queue_count += 1
    else
      @fault_count += 1
    end
  end

  def rem
    raise 'trying to rem empty queue' if @current_n.zero?
    @current_n -= 1
  end

  def step
    @summary_time_in_queue += @current_n
  end

  def is_empty?
    @current_n.zero?
  end
end
