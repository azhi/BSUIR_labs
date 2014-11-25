require 'RMagick'
require 'slop'

require_relative '../utils/image'
require_relative '../utils/processors/gaussian'

opts = Slop.parse(help: true) do
  banner "Usage: blur.rb [options]"

  on 'infile=', 'input file'
  on 'outdir=', 'output directory'
end

(1..3).step(0.2).each do |t|
  sigma = Math.exp(t)
  image = Utils::Image.new(opts[:infile])
  puts "Sigma: #{sigma.round(2)}"
  Utils::Processors::Gaussian.new(image).run!(sigma: sigma)
  image.save(File.join(opts[:outdir], "/#{sigma.round(2)}.png"))
end
