require 'pry'

require File.join(File.dirname(__FILE__), 'task_source')
require File.join(File.dirname(__FILE__), 'queue')
require File.join(File.dirname(__FILE__), 'channel')

class MSS
  attr_reader :task_source, :queue, :channels, :states

  def initialize(params)
    time_to_task = params.delete(:time_to_task) || 2
    queue_size = params.delete(:queue_size) || 1
    channels = params.delete(:channels) || [0.8, 0.6]

    @task_source = TaskSource.new(time_to_task)
    @queue = Queue.new(queue_size)
    @channels = []
    channels.each{ |pi| @channels << Channel.new(pi) }
    @states = {}
  end

  def step
    state = task_source.time_to_task.to_s +
            queue.current_n.to_s +
            channels.map{ |c| c.busy? ? '1' : '0' }.join
    @states[state] ||= 0; @states[state] += 1

    has_new_task = task_source.step
    channels.each{ |c| c.step }
    queue.step
    free_channel = channels.find{ |c| !c.busy? }
    if free_channel && !queue.empty?
      queue.rem
      free_channel.add
    end
    free_channel = channels.find{ |c| !c.busy? }
    if has_new_task
      free_channel ? free_channel.add : queue.add
    end
  end

  def self.simulate(params)
    n = params.delete(:n) || 100_000
    debug = params.delete(:debug)
    system = self.new(params)
    n.times{ system.step }
    p system.states.inject({}){ |res, (k, v)| res.merge({k => v / n.to_f}) } if debug
    {
      fault_probability: system.queue.fault_count / system.task_source.tasks_count.to_f,
      average_time_in_queue: system.queue.summary_time_in_queue / (system.task_source.tasks_count - system.queue.fault_count).to_f
    }
  end

  def self.pretty_simulate(params)
    res = simulate(params.merge(debug: false))
    puts '*' * 80
    puts res.map{ |k, v| '* ' + k.to_s.gsub('_', ' ').capitalize + ':' + v.to_s.rjust(75 - k.to_s.size) + ' *' }.join("\n")
    puts '*' * 80
  end
end
