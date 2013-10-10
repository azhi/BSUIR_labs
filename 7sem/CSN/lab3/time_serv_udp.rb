require 'socket'

socket = Socket.new(:INET, :DGRAM)
local_adr = Addrinfo.udp("127.0.0.1", 37)
socket.bind(local_adr)

begin
  loop {
    msg, client_adr = socket.recvfrom(255)
    socket.send [Time.now.utc - Time.utc(1900, 01, 01)].pack('L>'), 0, client_adr
  }
rescue SignalException
  puts 'Got signal. Terminating.'
ensure
  socket.close
end
