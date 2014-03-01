require File.join(File.dirname(__FILE__), 'mystem_wrapper')

class Zipf
  attr_reader :text

  STOP_WORDS = %w[а без более бы был была были было быть в вам вас весь во вот все всего всех вы г где да даже для до его ее если есть еще же за здесь и из из-за или им их к как какой-то ко когда который кто ли либо мне может мы на надо наш не него нее нет ни них но ну о об однако он она они оно от очень по под при про с со т так также такой там те тем то того тоже той только том тот ты тут у уж уже хотя чего чей чем что чтобы чье чья эта эти это этот я]

  def initialize(filename)
    @text = File.read(filename)
  end

  def law1(filter_stop_words = false)
    return @law_freqs if @law_freqs
    normalized_words = normalize_words(filter_stop_words)
    word_counts = normalized_words.each_with_object({}){ |word, result| result[word] ||= 0; result[word] += 1 }
    summary_word_count = word_counts.inject(0){ |res, (_, word_count)| res + word_count }
    uniq_counts = word_counts.values.uniq.sort.reverse
    law_freqs = uniq_counts.map do |count|
      # picked_word = word_counts.find_all{ |_, word_count| count == word_count }.sample.first
      picked_word = word_counts.find_all{ |_, word_count| count == word_count }.first.first
      {freq: count / summary_word_count.to_f, word: picked_word}
    end
    @law_freqs = law_freqs
  end

  def keywords
    law_freqs = law1(true)
    middle = law_freqs.size / 2
    left = [0, middle - 4].max
    right = [law_freqs.size, middle + 4].min
    law_freqs.map{ |law_freq| law_freq[:word] }[left..right]
  end

  def avg_C(print_to_stdout = true)
    law_freqs = law1
    cs = law_freqs.map.with_index{ |law_freq, i| law_freq[:freq] * (i + 1)  }
    avg_C = cs.inject(0.0){ |c, sum| sum + c } / cs.size.to_f

    if print_to_stdout
      puts "All C: " + cs.map{ |c| c.round(3) }.join(', ')
      puts "Average C: " + avg_C.round(3).to_s
    end

    avg_C
  end

  def law2(filter_stop_words = false)
    return @freq_word_counts if @freq_word_counts
    normalized_words = normalize_words(filter_stop_words)
    word_counts = normalized_words.each_with_object({}){ |word, result| result[word] ||= 0; result[word] += 1 }
    counts_word_counts = word_counts.each_with_object({}) do |(_, word_count), result|
      result[word_count] ||= 0; result[word_count] += 1
    end
    summary_word_count = word_counts.inject(0){ |res, (_, word_count)| res + word_count }
    freq_word_counts = Hash[*
      counts_word_counts.map do |word_count, count_word_count|
        [word_count / summary_word_count.to_f, count_word_count]
      end.sort[1..-1].flatten
    ]
    @freq_word_counts = freq_word_counts
  end

  private
    def normalize_words(filter_stop_words = false)
      normalized_words = MystemWrapper.new(text).normalized_words
      normalized_words = normalized_words.select{ |word| !STOP_WORDS.include?(word) } if filter_stop_words
      normalized_words
    end
end
