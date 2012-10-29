#! /usr/bin/env ruby

require 'gnuplot'
require 'pry'

def calc_d_sum point
  [ @d[0], @d[1] * point[0], @d[2] * point[1], @d[3] * point[0] * point[1] ].inject(0){ |res, el| res + el }
end

def adjust_d cl, point
  adj_c = [ 1, 4 * point[0], 4 * point[1], 16 * point[0] * point[1] ]
  4.times do |i|
    @d[i] += adj_c[i] * ( cl == 1 ? 1 : -1 )
  end
end

def plot points
  Gnuplot::open do |gp|
    Gnuplot::Plot.new(gp) do |plot|
      cl0 = points.find_all{ |point| point[0] == 1 }.map{ |point| point[1..-1] }.transpose
      cl1 = points.find_all{ |point| point[0] == 2 }.map{ |point| point[1..-1] }.transpose
      minx = (cl0[0] + cl1[0]).min; maxx = (cl0[0] + cl1[0]).max;
      miny = (cl0[1] + cl1[1]).min; maxy = (cl0[1] + cl1[1]).max;
      plot.xrange "[#{(minx > 0 ? 0.9 : 1.1) * minx}:#{(maxx > 0 ? 1.1 : 0.9) * maxx}]"
      plot.yrange "[#{(miny > 0 ? 0.9 : 1.1) * miny}:#{(maxy > 0 ? 1.1 : 0.9) * maxy}]"

      plot.data << Gnuplot::DataSet.new("(-(#{@d[0]}) - (#{@d[1]}*x)) / ((#{@d[2]}) + (#{@d[3]})*x)") do |ds|
        ds.with = 'lines'
        ds.notitle
      end

      plot.data << Gnuplot::DataSet.new(cl0) do |ds|
        ds.with = 'points'
        ds.linewidth = 3
        ds.notitle
      end

      plot.data << Gnuplot::DataSet.new(cl1) do |ds|
        ds.with = 'points'
        ds.linewidth = 3
        ds.notitle
      end
    end
  end
end

print "Enter points count: "
count = gets.chomp.to_i
puts "Enter #{count} points in format: <class> <x> <y>"
points = []
count.times do
  points << gets.chomp.split.map(&:to_f)
end

@d = Array.new(4){0}

valid = false
500.times do
  valid = points.inject(true) do |res, point|
    cl = point[0];
    point_coord = point[1..-1]
    if ( (cl-1).zero? ^ (calc_d_sum(point_coord) > 0) )
      adjust_d cl, point_coord
      false
    else
      res
    end
  end
  break if valid
end

unless valid
  puts "Can't build a proper potential function.\nShutting down..."
  exit
end

puts "d(x) = #{(@d[0] < 0 ? '- ' : '')+@d[0].abs.round(3).to_s} #{(@d[1] < 0 ? ?- : ?+)+" "+@d[1].abs.round(3).to_s}*x1 \
#{(@d[2] < 0 ? ?- : ?+)+" "+@d[2].abs.round(3).to_s}*x2 #{(@d[3] < 0 ? ?- : ?+)+" "+@d[3].abs.round(3).to_s}*x1*x2"

puts "Enter points in format <x> <y> to find their classes, q to quit or p to plot d-function"
while true do
  str = $stdin.gets.chomp
  if str[0] == 'q'
    exit
  elsif str[0] == 'p'
    plot points
  else
    ar = str.split.map(&:to_f)
    puts "Class: #{calc_d_sum(ar) > 0 ? 1 : 2}"
  end
end
