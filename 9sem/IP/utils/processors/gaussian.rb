require_relative 'base'
require_relative 'gaussian/gaussian_ext'

module Utils
  module Processors
    class Gaussian < Base
      C = 1

      def run!(sigma:)
        image_data = image.image_data
        m = C * sigma + 1
        m = m.round
        primitive = generate_primitive(sigma, m)
        out_image_data = GaussianExt.do_gauss(m, primitive, image_data, image.height, image.width)
        image.image_data = out_image_data
      end

      def generate_primitive(sigma, m)
        @memoized_primitives ||= {}
        @memoized_primitives[sigma.round(2)] ||= begin
          one_d_primitive = generate_1d_primitive(sigma, m)
          two_d_primitive = one_d_primitive.product(one_d_primitive).map{ |ar| ar.reduce(:*) }.each_slice(one_d_primitive.size).to_a
          two_d_primitive.flatten
        end
      end

      def generate_1d_primitive(sigma, m)
        primitive = (-m..m).to_a.map do |n|
          Math.exp(-1 * n ** 2 / 2 / sigma ** 2) / sigma / Math.sqrt(2 * Math::PI)
        end
        # primitive.map{ |e| e / primitive.max }
      end

      def calc_effective_primitive(primitive, m, x, y)
        effective_primitive = primitive.dup
        (-m..m).each do |offset_x|
          (-m..m).each do |offset_y|
            nx = x + offset_x
            ny = y + offset_y
            if image.out_of_bounds?(nx, ny)
              value = effective_primitive[offset_y + 1][offset_x + 1]
              effective_primitive[offset_y + 1][offset_x + 1] = 0
              alive_count = effective_primitive.flatten.count{ |e| !e.zero? }
              addition = value / alive_count.to_f
              effective_primitive.map! do |row|
                row.map do |el|
                  el.zero? ? el : (el + addition).to_f
                end
              end
            end
          end
        end
        effective_primitive
      end

      def apply_primitive(primitive, m, in_image_data, out_image_data, x, y)
        effective_primitive = calc_effective_primitive(primitive, m, x, y)

        sums = [0.0, 0.0, 0.0]
        (-m..m).each do |offset_x|
          (-m..m).each do |offset_y|
            nx = x + offset_x
            ny = y + offset_y
            if image.out_of_bounds?(nx, ny)
              pixel = [0, 0, 0]
            else
              pixel = in_image_data[nx * image.width + ny]
            end
            (0..2).each do |c|
              sums[c] += effective_primitive[offset_y + 1][offset_x + 1] * pixel[c]
            end
          end
        end
        total = effective_primitive.inject(0){ |s, row| s + row.inject(0){ |s, e| s + e } }
        sums = sums.map do |sum|
          (sum / total.to_f).round
        end
        out_image_data[x * image.width + y] = sums
      end
    end
  end
end
