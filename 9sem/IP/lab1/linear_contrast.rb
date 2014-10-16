require 'RMagick'
require 'slop'

require_relative '../utils/image'
require_relative '../utils/chainer'
require_relative '../utils/histogramm_plotter'
require_relative '../utils/processors/grayscale'
require_relative '../utils/processors/linear_contrast'

opts = Slop.parse(help: true) do
  banner "Usage: linear_contrast.rb [options]"

  on 'infile=', 'input file'
  on 'outfile=', 'output file'
  on 'gmin=', 'new minimum image brightness', as: Integer, default: 0
  on 'gmax=', 'new maximum image brightness', as: Integer, default: Magick::QuantumRange
  on 'min-intens-percentile=', 'intensity precentile count as fmin', as: Float, default: 2
  on 'max-intens-percentile=', 'intensity precentile count as fmax', as: Float, default: 98
  on 'grayscale', 'consider input image already grayscaled'
  on 'histogramm', 'show histogramms'
end

image = Utils::Image.new(opts[:infile])
HistogrammPlotter.plot(image.histogramm(opts[:grayscale]), 'Before correction') if opts[:histogramm]
Utils::Processors::LinearContrast.new(image).run!(
  gmin: opts[:gmin], gmax: opts[:gmax], grayscaled: opts[:grayscale],
  min_intens_percentile: opts['min-intens-percentile'],
  max_intens_percentile: opts['max-intens-percentile']
)
HistogrammPlotter.plot(image.histogramm(opts[:grayscale]), 'After correction') if opts[:histogramm]
image.save(opts[:outfile])
