module Utils
  module Processors
    class Base
      QRange = ::Magick::QuantumRange

      attr_reader :image

      def initialize(image)
        @image = image
      end
    end
  end
end
