require 'socket'
require 'pry'

PR_FLIP_CHANCE = 0.2
PRICE_RANGE = 100..10000
DELTA_PRICE_RANGE = -1000..1000

prices_count = ARGV[0].to_i
timeout = ARGV[1].to_i
@threads = []
@prices = [0] * prices_count
@prices.map!{ Random.new.rand(PRICE_RANGE) }

@gen_thread = Thread.new do
  random = Random.new
  loop {
    @prices.map!{ |pr| random.rand < PR_FLIP_CHANCE ? pr + random.rand(DELTA_PRICE_RANGE) : pr }
    sleep(timeout)
  }
end

begin
  server = TCPServer.new('127.0.0.1', 3333)
  loop {
    @threads << Thread.start(server.accept) do |client|
      while client.gets
        client.puts @prices.join(?,)
      end
      client.close
    end
  }
rescue SignalException
  puts 'Got signal. Terminating.'
  @threads.each(&:join)
  @gen_thread.kill.join
  server.close
end
