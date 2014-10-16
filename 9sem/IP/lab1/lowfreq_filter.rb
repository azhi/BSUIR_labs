require 'RMagick'
require 'slop'

require_relative '../utils/image'
require_relative '../utils/chainer'
require_relative '../utils/histogramm_plotter'
require_relative '../utils/processors/lowfreq_filter'

opts = Slop.parse(help: true) do
  banner "Usage: lowfreq_filter.rb [options]"

  on 'infile=', 'input file'
  on 'outfile=', 'output file'
  on 'primitive-id=', 'primitive-id (0, 1, 2)', as: Integer, default: 0
  on 'histogramm', 'show histogramms'
end

image = Utils::Image.new(opts[:infile])
HistogrammPlotter.plot(image.histogramm, 'Before correction') if opts[:histogramm]
Utils::Processors::LowfreqFilter.new(image).run!(
  primitive_id: opts['primitive-id']
)
HistogrammPlotter.plot(image.histogramm, 'After correction') if opts[:histogramm]
image.save(opts[:outfile])
