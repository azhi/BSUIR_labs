require 'RMagick'
require 'slop'

require_relative '../utils/image'
require_relative '../utils/processors/blob_detector'

opts = Slop.parse(help: true) do
  banner "Usage: lab3.rb [options]"

  on 'infile=', 'input file'
  on 'outfile=', 'output file'
  on 'enable-area-filter', 'enable area filter'
  on 'dog-brightness-threshold=', 'DoG brightness threshold', as: Integer, default: 10_000
end

image = Utils::Image.new(opts[:infile])
Utils::Processors::BlobDetector.new(image).run!(
  dog_brightness_threshold: opts['dog-brightness-threshold'],
  enable_area_filter: opts['enable-area-filter']
)
image.save(opts[:outfile])
