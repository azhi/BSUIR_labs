require 'open3'

class MystemWrapper
  WORD_REGEX = /\{(.+?)\}/
  attr_reader :text

  def initialize(text)
    @text = text
  end

  def normalized_words
    data = run_mystem(words_arguments)

    normalized_words = data.scan(WORD_REGEX).map do |match|
      word_options = match.first
      normalize_word(word_options)
    end.compact
    normalized_words
  end

  def normalized_text
    data = run_mystem(text_arguments)

    normalized_text = data.gsub(WORD_REGEX) do |word_options|
      word_options = word_options[1...-1]
      normalize_word(word_options)
    end
    normalized_text
  end

  private
    def normalize_word(word_options)
      # word = word_options.split(?|).sample
      word = word_options.split(?|).first
      process_unknown_word(word)
    end

    def process_unknown_word(word)
      return nil if word.end_with? '??'
      word = word[0...-1] if word.end_with? '?'
      word
    end

    def run_mystem(arguments)
      exec = [command, arguments].join(' ')

      data = Open3.popen3(exec) do |stdin, stdout, _|
        stdin.write text
        stdin.close
        stdout.read
      end
      data
    end

    def command
      File.join(File.dirname(__FILE__), '/mystem')
    end

    def words_arguments
      '-l -e utf-8'
    end

    def text_arguments
      '-l -c -e utf-8'
    end
end
