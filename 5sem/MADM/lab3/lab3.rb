#! /usr/bin/env ruby

require 'gnuplot'
require 'mathn'

include Math

def get_f1 x
  1 / @sigma1.to_f / (2 * PI) ** 0.5 * exp( -1 / 2.0 / @sigma1.to_f ** 2 * (x - @mu1) ** 2.0)
end

def get_f2 x
  1 / @sigma2.to_f / (2 * PI) ** 0.5 * exp( -1 / 2.0 / @sigma2.to_f ** 2 * (x - @mu2) ** 2.0)
end

def calc_intersect_point
  x1 = ( @sigma1 ** 2 * @mu2 - ( - 2 * @sigma1 ** 4 * @sigma2 ** 2 * log(@sigma2/@sigma1)  +
                                 + @sigma1 ** 2 * @mu1 ** 2 * @sigma2 ** 2 +
                                 - 2 * @sigma1 ** 2 * @mu1 * @sigma2 ** 2 * @mu2 +
                                 + 2 * @sigma1 ** 2 * @sigma2 ** 4 * log(@sigma2/@sigma1) +
                                 + @sigma1 ** 2 * @sigma2 ** 2 * @mu2 ** 2) ** 0.5 +
                                 - @mu1 * @sigma2 ** 2 ) / (@sigma1 ** 2 - @sigma2 ** 2)
  x2 = ( @sigma1 ** 2 * @mu2 + ( - 2 * @sigma1 ** 4 * @sigma2 ** 2 * log(@sigma2/@sigma1)  +
                                 + @sigma1 ** 2 * @mu1 ** 2 * @sigma2 ** 2 +
                                 - 2 * @sigma1 ** 2 * @mu1 * @sigma2 ** 2 * @mu2 +
                                 + 2 * @sigma1 ** 2 * @sigma2 ** 4 * log(@sigma2/@sigma1) +
                                 + @sigma1 ** 2 * @sigma2 ** 2 * @mu2 ** 2) ** 0.5 +
                                 - @mu1 * @sigma2 ** 2 ) / (@sigma1 ** 2 - @sigma2 ** 2)
  return x1, x2
end

params = Hash[ *ARGV.join.split(?,).map{ |arg| [arg.split(?=)[0], arg.split(?=)[1]] }.flatten ]
@mu1 = params["mu1"].to_i; @sigma1 = params["sigma1"].to_i;
@mu2 = params["mu2"].to_i; @sigma2 = params["sigma2"].to_i;

x1, x2 = calc_intersect_point
intersect_x = [x1, x2].min
puts "Intersect point: (#{intersect_x.round(6)}, #{get_f1(intersect_x).round(6)})"
f1_integr = ( 1 + erf( (intersect_x - @mu1) / (@sigma1 * 2 ** 0.5) ) ) / 2
f2_integr = ( 1 + erf( (intersect_x - @mu2) / (@sigma2 * 2 ** 0.5) ) ) / 2
puts "Error probability: #{(f1_integr / f2_integr * 100).round(2)}%"

Gnuplot::open do |gp|
  Gnuplot::Plot.new(gp) do |plot|
    maxy = [get_f1(@mu1), get_f2(@mu2)].max
    precis = maxy / 1000.0
    plot.yrange "[0:#{1.1 * maxy}]"
    minx = @mu1
    while ( get_f1(minx) > precis|| get_f2(minx) > precis ) do
      minx -= @mu1.abs / 10.0
    end
    maxx = @mu1
    while ( get_f1(maxx) > precis || get_f2(maxx) > precis ) do
      maxx += @mu1.abs / 10.0
    end
    plot.xrange "[#{minx}:#{maxx}]"

    plot.data << Gnuplot::DataSet.new("(#{(1 / @sigma1.to_f / (2 * PI) ** 0.5).to_f}) * exp( (#{(-1 / 2.0 / @sigma1.to_f ** 2).to_f}) * (x - (#{@mu1.to_f})) ** 2.0)") do |ds|
      ds.with = 'lines'
      ds.title = 'f1'
    end
    plot.data << Gnuplot::DataSet.new("(#{(1 / @sigma2.to_f / (2 * PI) ** 0.5).to_f}) * exp( (#{(-1 / 2.0 / @sigma2.to_f ** 2).to_f}) * (x - (#{@mu2.to_f})) ** 2.0)") do |ds|
      ds.with = 'lines'
      ds.title = 'f2'
    end
  end
end
