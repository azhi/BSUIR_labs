class MajorityDecoder
  def self.decode g, msg
    ks = []
    (0...g.row_size).each do |i|
      res = {0 => 0, 1 => 1}
      candidates = []
      (1..g.column_size).each do |comb_order|
        candidates += g.column_vectors.map.with_index.to_a.combination(comb_order).to_a.delete_if{ |el|
          el.inject(Vector[* [0] * g.row_size ]){ |sum, el| sum ^ el[0] }.to_a != (ar = [0] * g.row_size; ar[i] = 1; ar)
        }.map{ |el| el.inject(Vector[* [0] * g.column_size ]){ |vec, el| vec[el[1]] = 1; vec } }
      end
      candidates = candidates.reverse
      candidates.delete_if{ |el|
        delete = false
        (1...candidates.size).each do |comb_order|
          candidates.combination(comb_order).each do |comb|
            sum = comb.inject(Vector[* [0] * g.column_size]){ |sum, el| sum ^ el }
            if sum.to_a == el.to_a && !comb.map(&:to_a).include?(el.to_a)
              delete = true
              break;
            end
          end unless delete
        end
        delete
      }.each do |equas|
        guess = equas.to_enum.with_index.find_all{ |el| el.first == 1 }.inject(0){ |sum, el| sum ^ msg[el[1]] }
        res[guess] += 1
      end
      ks << res.max_by{ |k, v| v }.first
    end

    Vector[* ks]
  end
end
