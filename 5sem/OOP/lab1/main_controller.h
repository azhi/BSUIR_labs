#ifndef MAIN_CONTROLLER_H
#define MAIN_CONTROLLER_H

#include "sdl_controller.h"
#include "scene.h"
#include <map>

using namespace std;

typedef Figure* (*maker_function)(vector<Point>* points);
typedef bool (*draw_mode_getter)();

class Main_controller
{
  public:
    Main_controller();
    virtual ~Main_controller();
    void main_loop();
		void add_figure_creator(maker_function maker, draw_mode_getter dw_get);

  private:
    SDL_controller* sdl_controller;
		vector<maker_function> factory;
    vector<draw_mode_getter> draw_modes;
    short draw_mode;
    Scene* scene;

};

#endif // MAIN_CONTROLLER_H
