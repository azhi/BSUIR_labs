require 'securerandom'

class RNG
  def self.rand
    SecureRandom.random_number
  end
end
