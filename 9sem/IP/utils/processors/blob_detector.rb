require 'set'

require_relative 'base'
require_relative 'gaussian'
require_relative 'grayscale'

module Utils
  module Processors
    class BlobDetector < Base
      OCTAVE_COUNT = 3
      GAUSSIAN_DIFFERENCE_CONSTANT = 1.6
      INITIAL_SIGMA = Math.exp(1) * GAUSSIAN_DIFFERENCE_CONSTANT
      MAX_SIGMA = 60.0

      AREA_INTERSECTION_THRESHOLD = 0.40

      BLOB_COLOR = [QRange, 0, 0]

      def run!(dog_brightness_threshold:, enable_area_filter:)
        grayscaled_image = image.clone
        Grayscale.new(grayscaled_image).run!
        octaves = build_gaussians(grayscaled_image)
        dogs = get_dogs(octaves)
        blobs = get_blobs(dogs, dog_brightness_threshold)
        puts "Blobs count before area filter: #{blobs.count}" if ENV['DEBUG']
        blobs = filter_blobs(blobs) if enable_area_filter
        puts "Blobs count after area filter: #{blobs.count}" if ENV['DEBUG']
        blobs.each do |(x, y, r, _)|
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
          while sigma < MAX_SIGMA
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

      def get_blobs(dogs, dog_brightness_threshold)
        blobs = Set.new

        min_of_max = Float::INFINITY
        max_of_max = -Float::INFINITY

        (0...image.height).each do |x|
          (0...image.width).each do |y|
            dogs.each_cons(3) do |cons_dogs|
              sigma = cons_dogs[1].first
              brightness = cons_dogs[1].last[x * image.width + y].abs
              if local_extremum?(x, y, sigma, brightness, cons_dogs)
                blobs << [x, y, sigma * 2 ** 0.5, brightness] if brightness > dog_brightness_threshold
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

      def filter_blobs(blobs)
        blobs = blobs.to_a
        keep_ids = (0...blobs.size).to_a
        blobs.each.with_index.to_a.combination(2).each do |(blob1, i1), (blob2, i2)|
          if has_large_intersection?(blob1, blob2)
            if blob2.last > blob1.last && blob2[3] > blob1[3]
              keep_ids.delete(i1)
            elsif blob1.last > blob2.last && blob1[3] > blob2[3]
              keep_ids.delete(i2)
            end
          end
        end
        keep_ids.map{ |id| blobs[id] }
      end

      def has_large_intersection?(blob1, blob2)
        b1_x, b1_y, b1_r, _ = blob1
        b2_x, b2_y, b2_r, _ = blob2
        b1_box = [
          (keep_in_bound(:x, b1_x - b1_r)..keep_in_bound(:x, b1_x + b1_r)),
          (keep_in_bound(:y, b1_y - b1_r)..keep_in_bound(:y, b1_y + b1_r))
        ]
        b2_box = [
          (keep_in_bound(:x, b2_x - b2_r)..keep_in_bound(:x, b2_x + b2_r)),
          (keep_in_bound(:y, b2_y - b2_r)..keep_in_bound(:y, b2_y + b2_r))
        ]
        box_intersection = get_box_intersection(b1_box, b2_box)

        b1_area = (b1_box[0].end - b1_box[0].begin).abs * (b1_box[1].end - b1_box[1].begin).abs
        b2_area = (b2_box[0].end - b2_box[0].begin).abs * (b2_box[1].end - b2_box[1].begin).abs
        intersection_area = (box_intersection[0].end - box_intersection[0].begin).abs *
                            (box_intersection[1].end - box_intersection[1].begin).abs
        intersection_area / b1_area.to_f > AREA_INTERSECTION_THRESHOLD ||
          intersection_area / b2_area.to_f > AREA_INTERSECTION_THRESHOLD
      end

      def keep_in_bound(dimension, value)
        if dimension == :x
          value > 0 ?
            (value < image.width ? value : image.width) :
            0
        elsif dimension == :y
          value > 0 ?
            (value < image.height ? value : image.height) :
            0
        end
      end

      def get_box_intersection(b1, b2)
        intersect_x_range = [0, 0]
        intersect_y_range = [0, 0]
        if (b1.first.cover?(b2.first.begin) || b2.first.cover?(b1.first.begin)) &&
           (b1.last.cover?(b2.last.begin) || b2.last.cover?(b1.last.begin))
          if b1.first.cover?(b2.first.begin)
            intersect_x_range[0] = b2.first.begin
            intersect_x_range[1] = b1.first.cover?(b2.first.end) ? b2.first.end : b1.first.end
          elsif b2.first.cover?(b1.first.begin)
            intersect_x_range[0] = b1.first.begin
            intersect_x_range[1] = b2.first.cover?(b1.first.end) ? b1.first.end : b2.first.end
          end

          if b1.last.cover?(b2.last.begin)
            intersect_y_range[0] = b2.last.begin
            intersect_y_range[1] = b1.last.cover?(b2.last.end) ? b2.last.end : b1.last.end
          elsif b2.last.cover?(b1.last.begin)
            intersect_y_range[0] = b1.last.begin
            intersect_y_range[1] = b2.last.cover?(b1.last.end) ? b1.last.end : b2.last.end
          end
        end
        [(intersect_x_range.first..intersect_x_range.last), (intersect_y_range.first..intersect_y_range.last)]
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
