require 'pry'

require File.join(File.dirname(__FILE__), 'queue')
require File.join(File.dirname(__FILE__), 'channel')
require File.join(File.dirname(__FILE__), 'exponential_task_source')
require File.join(File.dirname(__FILE__), 'uniform_task_source')

class MSS
  attr_reader :task_source, :queue, :channel, :states

  def initialize(params)
    mu = params.delete(:mu) || 3.0
    queue_size = params.delete(:queue_size) || Float::INFINITY
    source_params = params.delete(:source) || {type: :exponential, lambda: 2.5}
    source_type = source_params.delete(:type)

    class_name = source_type.to_s.capitalize + 'ContinuousTaskSource'
    raise 'Source type not found' unless Object.const_defined?(class_name)
    task_source_class = Object.const_get(class_name)

    @task_source = task_source_class.new(source_params)
    @queue = Queue.new(queue_size)
    @channel = ContinuousChannel.new(mu)
  end

  def move_time(delta)
    has_new_task = task_source.move_time(delta)
    channel.move_time(delta)
    queue.move_time(delta)
    queue.add if has_new_task
    if !queue.empty? && !channel.busy?
      queue.rem
      channel.add
    end
  end

  def self.simulate(params = {})
    max_time = params.delete(:max_time) || 100_000
    system = self.new(params)
    current_time = 0.0
    while (current_time < max_time)
      delta = [system.task_source.time_to_task, system.channel.time_to_process].min
      system.move_time(delta)
      current_time += delta
    end
    {
      average_queue_length: system.queue.time_states.inject(0.0){ |sum, (k, v)| sum + k * v / current_time },
      average_time_in_queue: system.queue.summary_time_in_queue / (system.task_source.tasks_count - system.queue.fault_count).to_f
    }
  end

  def self.pretty_simulate(params = {})
    res = simulate(params)
    puts '*' * 80
    puts res.map{ |k, v| '* ' + k.to_s.gsub('_', ' ').capitalize + ':' + v.to_s.rjust(75 - k.to_s.size) + ' *' }.join("\n")
    puts '*' * 80
  end
end
