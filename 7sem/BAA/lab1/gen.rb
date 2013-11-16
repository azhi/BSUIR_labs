require 'date'
require File.join(File.dirname(__FILE__), 'db.rb')

rand = Random.new
(Date.parse('13-09-01')..Date.today).each do |date|
  dc = rand.rand(0..100)
  a = dc.times.inject(0){ |sum, el| sum + rand.rand(-9700..10000) }
  DB.query("INSERT INTO pnl_tbl VALUES ('#{date}', #{dc}, #{a})")
end
