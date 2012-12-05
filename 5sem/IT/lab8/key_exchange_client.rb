require File.join(File.dirname(__FILE__), 'elliptic_group.rb')

class KeyExchangeClient
  attr_reader :n, :p, :k
  def initialize el_g, g
    @eg = el_g
    @g = g
    begin
      @n = rand(1...@eg.m)
      @p = @eg.mul @g, @n
    end while @p == @eg.zero
  end

  def calc_k o_p
    @k = @eg.mul o_p, @n
  end
end
