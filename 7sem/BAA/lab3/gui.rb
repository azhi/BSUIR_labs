require 'green_shoes'
require 'gnuplot'
require 'socket'
require 'pry'
require File.join(File.dirname(__FILE__), 'plotter.rb')

SHOW_TIME_INTERVAL = 40

Shoes.app :title => "BAA LAB #3", :height => 630 do
  def get_new_prices
    @socket ||= TCPSocket.open('127.0.0.1', 3333)
    @socket.puts "GIFF ME DATA"
    data = @socket.gets
    data.chomp.split(?,).map(&:to_f)
  end

  def update_interval(interval)
    @timer.stop if @timer
    @current_interval = interval
    @timer = every(interval, &@update_proc)
  end

  @update_proc = Proc.new do
    @time_offset ||= 0
    @current_time ||= 0
    @current_time += @current_interval
    if @current_time > SHOW_TIME_INTERVAL
      @time_offset += @current_interval
      @prices = @prices.drop_while{ |pr| pr.first < @current_time - SHOW_TIME_INTERVAL }
    end
    @prices << [@current_time, get_new_prices]
    outfile = File.expand_path("../tmp/graph.svg", __FILE__)
    Plotter.update_prices_graph(@prices, @time_offset, SHOW_TIME_INTERVAL, outfile)
    @img.clear
    @img.append{ image File.read(outfile), :margin => 20 }
  end
  @prices = []

  background "#eee"

  stack :margin => 5 do
    inscription 'Update interval: ' ; @interval = edit_line width: 200; @interval.text = '1'
    button 'Set' do
      update_interval(@interval.text.to_i)
    end
    @img = flow{ image File.expand_path("../tmp/empty.png", __FILE__), :margin => 20 }
  end

  update_interval(1)
end
