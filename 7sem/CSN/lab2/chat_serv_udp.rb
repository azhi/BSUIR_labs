require 'socket'
require 'pry'

socket = Socket.new(:INET, :DGRAM)
local_adr = Addrinfo.udp("127.0.0.1", 2222)
@clients = []
@threads = []
socket.bind(local_adr)

begin
  loop {
    msg, client_adr = socket.recvfrom(255)
    if msg.start_with?("REGISTER")
      p new_client = {name: msg.sub('REGISTER ', '').chomp, addr: client_adr}
      @clients << new_client
    else
      msg, to, msg = msg.match(/(.*?) (.*)/).to_a
      from = @clients.find{ |client| client[:addr].to_s == client_adr.to_s }
      to = to == 'ALL' ? @clients.reject{ |client| client[:name] == from[:name] } : @clients.find_all{ |client| client[:name] == to }
      to.each do |client|
        socket.send "FROM: #{from[:name]}\n" + msg, 0, client[:addr]
      end
    end
  }
rescue SignalException
  puts 'Got signal. Terminating.'
  socket.close
end
