require 'green_shoes'
require 'gnuplot'
require 'mail'
require 'pry'
require File.join(File.dirname(__FILE__), 'mail_config.rb')
require File.join(File.dirname(__FILE__), 'db.rb')
require File.join(File.dirname(__FILE__), 'plotter.rb')

Shoes.app :title => "BAA LAB #1", :height => 630, :width => 900 do
  background "#FFF".."#333", :angle => 60

  flow do
    stack :margin => 5, :width => 250 do
      inscription 'From date: ' ; @from_date = edit_line width: 200; @from_date.text = '07.10.13'
      inscription 'To date: ' ; @to_date = edit_line width: 200; @to_date.text = '20.10.13'
      button 'Show stats' do
        gen_img
      end
      inscription 'Mail to: ' ; @mail_address = edit_line width: 200
      button 'Mail' do
        send_mail
      end
    end
    stack :margin => 5, :width => 650 do
      @img = flow do
        image File.expand_path("../tmp/empty.png", __FILE__), :margin => 20
        image File.expand_path("../tmp/empty.png", __FILE__), :margin => 20
      end
    end
  end

  def gen_img
    from_date = Date.strptime(@from_date.text, '%d.%m.%y')
    to_date = Date.strptime(@to_date.text, '%d.%m.%y')
    data_sql = "SELECT * FROM pnl_tbl WHERE date >= '#{from_date}' AND
                                            date <= '#{to_date}'"
    table_data = DB.query(data_sql).to_a

    initial_amount_sql = "SELECT SUM(amount) AS s_amount FROM pnl_tbl WHERE date < '#{from_date}'"
    initial_amount_sum = DB.query(initial_amount_sql).first['s_amount']  || 0.0

    out_diagr_path = ''
    out_graph_path = ''
    if table_data.empty?
      out_diagr_path = File.expand_path("../tmp/no_data.png", __FILE__)
      out_graph_path = File.expand_path("../tmp/no_data.png", __FILE__)
    else
      out_diagr_path = File.expand_path("../tmp/diagr.png", __FILE__)
      out_graph_path = File.expand_path("../tmp/graph.png", __FILE__)
      Plotter.new(initial_amount_sum, table_data).
              profit_diagramm(out_diagr_path).
              profit_sum_graph(out_graph_path)
    end
    @img.clear
    @img.append do
      image out_diagr_path, :margin => 20
      image out_graph_path, :margin => 20
    end
  rescue Exception => e
    binding.pry
  end

  def send_mail
    to_adr = @mail_address.text
    mail = Mail.new do
      from         'system@baa_labs.org'
      to           to_adr
      subject      'Profit diagramm'
      add_file     File.expand_path("../tmp/diagr.png", __FILE__)
      add_file     File.expand_path("../tmp/graph.png", __FILE__)
    end
    html_part = Mail::Part.new do
      content_type 'text/html; charset=UTF-8'
      body         "<img src=#{mail.attachments.inline['diagr.png'].url} /><br />\
                    <img src=#{mail.attachments.inline['graph.png'].url}>"
    end
    mail.html_part = html_part
    mail.deliver!
  end
end
