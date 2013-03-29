require File.join(File.dirname(__FILE__), 'linear_coder')

class RectCoder < LinearCoder
  def initialize x, y
    @x = x; @y = y
    super(x * y + x + y, x * y)
  end

  def encode msg
    raise "Message length should be #{@k}!" if msg > 2 ** @k - 1
    res = ""
    column_odds = 0

    msg.to_s(2).rjust(@k, '0').scan(/.{#{@x}}/) do |row|
      row_odd = row.count('1') % 2 == 0 ? '0' : '1'
      res += row + row_odd
      column_odds ^= row.to_i(2)
    end
    res += column_odds.to_s(2).rjust(@x, '0')
    res.to_i(2)
  end

  def pretty_encode msg
    enc = encode(msg).to_s(2).rjust(@n, '0')
    enc.scan(/.{#{@x + 1}}/) do |row|
      puts "#{row[0...-1]} | #{row[-1]}"
    end
    puts '-' * @x
    puts enc[-@x..-1]
    enc.to_i(2)
  end

  def decode msg
    raise "Message length should be #{@n}!" if msg > 2 ** @n - 1
    res = ""
    column_odds = 0

    msg = msg.to_s(2).rjust(@n, '0')
    error_row = nil; error_column = nil
    msg.scan(/.{#{@x + 1}}/).each.with_index do |row, i|
      res += row[0...-1]
      error_row ||= i if row.count('1') % 2 != 0
      column_odds ^= row.to_i(2)
    end
    column_odds = column_odds.to_s(2).rjust(@x, '0')
    column_odds_msg = msg[-@x..-1]
    (0...@x).each do |i|
      error_column ||= i if column_odds[i] != column_odds_msg[i]
    end

    errors = '0' * @k
    if error_row && error_column
      err_index = error_row * @x + error_column
      res[err_index] = res[err_index] == '0' ? '1' : '0'
    elsif error_row
      err_index = error_row * @x
      errors[err_index...err_index + @x] = '1' * @x
    elsif error_column
      (error_column...@k).step(@x).each{ |ind| errors[ind] = '1' }
    end

    return [res.to_i(2), errors.to_i(2)]
  end

  def decode_table msg
    res = decode_table_raw(msg).to_s(2).rjust(@n, '0')
    res = res[0...-@x]
    res.scan(/.{#{@x + 1}}/).map{ |row| row[0...-1] }.join.to_i(2)
  end
end
