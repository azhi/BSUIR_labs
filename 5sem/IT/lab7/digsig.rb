require File.join(File.dirname(__FILE__), 'sha1.rb')
require File.join(File.dirname(__FILE__), 'rsa.rb')

class RSA_DigitalSignature
  class << self
    def sign_file filename
      f = File.open(filename, 'r')
      sha1sum = SHA_1.sha_from_stream(f)
      str_sha = ""
      while sha1sum != 0 do
        byte = sha1sum & 0xFF
        sha1sum >>= 8
        str_sha += byte.chr
      end
      str_sha += '\0' if str_sha.size % 2 == 1
      f.close

      p = Math::get_primes(rand(100..256)).last
      q = Math::get_primes(rand(100..256)).last
      rsa = RSA.create_encoding(p, q, nil)
      STDERR.puts "ko:#{rsa.ko} r:#{rsa.r}"
      print rsa.encode(str_sha, rsa.ks)
    end

    def check_signature filename, ko, r
      f = File.open(filename, 'r')
      sha_num = SHA_1.sha_from_stream(f)
      f.close
      rsa = RSA.create_decoding(r, ko, nil)
      text = STDIN.binmode.read
      begin
        sha_from_sig = rsa.decode(text, rsa.ko)
      rescue Object => e
        puts e
        puts
        puts "[FAIL] Signature verification failed!"
        exit(1)
      end
      sha_from_sig_num = sha_from_sig.each_byte.to_a.reverse.inject(0){ |res, byte| (res << 8) | byte }
      puts "file SHA-1:      " + sha_num.to_s(16)
      puts "signature SHA-1: " + sha_from_sig_num.to_s(16)
      puts
      if ( sha_num == sha_from_sig_num )
        puts "[SUCCESS] Signature verified!"
      else
        puts "[FAIL] Signature verification failed!"
      end
    end
  end
end
