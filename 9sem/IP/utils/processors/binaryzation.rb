require_relative 'base'

module Utils
  module Processors
    class Binaryzation < Base
      def run!(brightness_threshold:)
        image.image_data.map! do |r, _, _|
          if r > brightness_threshold
            [QRange, QRange, QRange]
          else
            [0, 0, 0]
          end
        end
      end
    end
  end
end
