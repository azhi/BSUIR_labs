require_relative 'base'

module Utils
  module Processors
    class Areas < Base
      AREA_THRESHOLD = 20
      MAX_ELONGATION = 9999999.9

      def run!(silent: false, modify_image: false, area_colors: nil)
        areas, image_areas_binding = calc_areas
        areas_descriptors = calc_areas_descriptors(image, areas, image_areas_binding)
        areas, areas_descriptors = filter_areas(areas, areas_descriptors, silent)
        unless silent
          areas_descriptors.each.with_index do |area_descriptor, i|
            puts "Area ##{i + 1}: " +
              "center_of_mass=[#{area_descriptor[:center_of_mass].first.round(2)}, #{area_descriptor[:center_of_mass].last.round(2)}]".ljust(32, " ") +
              [:area, :perimeter, :density, :elongation].map{ |field| "#{field}=#{area_descriptor[field].round(2)}" }.join(" ")
          end
        end
        image.areas = areas
        image.areas_descriptors = areas_descriptors

        if modify_image
          area_colors ||= areas.size.times.map{ |i| [i + 1, i + 1, i + 1] }
          areas.each.with_index do |area, i|
            area.each do |x, y|
              image.image_data[x * image.width + y] = area_colors[i % area_colors.size]
            end
          end
        end
      end

      def calc_areas
        image_data = image.image_data
        areas = []
        areas_binding = Array.new(image_data.size, 0)
        call_stack = []
        (0...image.height).each do |x|
          (0...image.width).each do |y|
            if areas_binding[x * image.width + y].zero?
              call_stack.push([x, y, nil])
              process_call_stack(call_stack, image_data, areas, areas_binding)
            end
          end
        end
        $stderr.puts "Found #{areas.size} initial areas" if ENV['DEBUG']
        return areas, areas_binding
      end

      def process_call_stack(call_stack, image_data, areas, areas_binding)
        while call_stack.any?
          x, y, current_area = call_stack.pop
          index = x * image.width + y
          if areas_binding[index].zero? && image_data[index].first == QRange
            current_area ||= []
            current_area << [x, y]
            areas_binding[index] = areas.size + 1
            call_stack.push([x - 1, y, current_area]) if x > 0
            call_stack.push([x + 1, y, current_area]) if x < image.height - 1
            call_stack.push([x, y - 1, current_area]) if y > 0
            call_stack.push([x, y + 1, current_area]) if y < image.width - 1
          end
        end
        areas << current_area if current_area
      end

      def calc_areas_descriptors(image, areas, areas_binding)
        result = []
        areas.each.with_index do |area, i|
          area_descriptor = {
            area: get_area(area),
            perimeter: get_perimeter(area, i, image, areas_binding),
          }
          area_descriptor[:center_of_mass] = get_center_of_mass(area, area_descriptor[:area])
          area_descriptor[:density] = area_descriptor[:perimeter] ** 2 / area_descriptor[:area].to_f
          m20 = get_central_moment(area, 2, 0, area_descriptor[:center_of_mass])
          m02 = get_central_moment(area, 0, 2, area_descriptor[:center_of_mass])
          m11 = get_central_moment(area, 1, 1, area_descriptor[:center_of_mass])
          area_descriptor[:elongation] = (m20 + m02 + ((m20 - m02) ** 2 + 4 * m11 ** 2) ** 0.5) /
            (m20 + m02 - ((m20 - m02) ** 2 + 4 * m11 ** 2) ** 0.5)
          # protection from Infinity
          area_descriptor[:elongation] = MAX_ELONGATION if area_descriptor[:elongation] > MAX_ELONGATION
          result << area_descriptor
        end
        result
      end

      def get_area(area)
        area.size
      end

      def get_perimeter(area, area_index, image, areas_binding)
        area.inject(0) do |perimeter, (x, y)|
          edge = false
          edge ||= image.out_of_bounds?(x - 1, y) || areas_binding[(x - 1) * image.width + y] != area_index + 1
          edge ||= image.out_of_bounds?(x + 1, y) || areas_binding[(x + 1) * image.width + y] != area_index + 1
          edge ||= image.out_of_bounds?(x, y - 1) || areas_binding[x * image.width + y - 1] != area_index + 1
          edge ||= image.out_of_bounds?(x, y + 1) || areas_binding[x * image.width + y + 1] != area_index + 1
          edge ? perimeter + 1 : perimeter
        end
      end

      def get_center_of_mass(area, a)
        c_x = area.inject(0){ |sum, (x, _y)| sum + x } / a.to_f
        c_y = area.inject(0){ |sum, (_x, y)| sum + y } / a.to_f
        [c_x, c_y]
      end

      def get_central_moment(area, i, j, center_of_mass)
        area.inject(0) do |res, (x, y)|
          res + (x - center_of_mass.first) ** i * (y - center_of_mass.last) ** j
        end
      end

      def filter_areas(areas, areas_descriptors, silent = false)
        to_delete = []
        areas_descriptors.each.with_index do |area_descriptor, i|
          to_delete << i if area_descriptor[:area] < AREA_THRESHOLD
        end
        puts "Found #{to_delete.size} too small areas (#{to_delete.map{ |i| areas_descriptors[i][:area].round(2).to_s }.join(", ")})."
        areas = areas.each.with_index.map{ |area, i| area unless to_delete.include?(i) }.compact
        areas_descriptors = areas_descriptors.each.with_index.map{ |area_descriptor, i| area_descriptor unless to_delete.include?(i) }.compact
        return areas, areas_descriptors
      end
    end
  end
end
