require_relative 'base'

module Utils
  module Processors
    class LowfreqFilter < Base
      PRIMITIVES = [
        [[1, 2, 1],
         [2, 4, 2],
         [1, 2, 1]],

        [[1, 1, 1],
         [1, 2, 1],
         [1, 1, 1]],

        [[1, 1, 1],
         [1, 1, 1],
         [1, 1, 1]]
      ]
      COEFFS = [1 / 16.0, 1 / 10.0, 1 / 9.0]

      def run!(primitive_id: 0)
        image_data = image.image_data
        out_image_data = image_data.dup
        (0...image.height).each do |x|
          (0...image.width).each do |y|
            apply_primitive(primitive_id, image_data, out_image_data, x, y)
          end
        end
        image.image_data = out_image_data
      end

      def calc_effective_primitive(primitive_id, x, y)
        effective_primitive = PRIMITIVES[primitive_id].dup
        (-1..1).each do |offset_x|
          (-1..1).each do |offset_y|
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

      def apply_primitive(primitive_id, in_image_data, out_image_data, x, y)
        effective_primitive = calc_effective_primitive(primitive_id, x, y)

        sums = [0.0, 0.0, 0.0]
        (-1..1).each do |offset_x|
          (-1..1).each do |offset_y|
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
        sums = sums.map do |sum|
          (COEFFS[primitive_id] * sum).round
        end
        out_image_data[x * image.width + y] = sums
      end
    end
  end
end
