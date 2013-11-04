class TaskSource
  attr_reader :tasks_count, :time_to_task

  def initialize(time_to_task = 2)
    @time_to_task = time_to_task
    @tasks_count = 0
  end

  def step
    @time_to_task -= 1
    if @time_to_task.zero?
      @time_to_task = 2
      @tasks_count += 1
      true
    end
  end
end
