Lab4::Application.routes.draw do
  resources :deals do
    get :history, on: :member
  end

  resources :portfolios do
    get :history, on: :member
  end

  devise_for :users

  root to: 'pages#home'
  get '/audit', to: 'audit#select_user'
  post '/audit/user', to: 'audit#show'
end
