module Utils
  class Image
    attr_accessor :areas
    attr_accessor :areas_descriptors
    attr_reader :image

    def initialize(path, data = nil, width = nil, height = nil)
      if path
        @image = Magick::Image.read(path).first
      else
        @image = Magick::Image.new(width, height)
        @image.import_pixels(0, 0, width, height, "RGB", data.flatten, Magick::QuantumPixel)
      end
    end

    def pixel_count
      image.rows * image.columns
    end

    def width
      image.columns
    end

    def height
      image.rows
    end

    def out_of_bounds?(x, y)
      x < 0 || x >= height ||
        y < 0 || y >= width
    end

    def original_image_data
      image.export_pixels(0, 0, image.columns, image.rows, "RGB").each_slice(3)
    end

    def image_data
      @image_data ||= original_image_data.to_a
    end

    def image_data=(image_data)
      @image_data = image_data
    end

    def build_out_image
      @out_image = Magick::Image.new(image.columns, image.rows)
      @out_image.import_pixels(0, 0, image.columns, image.rows, "RGB", image_data.flatten, Magick::QuantumPixel)
      @out_image
    end

    def clone
      self.class.new(nil, image_data.flatten, width, height)
    end

    def image_diff(other)
      raise ArgumentError unless self.width == other.width && self.height == other.height
      diff = image_data.zip(other.image_data).map do |(y, _, _), (o_y, _, _)|
        y - o_y
      end
      diff
    end

    def histogramm(grayscaled = false)
      image_data.each.with_object(Hash.new(0)) do |(r, g, b), res|
        y = grayscaled ? r : (0.3 * r + 0.59 * g + 0.11 * b)
        res[y] += 1
      end.map do |k, v|
        [k, v / pixel_count.to_f]
      end.sort_by do |k, v|
        k
      end.to_h
    end

    def save(path)
      build_out_image.write(path)
    end

    def display
      build_out_image.display
    end
  end
end
