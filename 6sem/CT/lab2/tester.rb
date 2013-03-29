class Tester
  def self.test_decode num, lc, dec_sym
    errors = get_all_errors num.to_s(2).rjust(lc.n, '0'), [false] * lc.n
    res = {}
    decoded_num = lc.send(dec_sym, num)
    decoded_num = decoded_num[0] if decoded_num.is_a?(Array)
    errors.each do |error|
      response = lc.send(dec_sym, error[0])
      res[error[1]] ||= {}
      res[error[1]][:success] ||= 0
      res[error[1]][:detect] ||= 0
      res[error[1]][:failure] ||= 0
      if response.is_a?(Array)
        if response[0] == decoded_num
          res[error[1]][:success] += 1
        elsif response[1] != 0
          res[error[1]][:detect] += 1
        else
          res[error[1]][:failure] += 1
        end
      else
        if response == decoded_num
          res[error[1]][:success] += 1
        else
          res[error[1]][:failure] += 1
        end
      end
    end
    puts "====================== RESULTS ====================="
    res.each do |k, v|
      if dec_sym == :decode
        puts "#{k} errors: #{v[:success]} corrected, #{v[:detect]} detected, #{v[:failure]} failed"
      else
        puts "#{k} errors: #{v[:success]} corrected, #{v[:failure]} failed"
      end
    end
    puts "----------------------------------------------------"
  end

  private
    def self.get_all_errors num, errors, res = []
      (0...num.length).each do |i|
        unless errors[i]
          num[i] = num[i] == '0' ? '1' : '0'
          errors[i] = true
          res << [num.to_i(2), errors.count(true)] unless res.any?{ |el| el[0] == num.to_i(2) }
          get_all_errors num, errors, res
          errors[i] = false
          num[i] = num[i] == '0' ? '1' : '0'
        end
      end
      res
    end
end
