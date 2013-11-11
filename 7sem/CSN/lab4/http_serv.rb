require 'socket'
require 'pry'

@threads = []

HEADER_REGEX_BASE = %q{
  (?<req_type> \w+){0}
  (?<version> \d+\.?\d*){0}
  (?<location> (\w|/|\.)+){0}

  (?<host> (\w|\d|\.|:)*){0}
  (?<keep_alive> Connection:\skeep-alive\r\n){0}
  (?<content_length> \d+){0}
}.strip!
HEADER_REGEX_EL = Hash[* {req_type: '(\g<req_type>)\s*(\g<location>)\s*HTTP/(\g<version>)\r\n',
                          location: '(\g<req_type>)\s*(\g<location>)\s*HTTP/(\g<version>)\r\n',
                          version: '(\g<req_type>)\s*(\g<location>)\s*HTTP/(\g<version>)\r\n',
                          host: 'Host:\s(\g<host>)\r\n',
                          keep_alive: '(\g<keep_alive>)',
                          content_length: 'Content-Length:\s(\g<content_length>)\r\n'}.map do |sym, el|
                            [sym, Regexp.compile(HEADER_REGEX_BASE + el, Regexp::EXTENDED | Regexp::MULTILINE)]
                          end.flatten
                      ]

def parse_header(header)
  [:req_type, :location, :version, :host, :keep_alive, :content_length].each.with_object({}) do |sym, res|
    val = header.match(HEADER_REGEX_EL[sym])
    val = val && val[sym]
    res.merge!(sym => val)
  end
end

def resp_header(header)
  res = ""
  res << "HTTP/#{header[:version]} #{header[:status]} #{header[:status_text]}\r\n"
  res << "Content-Type: text/html; charset=utf-8\r\n"
  res << "Status: #{header[:status]} #{header[:status_text]}\r\n"
  (header[:cookies] || {}).map{ |k, v| "#{k}=#{v}" }.each do |cookie|
    res << "Set-Cookie: #{cookie}\r\n"
  end
  res << "\r\n"
end

BASE_FOLDER = ARGV[0]

sep = '-' * (ENV['COLUMNS'] ? ENV['COLUMNS'].to_i : 80)
rcv_msg = ""
begin
  server = TCPServer.new('127.0.0.1', 3333)
  loop {
    @threads << Thread.new(server.accept) do |client|
      rcv_msg = ""
      while (line = client.gets) != "\r\n"
        rcv_msg += line
      end
      header = parse_header(rcv_msg)
      header[:location] = "/index.html" if header[:location] == '/'
      data = {}
      data_str = ""
      if header[:content_length]
        data_str = client.read(header[:content_length].to_i)
        data = Hash[* data_str.split(?&).map{ |e| e.split(?=) }.flatten ]
      end
      puts 'REQUEST:'
      puts rcv_msg + data_str
      puts sep

      ret_msg = ""
      resp_header_hash = {version: header[:version], status: 200, status_text: 'OK', server: 'CSNLAB4SERV'}
      if %w[HEAD GET POST].include? header[:req_type]
        if File.exists?(File.join(BASE_FOLDER, header[:location]))
          resp_header_hash.merge!(cookies: data) if data.any?
          ret_msg = resp_header(resp_header_hash)
          ret_msg += File.read(File.join(BASE_FOLDER, header[:location])) unless header[:req_type] == 'HEAD'
        else
          resp_header_hash.merge!(status: 404, status_text: 'Not Found')
          ret_msg = resp_header(resp_header_hash)
          ret_msg += File.read(File.join(BASE_FOLDER, '404.html')) unless header[:req_type] == 'HEAD'
        end
      else
        resp_header_hash.merge!(status: 500, status_text: 'Internal Server Error')
        ret_msg = resp_header(resp_header_hash)
        ret_msg += File.read(File.join(BASE_FOLDER, '500.html')) unless header[:req_type] == 'HEAD'
      end

      puts 'RESPONSE:'
      puts ret_msg
      puts sep
      client.print ret_msg
      client.close
    end
  }
rescue SignalException
  puts 'Got signal. Terminating.'
ensure
  @threads.each(&:join)
  server.close
end
