class AuditController < ApplicationController
  def select_user
  end

  def show
    user_id = params[:id]
    @versions = PaperTrail::Version.where(whodunnit: user_id)
  end
end
