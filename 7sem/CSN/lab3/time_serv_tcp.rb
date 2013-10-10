require 'socket'

socket = Socket.new(:INET, :STREAM)
local_adr = Addrinfo.tcp("127.0.0.1", 37)
socket.bind(local_adr)
socket.listen(5)

begin
  loop {
    sockio, = socket.accept
    sockio.puts [Time.now.utc - Time.utc(1900, 01, 01)].pack('L>')
    sockio.close
  }
rescue SignalException
  puts 'Got signal. Terminating.'
ensure
  socket.close
end
