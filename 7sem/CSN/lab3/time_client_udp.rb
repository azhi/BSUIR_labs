require 'socket'

host = ARGV[0]
socket = Socket.new(:INET, :DGRAM)
local_adr = Addrinfo.udp(host, 37)
begin
  socket.send '', 0, local_adr
  time_bin, = socket.recvfrom(255)
  time_int = time_bin.unpack('L>').first
  time = (Time.utc(1900, 1, 1) + time_int).localtime
  puts "#{time_int} - #{time}"
rescue Exception => e
  p e
end
