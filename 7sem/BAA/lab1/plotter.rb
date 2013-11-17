class Plotter
  attr_reader :initial_amount_sum, :table_data, :dates, :counts, :amounts, :amounts_sum

  def initialize(initial_amount_sum, table_data)
    @initial_amount_sum = initial_amount_sum
    @table_data = table_data
    @dates = table_data.map{ |row| row['date'] }
    @counts = table_data.map{ |row| row['deal_count'] }
    @amounts = table_data.map{ |row| row['amount'] }
    @amounts_sum = amounts.each.with_object({:sum => initial_amount_sum}).map{ |el, h| h[:sum] += el; h[:sum] }
  end

  def profit_sum_graph(outfile)
    max_sum = amounts_sum.max || 1
    max_sum_order = Math::log(max_sum, 10).ceil

    amount_scale_coeff = max_sum_order > 3 ? max_sum_order - 3 : 0
    amounts_sum_scaled = amounts_sum.map{ |amount| amount / 10 ** amount_scale_coeff }

    Gnuplot.open(true) do |gp|
      Gnuplot::Plot.new( gp ) do |plot|
        plot.terminal "svg size 550,270"
        plot.output outfile

        plot.xlabel "date"
        plot.timefmt '"%Y-%m-%dT%H:00:00+00:00"'
        plot.xdata 'time'
        plot.xrange "['#{dates.first.to_datetime - 16 / 24.0}':'#{dates.last.to_datetime}']"
        plot.ylabel "amount x10^#{amount_scale_coeff}"
        plot.format 'x "%d.%m.%Y"'
        plot.xtic 'rotate by -60 font ",6"'
        plot.grid
        plot.key 'outside center tmargin'

        plot.data << Gnuplot::DataSet.new([dates.map{ |d| d.to_datetime.to_s }, amounts_sum_scaled]) do |ds|
          ds.with = "lines"
          ds.using = '1:2'
          ds.title = 'Total amount'
        end
      end
    end
    self
  end

  def profit_diagramm(outfile)
    max_sum = amounts_sum.max || 1
    max_sum_order = Math::log(max_sum, 10).ceil

    amount_scale_coeff = max_sum_order > 3 ? max_sum_order - 3 : 0
    amounts_scaled = amounts.map{ |amount| amount / 10 ** amount_scale_coeff }

    Gnuplot.open(true) do |gp|
      Gnuplot::Plot.new( gp ) do |plot|
        plot.terminal "svg size 550,270"
        plot.output outfile

        plot.xlabel "date"
        plot.timefmt '"%Y-%m-%dT%H:00:00+00:00"'
        plot.xdata 'time'
        plot.xrange "['#{dates.first.to_datetime - 16 / 24.0}':'#{dates.last.to_datetime}']"
        plot.ylabel "count, amount x10^#{amount_scale_coeff}"
        plot.format 'x "%d.%m.%Y"'
        plot.xtic 'rotate by -60 font ",6"'
        plot.grid
        plot.key 'outside center tmargin'

        # amounts histogramm
        plot.boxwidth 3600 * 6
        plot.style 'fill solid'
        plot.data << Gnuplot::DataSet.new([dates.map{ |d| (d.to_datetime + 3 / 24.0).to_s }, amounts_scaled]) do |ds|
          ds.with = 'boxes lc rgb "blue"'
          ds.using = "1:2"
          ds.title = 'Amount'
        end
        plot.data << Gnuplot::DataSet.new([dates.map{ |d| (d.to_datetime - 3 / 24.0).to_s },
                                           counts]) do |ds|
          ds.with = 'boxes lc rgb "green"'
          ds.using = "1:2"
          ds.title = 'Deal count'
        end
        plot.data << Gnuplot::DataSet.new([dates.map{ |d| (d.to_datetime - 3 / 24.0).to_s },
                                           counts,
                                           counts]) do |ds|
          ds.with = 'labels offset character 0, character 0.5 font ",6"'
          ds.using = "1:2:3"
          ds.notitle
        end

      end
    end
    self
  end
end
