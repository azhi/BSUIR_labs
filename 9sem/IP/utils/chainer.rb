module Utils
  class Chainer
    attr_reader :image, :processors_spec

    def initialize(image, processors_spec)
      @image, @processors_spec = image, processors_spec
    end

    def process!
      processors_spec.each do |processor_class, processor_args|
        processor_class.new(image).run!(processor_args)
      end
    end
  end
end
