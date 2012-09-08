#include "main_controller.h"

Main_controller::Main_controller()
{
  sdl_controller = new SDL_controller();
  scene = new Scene();
  scene->add_figure(new Line(50, 100, 100, 200));
  scene->add_figure(new Line(400, 100, 100, 500));
  scene->add_figure(new Ellipse(200, 100, 20, 40));
  scene->add_figure(new Line(50, 150, 100, 200));
  scene->add_figure(new Rectangle(50, 100, 100, 200));
  scene->set_working_surface( sdl_controller->get_surface() );
  main_loop();
}

Main_controller::~Main_controller()
{
  delete sdl_controller;
}

void Main_controller::main_loop()
{
  bool quit = false;
  while ( !quit )
  {
    SDL_Event event = sdl_controller->get_event();
    switch ( event.type )
    {
      case SDL_KEYDOWN:
        // if ( event.key.keysym.sym == SDLK_F11 )
        //   redraw();
        break;
      case SDL_MOUSEBUTTONDOWN:
        if ( event.button.button == SDL_BUTTON_LEFT )
        {
        }
        break;
      case SDL_MOUSEBUTTONUP:
        if ( event.button.button == SDL_BUTTON_LEFT )
        {
        }
        break;  
      case SDL_QUIT:
        quit = true;
        break;
    }
    sdl_controller->clear();
    scene->draw_all();
    sdl_controller->redraw();
  }
}
