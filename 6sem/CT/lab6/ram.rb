require File.join(File.dirname(__FILE__), 'signature_delegate.rb')

class Ram
  attr_accessor :signature_delegate

  def initialize(length, signature_delegate, random_data = false)
    @length = length
    @mem = 0
    @signature_delegate = signature_delegate
    set_state @length.times.to_a.inject([]){ |res| res << (rand > 0.5 ? 1 : 0) } if random_data
    @signature_delegate.sign! @mem
  end

  def [](i)
    to_s[i].to_i
  end

  def []=(i, val)
    s = to_s
    str_val = val == 1 ? '1' : '0'
    if s[i] != str_val
      s[i] = str_val
      @mem = s.to_i(2)

      # @signature_delegate.sign! @mem
      @signature_delegate.signature ^= @signature_delegate.h.column_vectors[i].to_a.join.to_i(2)
    end
  end

  def length
    @length
  end

  def signature
    @signature_delegate
  end

  def gen_errors(params)
    if params.kind_of?(Array)
      positions = params
    elsif params.kind_of?(Integer)
      positions = (0...@length).to_a.shuffle.first(params)
    else
      raise "wrong params, should be array or integer"
    end

    s = to_s
    positions.each do |err_i|
      s[err_i] = self.[](err_i) == 1 ? '0' : '1'
    end
    @mem = s.to_i(2)
    self
  end

  def set_state(bits)
    bits.each.with_index do |bit, i|
      self.[]=(i, bit)
    end
    self
  end

  def valid?
    @signature_delegate.signature == @signature_delegate.sign(@mem)
  end

  def to_s
    @mem.to_s(2).rjust(@length, '0')
  end
end
