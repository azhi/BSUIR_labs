require 'socket'
require 'pry'

socket = Socket.new(:INET, :DGRAM)
local_adr = Addrinfo.udp("127.0.0.1", 2222)
begin
  loop {
    rs, = IO.select [socket, $stdin]
    rs.each do |io_stream|
      if io_stream == $stdin
        line = io_stream.gets
        if line == "q\n"
          socket.close
          exit(0)
        end
        socket.send line, 0, local_adr
      elsif io_stream == socket
        line, = io_stream.recvfrom(255)
        puts "RCVMSG: #{line}"
      else
        $stderr.p io_stream
      end
    end
  }
rescue Exception => e
  unless e.is_a?(SystemExit)
    p e
    $stderr.puts 'CONNECTION TO SERVER LOST. TERMINATING'
  end
end
