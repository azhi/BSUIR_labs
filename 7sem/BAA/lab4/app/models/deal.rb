class Deal < ActiveRecord::Base
  has_paper_trail

  extend Enumerize

  belongs_to :portfolio
  enumerize :buy_sell, in: [:buy, :sell]

  validates :name, :portfolio, :date, :buy_sell, :contractor, :price, presence: true

  scope :active, ->{ where(deleted: false) }
end
