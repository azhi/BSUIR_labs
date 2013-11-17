require 'green_shoes'
require 'gnuplot'
require 'pry'
require File.join(File.dirname(__FILE__), 'db.rb')
require File.join(File.dirname(__FILE__), 'plotter.rb')

Shoes.app :title => "BAA LAB #2", :height => 630 do
  background "#eee"

  stack :margin => 5 do
    inscription 'From date: ' ; @from_date = edit_line width: 200; @from_date.text = '07.10.13'
    inscription 'To date: ' ; @to_date = edit_line width: 200; @to_date.text = '20.10.13'
    button 'Show stats' do
      gen_img
    end
    @img = flow{ image File.expand_path("../tmp/empty.png", __FILE__), :margin => 20 }
  end

  def gen_img
    from_date = Date.strptime(@from_date.text, '%d.%m.%y')
    to_date = Date.strptime(@to_date.text, '%d.%m.%y')
    data_sql = "SELECT buy_sell, price, sum(amount) AS s_amount
                  FROM deal_tbl
                  WHERE trade_date >= '#{from_date}' AND
                        trade_date <= '#{to_date}'
                  GROUP BY price, buy_sell"
    table_data = DB.query(data_sql).to_a

    outfile = ''
    if table_data.empty?
      outfile = File.expand_path("../tmp/no_data.png", __FILE__)
    else
      outfile = File.expand_path("../tmp/diagr.svg", __FILE__)
      Plotter.price_distribution_diagramm(table_data, outfile)
    end

    @img.clear
    @img.append{ image File.read(outfile), :margin => 20 }
  end
end
