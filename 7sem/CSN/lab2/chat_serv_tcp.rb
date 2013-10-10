require 'socket'
require 'pry'

socket = Socket.new(:INET, :STREAM)
local_adr = Addrinfo.tcp("127.0.0.1", 2222)
@clients = []
@threads = []
socket.bind(local_adr)
socket.listen(5)

begin
  loop {
    @threads << Thread.start(socket.accept) do |sockio, client_adr|
      p [sockio, client_adr]
      @clients << {sockio: sockio, adr: client_adr}
      while line = sockio.gets
        @clients.reject{ |client| client[:adr] == client_adr }.each do |client|
          client[:sockio].puts line
        end
      end
      sockio.close
    end
  }
rescue SignalException
  puts 'Got signal. Terminating.'
  @threads.each(&:join)
  socket.close
end
