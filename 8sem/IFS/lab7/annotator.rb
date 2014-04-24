require_relative '../lab1/zipf.rb'
require_relative '../lab1/mystem_wrapper.rb'
require 'pry'

class Annotator
  attr_reader :text, :sentences, :keywords, :normalized_sentences

  def initialize(text_filename)
    language = text_filename.end_with?('.en.txt') ? :en : :ru
    @text = File.read(text_filename)
    @sentences = text.split(/(\.|\?|\!)/)
    @keywords = Zipf.new(text_filename, language).keywords
    @normalized_sentences = MystemWrapper.new(@text).normalized_text.split(/(\.|\?|\!)/)
    @picked_sentences = []
  end

  def annotate(required_percentage)
    keywords.each do |keyword|
      matched_sentences = normalized_sentences.find_all{ |sentence| sentence.include?(keyword) }
      matched_sentences.each do |sentence|
        @picked_sentences << normalized_sentences.index(sentence)
        percentage = current_percentage
        break if percentage > required_percentage ||
                   (percentage - required_percentage).abs < 5.0
      end
    end
    annotation = build_annotation
    actual_percentage = annotation.size / text.size.to_f * 100.0
    actual_percentage = actual_percentage.round(2)
    {actual_percentage: actual_percentage, annotation: annotation}
  end

  def current_percentage
    annotation = build_annotation
    annotation.size / text.size.to_f * 100.0
  end

  def build_annotation
    @picked_sentences.sort.uniq.map{ |sent_index| sentences[sent_index] }.join('.')
  end
end
