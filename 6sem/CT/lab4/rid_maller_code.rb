require 'mathn'
require File.join(File.dirname(__FILE__), 'extensions.rb')
require File.join(File.dirname(__FILE__), 'majority_decoder.rb')

class RidMallerCode
  attr_reader :g

  def initialize k, m
    @m = m
    @k = k

    cnks = []
    (0..m).each{ |i| cnks << Math.cnk(@m, i) }
    sum = 0
    crop_cnks = cnks.take_while{ |el| sum += el; @k > sum }
    raise "wrong k" unless sum == @k
    @r = crop_cnks.size

    g = []
    (0..m).to_a.reverse.each do |i|
      str = [1] * 2 ** i + [0] * 2 ** i
      str = str * (2 ** m / str.size) if str.size < 2 ** m
      g << Vector[*str.first(2 ** m)]
    end
    @g = g.dup
    (2..m).each do |comb_order|
      break if @g.size >= @k
      g[1..-1].combination(comb_order).each do |v1, v2|
        break if @g.size >= @k
        @g << v1.mul(v2)
      end
    end
    @g = Matrix[*@g]
  end

  def encode msg
    raise "message should be a vector!" unless msg.kind_of?(Vector)
    raise "message length should be #{@k}!" unless msg.size == @k
    (@g.transpose * Matrix[msg].transpose).transpose.row_vectors[0].map{ |el| el % 2 }
  end

  def decode msg
    raise "message should be a vector!" unless msg.kind_of?(Vector)
    raise "message length should be #{2 ** @m}!" unless msg.size == 2 ** @m
    MajorityDecoder.decode @g, msg
  end
end
