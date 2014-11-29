module Utils
  ITERATIONS_THRESHOLD = 50_000

  class HopfieldNeuralNetwork
    attr_reader :teach_vectors, :n, :m

    def initialize(teach_images)
      @teach_vectors = teach_images.map{ |image| image_to_vector(image) }
      raise 'All teach images should have same size' unless @teach_vectors.all?{ |vector| vector.size == @teach_vectors.first.size }
      @n = @teach_vectors.first.size
      @m = @teach_vectors.size
    end

    def teach!
      (0...n).each do |i|
        (0...i).each do |j|
          wij = teach_vectors.inject(0){ |res, vector| res + vector[i] * vector[j] } / n.to_f
          set_w(i, j, wij)
        end
      end
    end

    def recognize(image)
      image_vector = image_to_vector(image)
      raise 'Recognized image should have same size as teach ones' unless image_vector.size == n
      iterations = 0
      while !recognized?(image_vector) && iterations < ITERATIONS_THRESHOLD
        picked_neuron_index = rand(n)
        response = image_vector.each.with_index.inject(0) do |sum, (el, j)|
          sum + w(picked_neuron_index, j) * el
        end
        image_vector[picked_neuron_index] = response > 0 ? 1 : -1
        iterations += 1
      end
      _teach_vector, teach_vector_index = teach_vectors.each.with_index.find{ |teach_vector, _index| image_vector == teach_vector }
      teach_vector_index
    end

    def recognized?(image_vector)
      teach_vectors.any?{ |teach_vector| image_vector == teach_vector }
    end

    def w(i, j)
      @w ||= Array.new(n){ Array.new(n, 0) }
      @w[i][j]
    end

    def set_w(i, j, val)
      @w ||= Array.new(n){ Array.new(n, 0) }
      val = 0 if i == j
      @w[i][j] = val
      @w[j][i] = val
    end

    def image_to_vector(image)
      image.image_data.map do |(y, _, _)|
        y == Magick::QuantumRange ? 1 : -1
      end
    end
  end
end
