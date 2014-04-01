require_relative '../lab1/zipf'
require_relative '../../../5sem/MADM/lab4/perceptron'
require 'set'
require 'pry'

class PerceptronTextClassificator
  attr_reader :classes, :perceptron

  def initialize(classes, train_texts)
    @classes = classes
    @train_texts = train_texts
    prepare_train_texts
    @perceptron = build_perceptron
  end

  def recognize(text_filename)
    language = text_filename.end_with?('.en.txt') ? :en : :ru
    keywords = Zipf.new(text_filename, language).keywords
    attributes = perceptron_attributes(keywords)
    class_number = perceptron.recognize(attributes)
    classes[class_number]
  end

  private
    def prepare_train_texts
      @train_texts_keywords = @train_texts.map do |class_texts|
        class_texts.map do |filename|
          language = filename.end_with?('.en.txt') ? :en : :ru
          {filename: filename, keywords: Zipf.new(filename, language).keywords}
        end
      end
      @common_attributes_list = @train_texts_keywords.map do |class_texts|
        class_texts.map do |text|
          Set.new(text[:keywords])
        end
      end.flatten.reduce(:+).to_a
      @train_texts_attributes = @train_texts_keywords.map do |class_texts|
        class_texts.map do |text|
          perceptron_attributes(text[:keywords])
        end
      end
    end

    def perceptron_attributes(text_keywords)
      @common_attributes_list.map{ |keyword_attribute| text_keywords.include?(keyword_attribute) ? 1 : 0 }
    end

    def build_perceptron
      perceptron = Perceptron.try_build_by_samples(classes.count, @common_attributes_list.size,
                                                   @train_texts_attributes)
      raise StandardError, "Can't build perceptron with this classes." unless perceptron
      perceptron
    end
end
