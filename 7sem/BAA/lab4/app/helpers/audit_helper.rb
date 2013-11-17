module AuditHelper
  def attribute_diff(attr_before, attr_after)
    %w[created_at updated_at].each{ |k| attr_before.delete(k); attr_after.delete(k) }
    html = ""
    keys = (attr_before.keys + attr_after.keys).uniq
    keys.each do |key|
      if attr_before[key].present? && attr_after[key].present? && attr_before[key] != attr_after[key]
        html += "<p>CHANGE #{key} from #{attr_before[key]} to #{attr_after[key]}</p>"
      elsif attr_before[key].present? && !attr_after[key].present?
        html += "<p>REMOVE #{key} (was #{attr_before[key]})</p>"
      elsif !attr_before[key].present? && attr_after[key].present?
        html += "<p>ADDED #{key} (now #{attr_after[key]})</p>"
      end
    end
    html
  end
end
