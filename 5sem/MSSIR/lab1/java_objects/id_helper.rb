class IDHelper
  def self.get_next
    @@cur_id ||= -1
    @@cur_id += 1
  end
end
