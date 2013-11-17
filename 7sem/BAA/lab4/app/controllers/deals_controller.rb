class DealsController < ApplicationController
  before_action :set_deal, only: [:show, :edit, :update, :destroy, :history]

  def index
    if params[:all].present?
      @deals = Deal.all
    else
      @deals = Deal.active
    end
  end

  def show
  end

  def history
  end

  def new
    @deal = Deal.new
  end

  def edit
  end

  def create
    @deal = Deal.new(deal_params)

    if @deal.save
      redirect_to @deal, notice: 'Deal was successfully created.'
    else
      render action: 'new'
    end
  end

  def update
    if @deal.update(deal_params)
      redirect_to @deal, notice: 'Deal was successfully updated.'
    else
      render action: 'edit'
    end
  end

  def destroy
    @deal.update_attribute(:deleted, true)
    redirect_to deals_url, notice: 'Deal was successfully destroyed.'
  end

  private
    def set_deal
      @deal = Deal.find(params[:id])
    end

    def deal_params
      params.require(:deal).permit(:name, :portfolio_id, :date, :buy_sell, :contractor, :price)
    end
end
