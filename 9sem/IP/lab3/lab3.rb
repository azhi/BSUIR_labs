require 'RMagick'
require 'slop'

require_relative '../utils/image'
require_relative '../utils/processors/blob_detector'

opts = Slop.parse(help: true) do
  banner "Usage: lab3.rb [options]"

  on 'infile=', 'input file'
  on 'outfile=', 'output file'
end

image = Utils::Image.new(opts[:infile])
Utils::Processors::BlobDetector.new(image).run!
image.save(opts[:outfile])
