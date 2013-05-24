class SignatureDelegate
  attr_accessor :signature

  def initialize(delegate_obj, method = :calc_syndrom, convert_proc = lambda { |mem| mem })
    @obj = delegate_obj
    @method = method
    @convert_proc = convert_proc
    @signature = 0
  end

  def method_missing(method_name)
    @obj.respond_to?(method_name) ? @obj.public_send(method_name) : super
  end

  def sign(mem)
    @obj.public_send(@method, @convert_proc.call(mem))
  end

  def sign!(mem)
    @signature = sign(mem)
  end

  def to_s
    @signature.to_s(2)
  end
end
