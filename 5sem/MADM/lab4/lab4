#! /usr/bin/env ruby

require 'mathn'

class Vector
  def []=(i,val)
    @elements[i] = val
  end
end

class Matrix
  def []=(i, j, val)
    @rows[i][j] = val
  end
end

class Perceptron
  attr_reader :w

  def initialize class_count, attribute_count, c=1
    @class_count = class_count
    @attr_count = attribute_count
    @c = c
    @w = Matrix.zero(@class_count, @attr_count + 1)
    def @w.punish ind, c, x
      self.column_size.times do |j|
        @rows[ind][j] -= c * x[j]
      end
    end
    def @w.compl ind, c, x
      self.column_size.times do |j|
        @rows[ind][j] += c * x[j]
      end
    end
  end

  def recognize vec
    d = (@w * Vector[ *vec, 1]).to_a
    d.index d.max
  end

  def train src_vec, real_class
    vec = Vector[ *src_vec, 1 ]
    d = (@w * vec).to_a
    recognized_class = d.index d.max

    res = recognized_class == real_class

    unless res
      to_punish = d.to_a.map.with_index{ |el, i| [i, el] }.find_all{ |(i, el)| el >= d[real_class] && i != real_class }
      @w.compl(real_class, @c, vec)
      to_punish.each do |(i, el)|
        @w.punish(i, @c, vec)
      end
    end
    res
  end
end

class_count = ARGV[0].to_i
attr_count = ARGV[1].to_i
training_points_per_class_count = ARGV[2].to_i

puts "Generating valid data for perceptron..."
begin
  samples = []
  class_count.times do |c_ind|
    cl_samples = []
    training_points_per_class_count.times do |p_ind|
      vec = Vector[ *Array.new(attr_count){ rand(-10..10) } ]
      cl_samples << vec
    end
    samples << cl_samples
  end

  perc = Perceptron.new(class_count, attr_count)
  valid = false
  1000.times do
    valid = samples.each.with_index.inject(true) do |res, (cl_samples, c_ind)|
      cl_samples.inject(true){ |in_res, sample| in_res && perc.train(sample, c_ind) } && res
    end
    break if valid
  end
end until valid

puts "Build success!"

puts "Training samples:"
puts samples.map.with_index{ |cl_samples, i| "#{i+1} class:\n#{cl_samples.map(&:to_a).map(&:to_s).join("\n")}" }.join("\n")

puts "Splitting functions:"
func_str = perc.w.to_a.map do |row|
  row.map.with_index do |el, i|
    if i < row.size - 1
      "#{el<0 ? ?- : ?+}"+"#{el.abs}*X#{i}".rjust(6)
    else
      "#{el<0 ? ?- : ?+}"+"#{el.abs}".rjust(6)
    end
  end.join(' ')
end.join("\n")
puts func_str

while true do
  puts "Please enter #{attr_count} numbers to test or 'q' to quit"
  str = $stdin.gets.chomp
  if str[0] == 'q'
    exit
  end
  vec = Vector[ *str.split.map(&:to_i) ]
  puts "Perceptron answer: #{perc.recognize(vec) + 1}"
  puts
end

