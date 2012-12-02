#include "main_controller.h"

Main_controller::Main_controller()
{
  gm = new Grammatic_analyzer();
  sdl_controller = new SDL_controller();
  scene = new Scene();
  draw_mode = MO_LINE;
  main_loop();
}

Main_controller::~Main_controller()
{
  delete sdl_controller;
}

void Main_controller::main_loop()
{
  bool quit = false;
  Point down = Point(0, 0);
  vector<Point>* points_list = new vector<Point>;
  bool in_drawing = false;
  while ( !quit )
  {
    SDL_Event event = sdl_controller->get_event();
    sdl_controller->clear();
    switch ( event.type )
    {
      case SDL_KEYDOWN:
        switch ( event.key.keysym.sym )
        {
          case SDLK_q:
            draw_mode = MO_LINE;
            break;
          case SDLK_w:
            draw_mode = MO_RECTANGLE;
            break;
          case SDLK_e:
            draw_mode = MO_ELLIPSE;
            break;
          case SDLK_g:
            scene->clear();
            gm->build_match_scene(scene);
            scene->set_working_surface( sdl_controller->get_surface() );
            break;
        }
        break;
      case SDL_MOUSEBUTTONDOWN:
        if ( event.button.button == SDL_BUTTON_LEFT )
        {
          switch ( draw_mode )
          {
            case MO_LINE:
            case MO_RECTANGLE:
            case MO_ELLIPSE:  
              down.x = event.button.x; 
              down.y = event.button.y;
              in_drawing = true;
              break;
          }
        }
        break;
      case SDL_MOUSEBUTTONUP:
        if ( event.button.button == SDL_BUTTON_LEFT )
        {
          Point up;
          up.x = event.button.x; 
          up.y = event.button.y;
          switch ( draw_mode )
          {
            case MO_LINE:
              {
                Line* line = new Line(down.x, down.y, up.x, up.y);
                line->set_surface( sdl_controller->get_surface() );
                scene->add_figure(line);
                in_drawing = false;
                break;
              }
            case MO_RECTANGLE:
              {
                Rectangle* rectangle = new Rectangle(down.x, down.y, up.x, up.y);
                rectangle->set_surface( sdl_controller->get_surface() );
                scene->add_figure(rectangle);
                in_drawing = false;
                break;
              }
            case MO_ELLIPSE:  
              {
                int rx = abs(up.x - down.x);
                int ry = abs(up.y - down.y);
                Ellipse* ellipse = new Ellipse( down.x, down.y, rx, ry);
                ellipse->set_surface( sdl_controller->get_surface() );
                scene->add_figure(ellipse);
                in_drawing = false;
                break;
              }
          }
        }
        break;  
      case SDL_MOUSEMOTION:
        if ( in_drawing )
        {
          Point up;
          up.x = event.motion.x; 
          up.y = event.motion.y;
          switch ( draw_mode )
          {
            case MO_LINE:
              lineRGBA(sdl_controller->get_surface(), down.x, down.y, up.x, up.y, 0, 0, 0, 255);
              break;
            case MO_RECTANGLE:
              rectangleRGBA(sdl_controller->get_surface(), down.x, down.y, up.x, up.y, 0, 0, 0, 255);
              break;
            case MO_ELLIPSE:  
              int rx = abs(up.x - down.x);
              int ry = abs(up.y - down.y);
              ellipseRGBA(sdl_controller->get_surface(), down.x, down.y, rx, ry, 0, 0, 0, 255);
              break;
          }
        }
        break;
      case SDL_QUIT:
        quit = true;
        break;
    }
    scene->draw_all();
    sdl_controller->redraw();
  }
}
