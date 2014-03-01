require 'open3'

class MystemWrapper
  attr_reader :text

  def initialize(text)
    @text = text
  end

  def normalized_words
    data = run_mystem

    word_regex = /\{(.+?)\}/
    normalized_words = data.scan(word_regex).map do |match|
      words = match.first
      # word = words.split(?|).sample
      word = words.split(?|).first
      normalize_word(word)
    end.compact
    normalized_words
  end

  private
    def run_mystem
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

    def arguments
      '-l -e utf-8'
    end

    def normalize_word(word)
      return nil if word.end_with? '??'
      word = word[0...-1] if word.end_with? '?'
      word
    end
end
