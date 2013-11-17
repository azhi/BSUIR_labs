class AddDeletedAttributeToDealAndPortfolio < ActiveRecord::Migration
  def change
    add_column :portfolios, :deleted, :boolean, default: false
    add_column :deals, :deleted, :boolean, default: false
  end
end
