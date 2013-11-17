class Plotter
  def self.price_distribution_diagramm(table_data, outfile)
    table_data = table_data.group_by{ |row| row['price'] }

    table_data = Hash[* table_data.map do |price, data|
      [price,
       [data.first['buy_sell'] * data.first['s_amount'], data.last['buy_sell'] * data.last['s_amount']]]
    end.reduce(:+) ]

    Gnuplot.open(true) do |gp|
      Gnuplot::Plot.new( gp ) do |plot|
        plot.terminal "svg size 550,350"
        plot.output outfile

        plot.xlabel "price"
        plot.ylabel "amount"
        plot.format 'y "%.0s*10^%T"'
        plot.grid
        plot.nokey
        plot.style 'fill solid'
        plot.boxwidth '0.7 relative'

        plot.data << Gnuplot::DataSet.new([table_data.keys, table_data.values.map(&:first)]) do |ds|
          ds.with = "boxes lc rgb 'brown'"
          ds.notitle
        end

        plot.data << Gnuplot::DataSet.new([table_data.keys, table_data.values.map(&:last)]) do |ds|
          ds.with = "boxes lc rgb 'blue'"
          ds.notitle
        end

        # plot.data << Gnuplot::DataSet.new([]) do |ds|
        #   ds.with = 'labels offset character 0, character 0.5 font ",6"'
        #   ds.using = "1:2:3"
        #   ds.notitle
        # end

      end
    end
  end
end
