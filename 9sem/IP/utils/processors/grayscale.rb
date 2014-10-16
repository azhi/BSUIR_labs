require_relative 'base'

module Utils
  module Processors
    class Grayscale < Base
      def run!(options = {})
        image.image_data = image.image_data.map do |r, g, b|
          y = 0.3 * r + 0.59 * g + 0.11 * b

          [y, y, y]
        end
      end
    end
  end
end
