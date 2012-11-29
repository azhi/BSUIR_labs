class SHA_1
  class << self

    def sha_from_stream stream
      init
      size = 0
      until stream.eof? do
        block = stream.binmode.read(64).split('').map(&:ord)
        size += block.size
        break if block.size < 64
        input = block.each_slice(4).map{ |_4bytes| _4bytes.inject(0){ |res, byte| (res << 8) | byte } }
        calc_block input
      end
      align_last_block(block, size)
      res = [@@A, @@B, @@C, @@D, @@E].inject(0){ |res, _32bytes| (res << 32) | _32bytes }
    end

    private

    def init
      @@A = 0x67452301
      @@B = 0xEFCDAB89
      @@C = 0x98BADCFE
      @@D = 0x10325476
      @@E = 0xC3D2E1F0
      @@module = 2 ** 32
    end

    def align_last_block block, size
      block.clear if block.size == 64
      block << (1 << 7)
      if block.size > 56
        while block.size != 64 do
          block << 0
        end
        input = block.each_slice(4).map{ |_4bytes| _4bytes.inject(0){ |res, byte| (res << 8) | byte } }
        calc_block input
        block.clear
      end

      while block.size != 56
        block << 0
      end
      block += split_to_8_bytes size * 8
      input = block.each_slice(4).map{ |_4bytes| _4bytes.inject(0){ |res, byte| (res << 8) | byte } }
      calc_block input
    end

    def split_to_8_bytes inp
      [ (inp >> 56) & 0xFF,
        (inp >> 48) & 0xFF,
        (inp >> 40) & 0xFF,
        (inp >> 32) & 0xFF,
        (inp >> 24) & 0xFF,
        (inp >> 16) & 0xFF,
        (inp >> 8) & 0xFF,
        (inp) & 0xFF ]
    end

    def calc_block input
      @@w = input.dup
      a, b, c, d, e = @@A, @@B, @@C, @@D, @@E
      (1..80).each do |t|
        tmp = (cshl(a, 5) + ft(t, b, c, d)) % @@module
        tmp = (tmp + e) % @@module
        tmp = (tmp + kt(t)) % @@module
        tmp = (tmp + wt(t)) % @@module

        e = d
        d = c
        c = cshl(b, 30)
        b = a
        a = tmp
      end
      @@A = (@@A + a) % @@module
      @@B = (@@B + b) % @@module
      @@C = (@@C + c) % @@module
      @@D = (@@D + d) % @@module
      @@E = (@@E + e) % @@module
    end

    def cshl num, n, size = 32
      mask = (1 << size) - 1
      ( ( (num << n) & mask & (mask << n) ) |
        ( (num >> size - n) & ((1 << n) - 1) ) )
    end

    def wt t
      t < 17 ? @@w[t-1] : @@w[t-1] = cshl(@@w[t-4] ^ @@w[t-9] ^ @@w[t-15] ^ @@w[t-17], 1)
    end

    def kt t
      case t
      when 1..20
        0x5A827999
      when 21..40
        0x6ED9EBA1
      when 41..60
        0x8F1BBCDC
      when 61..80
        0xCA62C1D6
      else
        raise "(kt) Wrong t argument called"
      end
    end

    def ft t, x, y, z
      case t
      when 1..20
        (x & y) | (~x & z)
      when 21..40, 61..80
        x ^ y ^ z
      when 41..60
        (x & y) | (x & z) | (y & z)
      else
        raise "(ft) Wrong t argument called"
      end
    end
  end
end
