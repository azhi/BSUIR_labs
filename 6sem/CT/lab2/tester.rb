require 'set'

class Tester
  def self.test_decode num, lc, dec_sym
    errors = get_all_errors num, lc.n
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
    def self.get_all_errors num, n
      res = []
      1.upto(2**n-1) do |error|
        res << [error, (error ^ num).to_s(2).count('1')]
      end
      res.sort_by!{ |el| el.last }.shift
      res
    end
end
