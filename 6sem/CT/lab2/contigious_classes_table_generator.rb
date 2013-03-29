class ContigiousClassesTableGenerator
  def self.generate n, first_line
    res = [first_line]
    (1...n).each do |i|
      candidates = all_power i, n
      candidates.each do |candidate|
        unless res.flatten.include?(candidate)
          res << first_line.map{ |el| el ^ candidate }
        end
      end
    end
    res
  end

  private
    def self.all_power power, n
      [ ['0'] * (n - power) + ['1'] * power ].flatten.permutation.to_a.
        reverse.uniq.map{ |el| el.join.to_i(2) }
    end
end
