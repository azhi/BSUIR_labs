require File.join(File.dirname(__FILE__), 'linear_coder')

class TrCoder < LinearCoder
  def initialize n
    k = ((n + 1) / 2.0 * n).round(0)
    @c = n + 1
    super(n + k + 1, k)
  end

  def encode msg
    raise "Message length should be #{@k}!" if msg > 2 ** @k - 1
    res = []

    msg = msg.to_s(2).rjust(@k, '0')
    (1...@c).to_a.reverse.each do |i|
      res << msg[0...i]
      msg = msg[i..-1]
      res[-1] += (res.map{ |el| el[i] }.compact.join + res[-1]).count('1') % 2 == 0 ? '0' : '1'
    end
    res << res.map{ |el| el[0] }.join.count('1') % 2 == 0 ? '0' : '1'
    res.join.to_i(2)
  end

  def pretty_encode msg
    enc = encode(msg).to_s(2).rjust(@n, '0')
    (1...@c).to_a.reverse.each do |i|
      puts "#{enc[0..i]}"
      enc = enc[(i + 1)..-1]
    end
    puts enc[-1]
  end

  def decode msg
    raise "Message length should be #{@n}!" if msg > 2 ** @n - 1
    res = []
    errors = []

    msg = msg.to_s(2).rjust(@n, '0')
    (1...@c).to_a.reverse.each do |i|
      res << msg[0...i]
      if (res.map{ |el| el[i] }.compact.join + msg[0..i]).count('1') % 2 != 0
        errors = errors.map{ |el| el[i] = '1' if i < el.length ; el }
        errors << '1' * i
      else
        errors << '0' * i
      end
      msg = msg[(i + 1)..-1]
    end
    errors = errors.map{ |el| el[0] = '1'; el } if (res.map{ |el| el[0] }.join + msg[-1]).count('1') % 2 != 0

    return [res.join.to_i(2), errors.join.to_i(2)]
  end

  def decode_table msg
    msg = decode_table_raw(msg).to_s(2).rjust(@n, '0')
    res = []
    (1...@c).to_a.reverse.each do |i|
      res << msg[0...i]
      msg = msg[(i + 1)..-1]
    end
    res.join.to_i(2)
  end
end
