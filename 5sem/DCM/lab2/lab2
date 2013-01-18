require 'mathn'

class Vector
  def []=(i,val)
    @elements[i] = val
  end
end

class Matrix
  def []=(i, j, val)
    @rows[i][j] = val
  end
end

class Point
  attr_accessor :x, :y

  def initialize x, y
    @x = x
    @y = y
  end

  def to_s
    "Point: #{@x} #{@y}"
  end
end

Shoes.app :title => "Interpolation" do
  def read_from_file path
    file = open path
    @points = Array.new
    @i = 0
    while ( !file.eof ) do
      str = file.gets.split(' ')
      @points[@i] = Point.new(str[0].to_r, str[1].to_r)
      @i += 1
    end
    @count = @i
    @h = @points[1].x - @points[0].x
    @in.replace @points.map{ |p| "(#{p.x}; #{p.y.to_f.round(2)})" }.join(', ')
  end

  def fact n
    if n == 0 || n == 1
      1
    else
      (2..n).inject(1){ |sum, el| sum * el }
    end
  end

  def get_y_lagrange x
    @coeffs.each_with_index.inject(0) do |sum, ar|
      el = ar[0]
      i = ar[1]
      subprod = (0...@points.size).inject(1){ |ss, j| ss * ( i == j ? 1 : (x - @points[j].x) ) }
      sum + el * subprod
    end
  end

  def calc_lagrange
    @coeffs = @points.each_with_index.map do |p, i|
      p.y / @points.each_with_index.inject(1) do |sum, el|
        x = el[0].x
        j = el[1]
        sum * ( i == j ? 1 : p.x - x )
      end
    end

    @plot_str = Array.new
    @plot_str[0] = {}
    @plot_str[0][:min_x] = @points.min_by{ |el| el.x }.x.to_f - 1
    @plot_str[0][:min_y] = @points.min_by{ |el| el.y }.y.to_f - 1
    @plot_str[0][:max_x] = @points.max_by{ |el| el.x }.x.to_f + 1
    @plot_str[0][:max_y] = @points.max_by{ |el| el.y }.y.to_f + 1
    @plot_str[0][:polynom] = @coeffs.each_with_index.inject("") do |str, ar|
      el = ar[0]
      i = ar[1]
      str + "+(#{el.to_f.to_s + (0...@points.size).inject(""){ |xstr, j| xstr + ( i == j ? "" : " * (x - #{@points[j].x})" ) }})"
    end

    calc_x = Array.new(@points.size - 1){ |i| i + 1.5 }
    res = calc_x.inject({}){ |hash, x| hash.merge( x => get_y_lagrange(x) ) }
    @out.replace res.map{ |k, v| "(#{k}; #{v.to_f.round(2)})" }.join(', ')
    plot
  end

  def get_y_newton x
    @coeffs.each_with_index.inject(0) do |sum, ar|
      el = ar[0]
      i = ar[1]
      subprod = (0..i-1).inject(1){ |ss, j| ss * (x - @points[j].x) }
      sum + el * subprod
    end
  end

  def calc_newton
    dsy = Array.new(@count)
    dsy[0] = @points.map{ |p| p.y }
    (1...@count).each do |i|
      dsy[i] = Array.new(@count - i)
      (0...@count-i).each do |j|
        dsy[i][j] = dsy[i-1][j+1] - dsy[i-1][j]
      end
    end

    @coeffs = dsy.each_with_index.map do |el, i|
      el[0] / fact(i) / @h ** i
    end

    @plot_str = Array.new
    @plot_str[0] = {}
    @plot_str[0][:min_x] = @points.min_by{ |el| el.x }.x.to_f - 1
    @plot_str[0][:min_y] = @points.min_by{ |el| el.y }.y.to_f - 1
    @plot_str[0][:max_x] = @points.max_by{ |el| el.x }.x.to_f + 1
    @plot_str[0][:max_y] = @points.max_by{ |el| el.y }.y.to_f + 1
    @plot_str[0][:polynom] = @coeffs.each_with_index.inject("") do |str, ar|
      el = ar[0]
      i = ar[1]
      str + "+(#{el.to_f.to_s + (0..i-1).inject(""){ |xstr, j| xstr + " * (x - #{@points[j].x})" }})"
    end

    calc_x = Array.new(@points.size - 1){ |i| i + 1.5 }
    res = calc_x.inject({}){ |hash, x| hash.merge( x => get_y_newton(x) ) }
    @out.replace res.map{ |k, v| "(#{k}; #{v.to_f.round(2)})" }.join(', ')
    plot
  end

  def get_y_spline x
    @index = 1
    (1...@points.size).each do |i|
      @index = i
      break if @points[i].x >= x && @points[i-1].x <= x
    end
    @coeffs[@index][:a] + @coeffs[@index][:b] * (x - @points[@index].x) + @coeffs[@index][:c] / 2 * (x - @points[@index].x) ** 2 + @coeffs[@index][:d] / 6 * (x - @points[@index].x) ** 3
  end

  def calc_splines
    @A = Matrix.zero(@points.size)
    @A[0, 0] = 1
    @A[@points.size-1, @points.size-1] = 1
    @B = Vector[* Array.new(@points.size){0} ]
    h = Array.new(@points.size)
    (1...@points.size).each{ |i| h[i] = @points[i].x - @points[i-1].x }
    (1...@points.size-1).each do |i|
      p0 = @points[i-1]
      p1 = @points[i]
      p2 = @points[i+1]
      hi = h[i]
      hi1 = h[i+1]
      @A[i, i-1] = hi
      @A[i, i] = 2 * (hi + hi1)
      @A[i, i+1] = hi1
      @B[i] = 6 * ( (p2.y - p1.y) / hi1 - (p1.y - p0.y) / hi )
    end

    @C = @A.inv * @B

    @coeffs = Array.new(@points.size)
    (1...@points.size).each do |i|
      a = @points[i].y
      c = @C[i]
      d = (@C[i] - @C[i-1]) / h[i]
      b = h[i] / 2 * @C[i] - h[i] ** 2 / 6 * d + (@points[i].y - @points[i-1].y) / h[i]
      @coeffs[i] = { :a => a, :b => b, :c => c, :d => d }
    end

    @plot_str = Array.new(@coeffs.size)
    @coeffs.each_with_index.each do |coeff, i|
      if i > 0
        @plot_str[i] = {}
        @plot_str[i][:min_x] = @points[i-1].x
        @plot_str[i][:max_x] = @points[i].x
        @plot_str[i][:polynom] = "( #{coeff[:a].to_f} + #{coeff[:b].to_f} * (x - #{@points[i].x}) + #{coeff[:c].to_f / 2} * (x - #{@points[i].x}) ** 2 + #{coeff[:d].to_f / 6} * (x - #{@points[i].x}) ** 3 )"
      else
        @plot_str[i] = {}
        @plot_str[i][:min_x] = 0
        @plot_str[i][:max_x] = 0
        @plot_str[i][:polynom] = "(1/0)"
      end
    end

    calc_x = Array.new(@points.size - 1){ |i| i + 1.5 }
    res = calc_x.inject({}){ |hash, x| hash.merge( x => get_y_spline(x) ) }
    @out.replace res.map{ |k, v| "(#{k}; #{v.to_f.round(2)})" }.join(', ')
    plot
  end

  def plot
    File.open("plot.gnu", 'w') do |plot_file|
      plot_file.puts "set title \"#{@last} interpolation\""
      plot_file.puts 'set grid'
      plot_file.puts 'set xtics 1'
      plot_file.puts 'set ytics 0.25'
      plot_file.puts "set xrange [#{@points.min_by{ |el| el.x }.x.to_f - 1}:#{@points.max_by{ |el| el.x }.x.to_f + 1}]"
      plot_file.puts "set yrange [#{@points.min_by{ |el| el.y }.y.to_f - 1}:#{@points.max_by{ |el| el.y }.y.to_f + 1}]"
      plot_file.puts "set samples 10000"
      plot_file.puts 'set style line 1 lt 1 lw 4'
      plot_file.puts 'set style line 2 lc rgb "#0000A0" lw 4'
      plot_file.puts 'set xlabel "x"'
      plot_file.puts 'set ylabel "y"'
      
      plot_file.puts @plot_str.inject("plot "){ |str, el| str + "(x<=#{el[:max_x]} && x>=#{el[:min_x]}) ? (#{el[:polynom]}) : " } + '(1/0) notitle with lines linestyle 1' +
                                                                ", \"#{@filename}\" using 1:2 notitle with points linestyle 2"
    end
    system "gnuplot -p plot.gnu"
  end

  background "#FFF".."#333", :angle => 60

  stack :margin => 10 do
    button "Read points from file", :margin => 5 do
      @filename = ask_open_file
      read_from_file @filename
    end

    flow do
      button "calculate Lagrange", :margin => 5 do
        @last = "Lagrange"
        calc_lagrange
      end

      button "calculate Newton", :margin => 5 do
        @last = "Newton"
        calc_newton
      end

      button "calculate splines", :margin => 5 do
        @last = "Splines"
        calc_splines
      end
    end

    button "Replot", :margin => 5 do
      plot
    end

    para "Source data:", :margin => 5
    @in = para :margin => 20
    para "Middle points:", :margin => 5
    @out = para :margin => 20

    flow :margin => 5 do
      inscription "X:"
      @inX = edit_line :width => 50
      inscription "Y:"
      @outY = edit_line :width => 200
      button "calculate Y" do
        case @last
        when "Lagrange"
          @outY.text = get_y_lagrange @inX.text.to_f
        when "Newton"
          @outY.text = get_y_newton @inX.text.to_f
        when "Splines"  
          @outY.text = get_y_spline @inX.text.to_f
        end
      end
    end
  end

end
