require 'RMagick'
require 'slop'

require_relative '../utils/image'
require_relative '../utils/chainer'
require_relative '../utils/histogramm_plotter'
require_relative '../utils/processors/grayscale'
require_relative '../utils/processors/linear_contrast'
require_relative '../utils/processors/binaryzation'
require_relative '../utils/processors/areas'
require_relative '../utils/processors/classification'

DISTINCT_COLORS = [
  [65535, 0, 0], [0, 65535, 0], [0, 0, 65535], [65535, 65535, 0],
  [65535, 0, 65535], [0, 65535, 65535], [32896, 0, 0], [0, 32896, 0],
  [0, 0, 32896], [32896, 32896, 0], [32896, 0, 32896], [0, 32896, 32896],
  [32896, 32896, 32896], [49344, 0, 0], [0, 49344, 0], [0, 0, 49344],
  [49344, 49344, 0], [49344, 0, 49344], [0, 49344, 49344], [49344, 49344, 49344],
  [16448, 0, 0], [0, 16448, 0], [0, 0, 16448], [16448, 16448, 0], [16448, 0, 16448],
  [0, 16448, 16448], [16448, 16448, 16448], [8224, 0, 0], [0, 8224, 0], [0, 0, 8224],
  [8224, 8224, 0], [8224, 0, 8224], [0, 8224, 8224], [8224, 8224, 8224], [24672, 0, 0],
  [0, 24672, 0], [0, 0, 24672], [24672, 24672, 0], [24672, 0, 24672], [0, 24672, 24672],
  [24672, 24672, 24672], [41120, 0, 0], [0, 41120, 0], [0, 0, 41120],
  [41120, 41120, 0], [41120, 0, 41120], [0, 41120, 41120], [41120, 41120, 41120],
  [57568, 0, 0], [0, 57568, 0], [0, 0, 57568], [57568, 57568, 0],
  [57568, 0, 57568], [0, 57568, 57568], [57568, 57568, 57568]
]

opts = Slop.parse(help: true) do
  banner "Usage: linear_contrast.rb [options]"

  on 'infile=', 'input file'
  on 'outfile=', 'output file'
  on 'binaryzation-threshold=', 'binaryzation threshold as percent of quantum range', as: Float, default: 0.7
  on 'cluster-count=', 'count of object types in image', as: Integer
  on 'classify-by=', 'attributes used for classification (area, perimeter, density, elongation)', as: Array, default: [:area, :perimeter, :density, :elongation]
end

opts['classify-by'].map!{ |e| e.to_sym }

processors_spec = [
  [Utils::Processors::Grayscale, {}],
  [Utils::Processors::LinearContrast, {gmin: 0, gmax: ::Magick::QuantumRange,
                                       min_intens_percentile: 1,
                                       max_intens_percentile: 99,
                                       grayscaled: true}],
  [Utils::Processors::Binaryzation, {brightness_threshold: ::Magick::QuantumRange * opts['binaryzation-threshold']}],
  # [Utils::Processors::Areas, {modify_image: true, area_colors: DISTINCT_COLORS}]
  [Utils::Processors::Areas, {}],
  [Utils::Processors::Classification, {attributes: opts['classify-by'], cluster_count: opts['cluster-count'], object_colors: DISTINCT_COLORS}]
]

image = Utils::Image.new(opts[:infile])
Utils::Chainer.new(image, processors_spec).process!
image.save(opts[:outfile])
