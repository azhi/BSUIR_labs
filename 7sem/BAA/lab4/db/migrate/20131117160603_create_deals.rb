class CreateDeals < ActiveRecord::Migration
  def change
    create_table :deals do |t|
      t.string :name
      t.belongs_to :portfolio
      t.datetime :date
      t.string :buy_sell
      t.string :contractor
      t.float :price

      t.timestamps
    end
  end
end
