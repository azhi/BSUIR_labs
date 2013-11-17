class PortfoliosController < ApplicationController
  before_action :set_portfolio, only: [:show, :edit, :update, :destroy, :history]

  def index
    if params[:all].present?
      @portfolios = Portfolio.all
    else
      @portfolios = Portfolio.active
    end
  end

  def show
  end

  def history
  end

  def new
    @portfolio = Portfolio.new
  end

  def edit
  end

  def create
    @portfolio = Portfolio.new(portfolio_params)

    if @portfolio.save
      redirect_to @portfolio, notice: 'Portfolio was successfully created.'
    else
      render action: 'new'
    end
  end

  def update
    if @portfolio.update(portfolio_params)
      redirect_to @portfolio, notice: 'Portfolio was successfully updated.'
    else
      render action: 'edit'
    end
  end

  def destroy
    @portfolio.update_attribute(:deleted, true)
    redirect_to portfolios_url, notice: 'Portfolio was successfully destroyed.'
  end

  private
    def set_portfolio
      @portfolio = Portfolio.find(params[:id])
    end

    def portfolio_params
      params.require(:portfolio).permit(:name)
    end
end
