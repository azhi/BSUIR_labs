require_relative 'base'

module Utils
  module Processors
    class LinearContrast < Base
      def run!(gmin:, gmax:, min_intens_percentile:, max_intens_percentile:, grayscaled:)
        histogramm = image.histogramm(grayscaled)
        fmin = calc_fmin(histogramm, min_intens_percentile)
        fmax = calc_fmax(histogramm, max_intens_percentile)
        if ENV['DEBUG']
          $stderr.puts("Adjusting brightness from #{fmin.round}..#{fmax.round} to #{gmin.round}..#{gmax.round}")
        end
        image.image_data.map! do |r, g, b|
          if grayscaled
            o = (r - fmin) / (fmax - fmin).to_f * (gmax - gmin) + gmin
            o = o.round

            o = 0 if o < 0
            o = QRange if o > QRange

            [o, o, o]
          else
            r = (r - fmin) / (fmax - fmin) * (gmax - gmin) + gmin
            g = (g - fmin) / (fmax - fmin) * (gmax - gmin) + gmin
            b = (b - fmin) / (fmax - fmin) * (gmax - gmin) + gmin

            r = 0 if r < 0
            g = 0 if g < 0
            b = 0 if b < 0

            r = QRange if r > QRange
            g = QRange if g > QRange
            b = QRange if b > QRange

            [r, g, b]
          end
        end
      end

      def calc_fmin(histogramm, min_intens_percentile)
        min_intens_threshold = min_intens_percentile / 100.0
        sum = 0.0
        histogramm.find do |_brightness, intensity|
          sum += intensity
          sum > min_intens_threshold
        end.first
      end

      def calc_fmax(histogramm, max_intens_percentile)
        max_intens_threshold = (100 - max_intens_percentile) / 100.0
        sum = 0.0
        histogramm.reverse_each.find do |_brightness, intensity|
          sum += intensity
          sum > max_intens_threshold
        end.first
      end
    end
  end
end
