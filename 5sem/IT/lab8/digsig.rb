require File.join(File.dirname(__FILE__), 'key_exchange_client.rb')
require File.join(File.dirname(__FILE__), 'sha1.rb')

class EllipticGroup_DigitalSignature
  def initialize el_g, g, q
    @eg = el_g
    @g = g
    @q = q
    @user = KeyExchangeClient.new @eg, @g
  end

  def sign_file filename
    f = File.open(filename, 'r')
    sha1sum = SHA_1.sha_from_stream(f)
    f.close

    begin
      begin
        k = rand(2...(@q-1))
        kg = @eg.mul @g, k
        r = kg.x % @q
      end while r == 0
      ki = Math::extended_euclid k, @q
      s = ki * ( sha1sum + @user.n * r ) % @q
    end while s == 0
    
    return @user.p, r, s
  end

  def check_signature pa, r, s, filename
    f = File.open(filename, 'r')
    sha1sum = SHA_1.sha_from_stream(f)
    f.close

    raise "Wrong r or q" unless r.between?(1, @q - 1)
    raise "Wrong s or q" unless s.between?(1, @q - 1)

    w = Math::extended_euclid s, @q
    u1 = sha1sum * w % @q
    u2 = r * w % @q

    kgs = @eg.add(@eg.mul(@g, u1), @eg.mul(pa, u2))
    rs = kgs.x % @q

    rs == r
  end
end
