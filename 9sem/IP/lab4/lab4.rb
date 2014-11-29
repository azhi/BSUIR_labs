require 'RMagick'
require 'slop'
require 'colorize'

require_relative '../utils/image'
require_relative '../utils/hopfield_neural_network'

opts = Slop.parse(help: true) do
  banner "Usage: lab4.rb [options]"

  on 'teach-images=', 'teach images', as: Array
  on 'test-directory=', 'directory with test images'
  on 'colorize', 'enable output colorize (test images should have same names as teach ones with suffix)'
end

teach_images = opts['teach-images'].map{ |path| [File.basename(path), Utils::Image.new(path)] }
neural_network = Utils::HopfieldNeuralNetwork.new(teach_images.map(&:last))
neural_network.teach!
Dir["#{opts['test-directory']}/**/*"].sort.each do |file|
  next unless File.file?(file)
  image = Utils::Image.new(file)
  index = neural_network.recognize(image)
  if index
    teach_image = teach_images[index].first
    msg = "#{file} got recognized as #{teach_image}"
    if opts[:colorize]
      teach_filename = File.basename(teach_image, File.extname(teach_image))
      image_filename = File.basename(file, File.extname(file))
      color = image_filename.start_with?(teach_filename) ? :green : :red
      msg = msg.colorize(color)
    end
    puts msg
  else
    msg = "#{file} is unstable!"
    msg = msg.colorize(:red) if opts[:colorize]
    puts msg
  end
end
