require File.join(File.dirname(__FILE__), 'mystem_wrapper')

class Zipf
  attr_reader :text, :language

  STOP_WORDS = {
    ru: %w[а без более бы был была были было быть в вам вас весь во вот все всего всех вы г где да даже для до его ее если есть еще же за здесь и из из-за или им их к как какой-то ко когда который кто ли либо мне может мы на надо наш не него нее нет ни них но ну о об однако он она они оно от очень по под при про с со т так также такой там те тем то того тоже той только том тот ты тут у уж уже хотя чего чей чем что чтобы чье чья эта эти это этот я],
    en: %w[a about above after again against all am an and any are aren't as at be because been before being below between both but by can't cannot could couldn't did didn't do does doesn't doing don't down during each few for from further had hadn't has hasn't have haven't having he he'd he'll he's her here here's hers herself him himself his how how's i i'd i'll i'm i've if in into is isn't it it's its itself let's me more most mustn't my myself no nor not of off on once only or other ought our ours ourselves out over own same shan't she she'd she'll she's should shouldn't so some such than that that's the their theirs them themselves then there there's these they they'd they'll they're they've this those through to too under until up very was wasn't we we'd we'll we're we've were weren't what what's when when's where where's which while who who's whom why why's with won't would wouldn't you you'd you'll you're you've your yours yourself yourselves]
  }

  def initialize(filename, language = :ru)
    @text = File.read(filename)
    @language = language.to_sym
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
      normalized_words = language == :ru ?
                           MystemWrapper.new(text).normalized_words :
                           text.gsub(/[~!@#$%^&*()_+=\/?.,<>'";:\[\]{}]/, '').split(' ').
                                map{ |word| !word.empty? ? word : nil }.
                                compact.map(&:downcase)
      normalized_words = normalized_words.select{ |word| !STOP_WORDS[language].include?(word) } if filter_stop_words
      normalized_words
    end
end
