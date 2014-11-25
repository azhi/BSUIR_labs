require 'set'

require_relative 'base'
require_relative 'gaussian'
require_relative 'grayscale'

module Utils
  module Processors
    class BlobDetector < Base
      OCTAVE_SCALES = [6, 4, 3]
      OCTAVE_COUNT = 3
      GAUSSIAN_DIFFERENCE_CONSTANT = 1.6
      INITIAL_SIGMA = Math.exp(1) * GAUSSIAN_DIFFERENCE_CONSTANT

      BRIGHTNESS_THRESHOLD = 10000
      BLOB_COLOR = [QRange, 0.0, 0.0]

      def run!
        grayscaled_image = image.clone
        Grayscale.new(grayscaled_image).run!
        octaves = build_gaussians(grayscaled_image)
        dogs = get_dogs(octaves)
        blobs = get_blobs(dogs)
        puts "Blobs count: #{blobs.count}" if ENV['DEBUG']
        blobs.each do |(x, y, sigma)|
          r = 2 ** 0.5 * sigma
          r = r.round
          # p [x, y, r]
          draw_circle(x, y, r)
        end
      end

      def build_gaussians(grayscaled_image)
        octaves = []
        OCTAVE_COUNT.times do |octave|
          puts "-" * 15 + " #{octave + 1} octave " + "-" * 15 if ENV['DEBUG']
          sigma = INITIAL_SIGMA * (octave + 1)
          images = {}
          (OCTAVE_SCALES[octave] + 1).times do
            puts "Sigma: #{sigma.round(2)}" if ENV['DEBUG']
            current_image = images[sigma.round(2)] = grayscaled_image.clone
            Gaussian.new(current_image).run!(sigma: sigma)
            current_image.save(File.join(File.dirname(__FILE__), "../../images/blur/#{sigma.round(2)}.png")) if ENV['DEBUG']
            sigma *= GAUSSIAN_DIFFERENCE_CONSTANT
          end
          octaves << images
        end
        octaves
      end

      def get_dogs(octaves)
        res = {}
        octaves.each do |gaussians|
          octave_dogs = gaussians.each_cons(2).map do |(sigma1, image1), (_sigma2, image2)|
            diff = image1.image_diff(image2)

            if ENV['DEBUG']
              image_data = diff.map{ |e| e < 0 ? [-e, 0, -e] : [0, e, e] }
              current_image = Magick::Image.new(image.width, image.height)
              current_image.import_pixels(0, 0, image.width, image.height, "RGB", image_data.flatten, Magick::QuantumPixel)
              current_image.write(File.join(File.dirname(__FILE__), "../../images/diff/#{sigma1.round(2)}.png"))
            end

            [sigma1, diff]
          end.to_h
          res = res.merge octave_dogs
        end
        res = res.sort_by{ |sigma, _diff| sigma }.to_h
        res
      end

      def get_blobs(dogs)
        blobs = Set.new

        min_of_max = Float::INFINITY
        max_of_max = -Float::INFINITY

        (0...image.height).each do |x|
          (0...image.width).each do |y|
            dogs.each_cons(3) do |cons_dogs|
              sigma = cons_dogs[1].first
              brightness = cons_dogs[1].last[x * image.width + y].abs
              if local_extremum?(x, y, sigma, brightness, cons_dogs)
                blobs << [x, y, sigma] if brightness > BRIGHTNESS_THRESHOLD
                if ENV['DEBUG']
                  min_of_max = brightness if brightness < min_of_max
                  max_of_max = brightness if brightness > max_of_max
                end
              end
            end
          end
        end

        p [min_of_max, max_of_max] if ENV['DEBUG']
        blobs
      end

      def local_extremum?(x, y, sigma, brightness, cons_dogs)
        max_brightness = brightness
        found = false
        catch :out do
          (-1..1).each do |offset_x|
            (-1..1).each do |offset_y|
              nx = x + offset_x
              ny = y + offset_y
              unless image.out_of_bounds?(nx, ny)
                sigma, brightness = cons_dogs.map do |sigma, diff|
                  [sigma, diff[nx * image.width + ny].abs]
                end.max_by do |sigma, brightness|
                  brightness
                end
                if brightness > max_brightness
                  found = true
                  throw :out
                end
              end
            end
          end
        end
        return !found
      end

      def draw_circle(x0, y0, r)
        x = r;
        y = 0;
        radiusError = 1 - x;
        while x >= y
          put_pixel(x + x0, y + y0, BLOB_COLOR)
          put_pixel(y + x0, x + y0, BLOB_COLOR)
          put_pixel(-x + x0, y + y0, BLOB_COLOR)
          put_pixel(-y + x0, x + y0, BLOB_COLOR)
          put_pixel(-x + x0, -y + y0, BLOB_COLOR)
          put_pixel(-y + x0, -x + y0, BLOB_COLOR)
          put_pixel(x + x0, -y + y0, BLOB_COLOR)
          put_pixel(y + x0, -x + y0, BLOB_COLOR)
          y += 1;
          if radiusError < 0
            radiusError += 2 * y + 1
          else
            x -= 1;
            radiusError += 2 * (y - x + 1)
          end
        end
      end

      def put_pixel(x, y, color)
        image.image_data[x * image.width + y] = color unless image.out_of_bounds?(x, y)
      end
    end
  end
end
