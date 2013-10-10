require 'socket'
require 'pry'

socket = Socket.new(:INET, :STREAM)
local_adr = Addrinfo.tcp("127.0.0.1", 2222)
begin
  socket.connect(local_adr)
  loop {
    rs, = IO.select [socket, $stdin]
    rs.each do |io_stream|
      line = io_stream.gets
      if io_stream == $stdin
        if line == "q\n"
          socket.close
          exit(0)
        end
        socket.print line
      elsif io_stream == socket
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
