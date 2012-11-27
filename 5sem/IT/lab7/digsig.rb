require File.join(File.dirname(__FILE__), 'sha1.rb')
require File.join(File.dirname(__FILE__), 'rsa.rb')

class RSA_DigitalSignature
  class << self
    def sign_file filename
      f = File.open(filename, 'r')
      str_sha = SHA_1.sha_from_stream(f).to_s(16)
      f.close
      p = Math::get_primes(rand(100..256)).last
      q = Math::get_primes(rand(100..256)).last
      rsa = RSA.create_encoding(p, q, nil)
      # STDERR.puts "#{rsa.r} #{rsa.ko}"
      print ((rsa.r >> 8) & 0xFF).chr
      print (rsa.r & 0xFF).chr
      print ((rsa.ko >> 8) & 0xFF).chr
      print (rsa.ko & 0xFF).chr
      print rsa.encode(str_sha, rsa.ks)
    end

    def check_signature filename
      f = File.open(filename, 'r')
      p str_sha = SHA_1.sha_from_stream(f).to_s(16)
      f.close
      r = STDIN.binmode.read(2).split('').map(&:ord).inject(0){ |res, byte| (res << 8) | byte }
      ko = STDIN.binmode.read(2).split('').map(&:ord).inject(0){ |res, byte| (res << 8) | byte }
      # p [r, ko]
      rsa = RSA.create_decoding(r, ko, nil)
      text = STDIN.binmode.read
      p sha_from_sig = rsa.decode(text, rsa.ko)
      if ( str_sha == sha_from_sig )
        STDERR.puts "Signature verified!"
      else
        STDERR.puts "Signature verification fail"
      end
    end
  end
end
