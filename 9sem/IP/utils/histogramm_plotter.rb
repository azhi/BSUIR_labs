require 'gnuplot'

class HistogrammPlotter
  MAX_SAMPLES = 1_000

  def self.plot(histogramm, title)
    sorted_histogramm = histogramm.sort_by{ |k, _v| k }
    if sorted_histogramm.size > MAX_SAMPLES
      slice_size = (sorted_histogramm.size / MAX_SAMPLES).to_i
      sorted_histogramm = sorted_histogramm.each_slice(slice_size).with_object([]).each do |slice, res|
        key = (slice.first.first + slice.last.first) / 2.0
        sum_value = slice.inject(0){ |sum, (_k, v)| sum + v }
        res << [key, sum_value]
      end
      sorted_histogramm = sorted_histogramm.sort_by{ |(k, _v)| k }
    end
    Gnuplot.open(true) do |gp|
      Gnuplot::Plot.new( gp ) do |plot|
        plot.title title
        plot.xlabel "brightness"
        plot.ylabel "freq"

        plot.xrange "['0':'#{Magick::QuantumRange}']"
        # p high_y_threshold = sorted_histogramm.map(&:last).sort[-sorted_histogramm.size / 10000]
        # plot.yrange "['0':'#{high_y_threshold}']"

        plot.grid
        plot.style 'fill solid 1.0'

        plot.data << Gnuplot::DataSet.new([sorted_histogramm.map(&:first), sorted_histogramm.map(&:last)]) do |ds|
          ds.with = 'boxes lc rgb "#483D8B"'
          ds.using = "1:2"
          ds.notitle
        end
      end
    end
  end
end
