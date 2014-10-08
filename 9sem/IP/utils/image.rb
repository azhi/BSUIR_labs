module Utils
  class Image
    attr_reader :image
    attr_writer :image_data

    def initialize(path)
      @image = Magick::Image.read(path).first
    end

    def original_image_data
      image.export_pixels(0, 0, image.columns, image.rows, "RGB").each_slice(3)
    end

    def image_data
      @image_data ||= original_image_data
    end

    def build_out_image
      out_image = Magick::Image.new(image.columns, image.rows)
      out_image.import_pixels(0, 0, image.columns, image.rows, "RGB", image_data.flatten, Magick::QuantumPixel)
      out_image
    end

    def save(path)
      build_out_image.write(path)
    end

    def display
      build_out_image.display
    end
  end
end
