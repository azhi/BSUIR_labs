require 'set'

class ContigiousClassesTableGenerator
  def self.generate n, first_line
    res = [first_line]
    els = first_line.to_set
    (1..(2 ** n - 1)).to_a.
      sort_by{ |el| el.to_s(2).count('1') * (2 ** n) - el }.
      each do |candidate|
        unless els.include?(candidate)
          res << first_line.map{ |el| el ^ candidate }
          els = els.merge first_line.map{ |el| el ^ candidate }
        end
      end
    res
  end
end
