require File.join(File.dirname(__FILE__), 'linear_coder')

class OddCoder < LinearCoder
  def initialize n, k
    raise "n - k should be == 1" if n - k != 1
    super
  end

  def encode msg
    raise "Message length should be #{@k}!" if msg > 2 ** @k - 1
    odd = msg.to_s(2).count('1') % 2 == 0 ? 0 : 1
    (msg << 1) + odd
  end

  def decode msg
    raise "Message length should be #{@n}!" if msg > 2 ** @n - 1
    odd = msg.to_s(2).count('1') % 2 != 0
    if odd
      errors = 2 ** @k - 1
    else
      errors = 0
    end
    return [msg >> 1, errors]
  end

  def decode_table msg
    res = decode_table_raw msg
    res >> 1
  end
end
