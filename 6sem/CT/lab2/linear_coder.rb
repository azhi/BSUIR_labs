require File.join(File.dirname(__FILE__), 'contigious_classes_table_generator')

class LinearCoder
  attr_reader :n

  def initialize n, k
    @n = n; @k = k
    first = generate_all_words(k).map{ |el| encode(el) }
    @table = ContigiousClassesTableGenerator.generate n, first
  end

  def decode_table_raw msg
    row = @table.find{ |row| row.include?(msg) }
    msg ^ row[0]
  end

  def contigious_classes_table
    @table
  end

  def pretty_contigious_classes_table
    puts "\n" + @table.map{ |row| row.map{ |el| el.to_s(2).rjust(@n, '0') }.join(' ') }.join("\n") + "\n"
  end

  private
    def generate_all_words n
      res = []
      word = 0
      while word < 2 ** n do
        res << word
        word += 1
      end
      res
    end
end
