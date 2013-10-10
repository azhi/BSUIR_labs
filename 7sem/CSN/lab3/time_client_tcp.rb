require 'socket'

host = ARGV[0]
socket = Socket.new(:INET, :STREAM)
local_adr = Addrinfo.tcp(host, 37)
begin
  socket.connect(local_adr)
  time_bin = socket.gets
  time_int = time_bin.unpack('L>').first
  time = (Time.utc(1900, 1, 1) + time_int).localtime
  puts "#{time_int} - #{time}"
rescue Exception => e
  p e
end
