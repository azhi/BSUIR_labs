#ifndef MAIN_CONTROLLER_H
#define MAIN_CONTROLLER_H

#include "sdl_controller.h"
#include "scene.h"
#include <map>

using namespace std;

typedef Figure* (*maker_function)(vector<Point>* points);

class Main_controller
{
  public:
    Main_controller();
    virtual ~Main_controller();
    void main_loop();
		void add_figure_creator(maker_function maker);

  private:
    SDL_controller* sdl_controller;
		vector<maker_function> factory;
    short draw_mode;
    Scene* scene;

};

#endif // MAIN_CONTROLLER_H
