require 'RMagick'
require 'slop'

require_relative '../utils/image'
require_relative '../utils/chainer'
require_relative '../utils/processors/color_shift'

opts = Slop.parse(help: true) do
  banner "Usage: lab1.rb [options]"

  on 'infile=', 'input file'
  on 'outfile=', 'output file'
  on 'r', 'shift-r=', 'red channel shift', as: Integer, default: 0
  on 'g', 'shift-g=', 'green channel shift', as: Integer, default: 0
  on 'b', 'shift-b=', 'blue channel shift', as: Integer, default: 0
end

processors_spec = {
  Utils::Processors::ColorShift => {r: opts['shift-r']},
  Utils::Processors::ColorShift => {g: opts['shift-g'], b: opts['shift-b']}
}

image = Utils::Image.new(opts[:infile])
Utils::Chainer.new(image, processors_spec).process!
image.save(opts[:outfile])
