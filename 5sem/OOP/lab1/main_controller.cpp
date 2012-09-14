#include "main_controller.h"

#include <fstream>
#include <string>
#include <SDL/SDL_gfxPrimitives.h>

BOOST_CLASS_EXPORT_IMPLEMENT(Ellipse)
BOOST_CLASS_EXPORT_IMPLEMENT(Line)
BOOST_CLASS_EXPORT_IMPLEMENT(Rectangle)

Main_controller::Main_controller()
{
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
          case SDLK_s:
            {
              system("kdialog --getsavefilename $(pwd) '*.xml' > /tmp/filename.tmp");
              std::ifstream ifs("/tmp/filename.tmp", std::ios::in);
              std::string filename;
              ifs >> filename;
              ifs.close();
              std::ofstream ofs(filename.c_str(), std::ios::out);
              boost::archive::xml_oarchive xml_arch(ofs);
              xml_arch << BOOST_SERIALIZATION_NVP(scene);
              ofs.close();
            }
            break;
          case SDLK_l:
            {
              system("kdialog --getopenfilename $(pwd) '*.xml' > /tmp/filename.tmp");
              std::ifstream ifs("/tmp/filename.tmp", std::ios::in);
              std::string filename;
              ifs >> filename;
              ifs.close();
              ifs.open(filename.c_str(), std::ios::in);
              boost::archive::xml_iarchive xml_arch(ifs);
              delete scene;
              xml_arch >> BOOST_SERIALIZATION_NVP(scene);
              scene->set_working_surface( sdl_controller->get_surface() );
              ifs.close();
            }
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
              // printf("in button down!\n");
              // if ( !in_drawing )
              // {
              //   in_drawing = true;
              //   points_list->clear();
              // }
              // points_list->push_back( Point(event.button.x, event.button.y) );
              // printf("pushed, size: %d\n", points_list->size());
              // break;
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
      case SDL_MOUSEBUTTONUP:
        if ( event.button.button == SDL_BUTTON_LEFT )
        {
          Point up;
          up.x = event.button.x; 
          up.y = event.button.y;
          in_drawing = false;
          switch ( draw_mode )
          {
            case MO_LINE:
            {
              Line* line = new Line(down.x, down.y, up.x, up.y);
              line->set_surface( sdl_controller->get_surface() );
              scene->add_figure(line);
              break;
            }
            case MO_RECTANGLE:
            {
              Rectangle* rectangle = new Rectangle(down.x, down.y, up.x, up.y);
              rectangle->set_surface( sdl_controller->get_surface() );
              scene->add_figure(rectangle);
              break;
            }
            case MO_ELLIPSE:  
            {
              int rx = abs(up.x - down.x);
              int ry = abs(up.y - down.y);
              Ellipse* ellipse = new Ellipse( down.x, down.y, rx, ry);
              ellipse->set_surface( sdl_controller->get_surface() );
              scene->add_figure(ellipse);
              break;
            }
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
