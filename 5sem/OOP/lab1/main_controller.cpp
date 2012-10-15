#include "main_controller.h"

#include <fstream>
#include <string>
#include <SDL/SDL_gfxPrimitives.h>

Main_controller::Main_controller()
{
  sdl_controller = new SDL_controller();
  scene = new Scene();
  draw_mode = 0;
}

Main_controller::~Main_controller()
{
  delete sdl_controller;
}

void Main_controller::add_figure_creator(maker_function maker, draw_mode_getter dw_get)
{
  factory.push_back(maker);
  draw_modes.push_back(dw_get);
}

void Main_controller::main_loop()
{
  bool quit = false;
  Point down = Point(0, 0);
  vector<Point>* points_list = new vector<Point>;
  bool in_drawing = false;
  bool in_collecting_points = false;
  bool multiple_drawing = false; 
  Figure* figure;
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
            draw_mode = 0;
            break;
          case SDLK_w:
            draw_mode = 1;
            break;
          case SDLK_e:
            draw_mode = 2;
            break;
          case SDLK_r:
            draw_mode = 3;
            break;
          case SDLK_t:
            draw_mode = 4;
            break;
          case SDLK_y:
            draw_mode = 5;
            break;
          case SDLK_s:
            {
              char c_filename[300];
              FILE* in = popen("kdialog --getsavefilename $(pwd)", "r");
              fscanf(in, "%s", c_filename);
              pclose(in);
              std::string filename = c_filename;

              std::string ext = filename.substr( filename.length() - 3 );
              if ( ext.compare("xml") == 0 )
              {
                std::ofstream ofs(filename.c_str(), std::ios::out);
                boost::archive::xml_oarchive xml_arch(ofs);
                xml_arch << BOOST_SERIALIZATION_NVP(scene);
                ofs.close();
              }
              else if ( ext.compare("bin") == 0 )
              {
                std::ofstream ofs(filename.c_str(), std::ios::out);
                boost::archive::binary_oarchive binary_arch(ofs);
                binary_arch << BOOST_SERIALIZATION_NVP(scene);
                ofs.close();
              }
              else
              {
                std::ofstream ofs(filename.c_str(), std::ios::out);
                boost::archive::text_oarchive text_arch(ofs);
                text_arch << BOOST_SERIALIZATION_NVP(scene);
                ofs.close();
              }
            }
            break;
          case SDLK_l:
            {
              char c_filename[300];
              FILE* in = popen("kdialog --getopenfilename $(pwd)", "r");
              fscanf(in, "%s", c_filename);
              pclose(in);
              std::string filename = c_filename;

              std::string ext = filename.substr( filename.length() - 3 );
              if ( ext.compare("xml") == 0 )
              {
                std::ifstream ifs(filename.c_str(), std::ios::in);
                delete scene;
                boost::archive::xml_iarchive xml_arch(ifs);
                xml_arch >> BOOST_SERIALIZATION_NVP(scene);
                ifs.close();
              }
              else if ( ext.compare("bin") == 0 )
              {
                std::ifstream ifs(filename.c_str(), std::ios::in);
                delete scene;
                boost::archive::binary_iarchive binary_arch(ifs);
                binary_arch >> BOOST_SERIALIZATION_NVP(scene);
                ifs.close();
              }
              else
              {
                std::ifstream ifs(filename.c_str(), std::ios::in);
                delete scene;
                boost::archive::text_iarchive text_arch(ifs);
                text_arch >> BOOST_SERIALIZATION_NVP(scene);
                ifs.close();
              }
              scene->set_working_surface( sdl_controller->get_surface() );
            }
            break;
        }
        break;
      case SDL_MOUSEBUTTONDOWN:
        {
          multiple_drawing = false;
          draw_mode_getter get_mode = NULL;
          if ( draw_mode < draw_modes.size() )
            get_mode = draw_modes[draw_mode];
          if ( get_mode == NULL )
            fprintf(stderr, "Doesn't have mode getter for that figure\n");
          else
            multiple_drawing = get_mode();

          if ( event.button.button == SDL_BUTTON_LEFT )
          {
            if ( in_collecting_points )
            {
              points_list->push_back( Point(event.button.x, event.button.y) );
              points_list->push_back( Point(event.button.x, event.button.y) );

              vector<Point>::iterator cur_point, prev_point;
              prev_point = points_list->begin();
              cur_point = prev_point + 1;
              while ( cur_point != points_list->end() )
              {
                lineRGBA(sdl_controller->get_surface(), prev_point->x, prev_point->y, cur_point->x, cur_point->y, 0, 0, 0, 255);
                cur_point++;
                prev_point++;
              }
            }
            else if ( !in_collecting_points && multiple_drawing )
            {
              points_list->clear();
              in_collecting_points = true;
              in_drawing = true;
              points_list->push_back( Point(event.button.x, event.button.y) );
              points_list->push_back( Point(event.button.x, event.button.y) );
            }
            else
            {
              points_list->clear();
              points_list->push_back( Point(event.button.x, event.button.y) );
              points_list->push_back( Point(event.button.x, event.button.y) );

              maker_function maker = NULL;
              if ( draw_mode < factory.size() )
                maker = factory[draw_mode];
              if ( maker == NULL )
                fprintf(stderr, "Doesn't have maker for that figure\n");
              else
              {
                in_drawing = true;
                figure = maker(points_list);
                figure->set_surface( sdl_controller->get_surface() );
                scene->add_figure( figure );
              }
            }
          }
          else if ( event.button.button == SDL_BUTTON_RIGHT && multiple_drawing && in_collecting_points )
          {
            in_collecting_points = false;
            in_drawing = false;
            vector<Point>::iterator first = points_list->begin();
            vector<Point>::iterator last = points_list->end() - 1;
            points_list->push_back( Point(first->x, first->y) );
            maker_function maker = NULL;
            if ( draw_mode < factory.size() )
              maker = factory[draw_mode];
            if ( maker == NULL )
              fprintf(stderr, "Doesn't have maker for that figure\n");
            else
            {
              figure = maker(points_list);
              figure->set_surface( sdl_controller->get_surface() );
              scene->add_figure( figure );
            }
          }
        }
        break;
      case SDL_MOUSEMOTION:
        {
          multiple_drawing = false;
          draw_mode_getter get_mode = NULL;
          if ( draw_mode < draw_modes.size() )
            get_mode = draw_modes[draw_mode];
          if ( get_mode == NULL )
            fprintf(stderr, "Doesn't have mode getter for that figure\n");
          else
            multiple_drawing = get_mode();

          if ( in_drawing )
          {
            if ( !multiple_drawing )
            {
              Point up;
              up.x = event.motion.x; 
              up.y = event.motion.y;
              figure->move_point2(up);
            }
            else
            {
              vector<Point>::iterator cur_point, prev_point;
              prev_point = points_list->begin();
              cur_point = prev_point + 1;
              while ( cur_point != points_list->end() )
              {
                lineRGBA(sdl_controller->get_surface(), prev_point->x, prev_point->y, cur_point->x, cur_point->y, 0, 0, 0, 255);
                cur_point++;
                prev_point++;
              }
            }
          }
        }
        break;
      case SDL_MOUSEBUTTONUP:
        {
          multiple_drawing = false;
          draw_mode_getter get_mode = NULL;
          if ( draw_mode < draw_modes.size() )
            get_mode = draw_modes[draw_mode];
          if ( get_mode == NULL )
            fprintf(stderr, "Doesn't have mode getter for that figure\n");
          else
            multiple_drawing = get_mode();

          if ( event.button.button == SDL_BUTTON_LEFT && !multiple_drawing)
          {
            in_drawing = false;
          }

          if ( multiple_drawing )
          {
            vector<Point>::iterator cur_point, prev_point;
            prev_point = points_list->begin();
            cur_point = prev_point + 1;
            while ( cur_point != points_list->end() )
            {
              lineRGBA(sdl_controller->get_surface(), prev_point->x, prev_point->y, cur_point->x, cur_point->y, 0, 0, 0, 255);
              cur_point++;
              prev_point++;
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
