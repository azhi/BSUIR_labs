class Portfolio < ActiveRecord::Base
  has_paper_trail

  has_many :deals, dependent: :destroy

  validates :name, presence: true

  scope :active, ->{ where(deleted: false) }
end
