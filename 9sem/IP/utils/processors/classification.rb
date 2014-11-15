require_relative 'base'

module Utils
  module Processors
    class Classification < Base
      EPS = 0.0001

      def run!(object_colors:, attributes:, cluster_count:)
        areas_descriptors = image.areas_descriptors
        areas_descriptors = get_object_classes(areas_descriptors, attributes, cluster_count)
        image.areas_descriptors = areas_descriptors

        image.areas.each.with_index do |area, i|
          color = object_colors[areas_descriptors[i][:class_num]]
          area.each do |x, y|
            image.image_data[x * image.width + y] = color
          end
        end
      end

      def get_object_classes(areas_descriptors, attributes, cluster_count)
        areas = areas_descriptors.map{ |area_descriptor| area_descriptor.keep_if{ |k, v| attributes.include?(k) }.values }
        areas_binding = do_k_means(areas, cluster_count)
        areas_descriptors = areas_descriptors.each.with_index.map do |area_descriptor, i|
          area_descriptor[:class_num] = areas_binding[i]
          area_descriptor
        end
        areas_descriptors
      end

      def do_k_means(areas, cluster_count)
        means = areas.shuffle.first(cluster_count)
        attributes_count = means.first.size
        objects_binding = Array.new(areas.size, -1)
        done = false
        while !done
          old_means = means.dup

          areas.each.with_index do |area, i|
            _mean, class_num = means.each.with_index.min_by{ |mean, _i| calc_dist(area, mean) }
            objects_binding[i] = class_num
          end

          objects_binding.each.with_index.group_by{ |class_num, _i| class_num }.each do |class_num, indexes|
            sum = indexes.map(&:last).each.with_object(Array.new(attributes_count, 0)) do |i, sum|
              sum.map!.with_index do |s, attribute_index|
                s += areas[i][attribute_index]
              end
            end
            avg = sum.map{ |attribute| attribute / indexes.size.to_f }
            means[class_num] = avg
          end

          done = (0...means.size).map{ |i| calc_dist(means[i], old_means[i]) }.all?{ |dist| dist < EPS }
        end
        objects_binding
      end

      def calc_dist(o1, o2)
        (0...o1.size).inject(0) do |dist, i|
          dist + (o1[i] - o2[i]) ** 2
        end
      end
    end
  end
end
