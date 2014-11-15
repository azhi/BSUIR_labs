require_relative 'base'

module Utils
  module Processors
    class ColorShift < Base
      def run!(options)
        %i[r g b].each{ |k| options[k] ||= 0 }
        image.image_data.map! do |r, g, b|
          r += options[:r]
          g += options[:g]
          b += options[:b]

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
  end
end
