require 'gnuplot'
require 'pry'

class Plotter
  def self.plot_for_law1(law_freqs)
    Gnuplot.open(true) do |gp|
      Gnuplot::Plot.new( gp ) do |plot|
        plot.xlabel "rank"
        plot.ylabel "freq"
        plot.grid
        plot.xtic 'rotate by -60 font ",8"'
        plot.xtics ?( + law_freqs.map.with_index{ |law_freq, i| ?" + law_freq[:word].to_s + ?" + ' ' + i.to_s }.join(?,) + ?)

        plot.data << Gnuplot::DataSet.new([(1..law_freqs.size).to_a,
                                           law_freqs.map{ |law_freq| law_freq[:freq] }]) do |ds|
          ds.with = "lines"
          ds.using = '1:2'
          ds.notitle
        end
      end
    end
  end

  def self.plot_for_law2(freq_word_counts)
    Gnuplot.open(true) do |gp|
      Gnuplot::Plot.new( gp ) do |plot|
        plot.xlabel "freq"
        plot.ylabel "word count"
        plot.grid

        plot.data << Gnuplot::DataSet.new([freq_word_counts.keys,
                                           freq_word_counts.values]) do |ds|
          ds.with = "lines"
          ds.using = '1:2'
          ds.notitle
        end
      end
    end
  end
end
