require 'date'
require File.join(File.dirname(__FILE__), 'db.rb')

rand = Random.new
(Date.parse('13-09-01')..Date.today).each do |date|
  dir = {buy: -1, sell: 1}
  [:buy, :sell].each do |bs|
    deal_count = rand.rand(0..20)
    deal_count.times do
      price = rand.rand(10..30)
      amount = rand.rand(10000..100000)
      DB.query("INSERT INTO deal_tbl VALUES (0, '#{date}', #{dir[bs]}, #{price}, #{amount})")
    end
  end
end
