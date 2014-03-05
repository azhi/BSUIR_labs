require File.join(File.dirname(__FILE__), '../lab1/zipf.rb')
require 'pry'

class TextSearch
  attr_reader :texts_keywords, :search_keywords, :pattern_text_filename

  def initialize(texts_filenames, pattern_text_filename)
    @pattern_text_filename = pattern_text_filename
    @texts_keywords = texts_filenames.map do |filename|
      language = filename.end_with?('.en.txt') ? :en : :ru
      {filename: filename, keywords: Zipf.new(filename, language).keywords}
    end
    language = pattern_text_filename.end_with?('.en.txt') ? :en : :ru
    @search_keywords = Zipf.new(pattern_text_filename, language).keywords
  end


  def search
    texts_search_info = texts_keywords.map do |text|
      text_keywords = text[:keywords]
      matched_keywords = search_keywords & text_keywords
      matched_percentage = matched_keywords.size / [texts_keywords.size, search_keywords.size].min.to_f
      text.merge(matched: matched_keywords, matched_percentage: matched_percentage)
    end

    matched_texts = texts_search_info.find_all do |text|
      text[:matched_percentage] >= 0.2 && text[:filename] != pattern_text_filename
    end.sort_by{ |text| text[:matched_percentage] }.reverse
    matched_texts
  end
end
