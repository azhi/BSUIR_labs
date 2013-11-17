class Plotter
  def self.update_prices_graph(prices, time_offset, time_range, outfile)
    split_prices = (0...prices.first.last.size).map do |price_index|
      prices.map{ |a_pr| [a_pr.first, a_pr.last[price_index]] }
    end

    Gnuplot.open(true) do |gp|
      Gnuplot::Plot.new( gp ) do |plot|
        plot.terminal "svg size 550,350"
        plot.output outfile

        plot.xlabel "time"
        plot.ylabel "price"
        plot.xrange "[#{time_offset}:#{time_offset + time_range}]"
        plot.grid
        plot.nokey

        split_prices.each do |price_series|
          plot.data << Gnuplot::DataSet.new([price_series.map(&:first), price_series.map(&:last)]) do |ds|
            ds.with = "lines"
            ds.using = '1:2'
          end
        end
      end
    end
  end
end
