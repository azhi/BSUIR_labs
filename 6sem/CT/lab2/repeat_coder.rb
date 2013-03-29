require File.join(File.dirname(__FILE__), 'linear_coder')

class RepeatCoder < LinearCoder
  def initialize n, k
    raise "n % k should be == 0" if n % k != 0
    @r_multiplier = n / k
    super
  end

  def encode msg
    raise "Message length should be #{@k}!" if msg > 2 ** @k - 1
    (msg.to_s(2).rjust(@k, '0') * @r_multiplier).to_i(2)
  end

  def decode msg
    raise "Message length should be #{@n}!" if msg > 2 ** @n - 1
    true_counts = [0] * @k
    msg.to_s(2).rjust(@n, '0').scan(/.{#{@k}}/m) do |o_msg|
      o_msg.each_char.with_index{ |bit, i| true_counts[i] += 1 if bit == '1' }
    end
    res = ""
    errors = ""
    true_counts.each do |count|
      if @r_multiplier - count == count
        errors << '1'
        res << '0'
      elsif count > @r_multiplier / 2
        res << '1'
        errors << '0'
      else
        res << '0'
        errors << '0'
      end
    end
    return [res.to_i(2), errors.to_i(2)]
  end

  def decode_table msg
    res = decode_table_raw msg
    res % (2 ** @k)
  end
end
