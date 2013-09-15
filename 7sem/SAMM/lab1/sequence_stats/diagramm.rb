require 'mathn'
require 'tempfile'

module SequenceStats
  class Diagramm
    attr_accessor :sequence

    def initialize(sequence, k)
      raise ArgumentError, 'must be kind of enumerable' unless sequence.kind_of?(Enumerable)
      @sequence = sequence
      @k = k

      @min = sequence.min
      @max = sequence.max
      @length = @max - @min
      @step = @length / k
    end

    def gnuplot_diagram
      tfile = Tempfile.new('gnuplot_SAMM_l1')
      file_str = diagramm_data.map{ |el| "\"#{el[:s]}-#{el[:e]}\" #{el[:d]}" }.join("\n")
      tfile.print(file_str)
      tfile.close
      hist_script_path = File.join(File.dirname(__FILE__), 'hist.gnu')
      system %Q{
        gnuplot -p -e 'load \"#{hist_script_path}\";
        stats \"#{tfile.path}\" using 2 nooutput;
        set yrange [(0.9 * STATS_min):(1.1 * STATS_max)];
        plot \"#{tfile.path}\" using 2:xtic(1) notitle with boxes ls 1'
      }
      tfile.unlink
    end

    private
      def diagramm_data
        @diagramm_data ||=
          (@min..@max).step(@step).to_a[0...-1].map do |start_x|
            end_x = start_x + @step
            n_count = @sequence.count{ |el| el.between?(start_x, end_x) }
            {s: start_x.to_f.round(2), e: end_x.to_f.round(2), d: (n_count / @sequence.size).to_f}
          end
      end
  end
end
