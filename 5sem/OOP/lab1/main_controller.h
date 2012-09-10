#ifndef MAIN_CONTROLLER_H
#define MAIN_CONTROLLER_H

#include "sdl_controller.h"
#include "scene.h"

using namespace std;

#define MO_LINE 1
#define MO_RECTANGLE 2
#define MO_ELLIPSE 3

class Main_controller
{
  public:
    Main_controller();
    virtual ~Main_controller();
    void main_loop();

  private:
    SDL_controller* sdl_controller;
    short draw_mode;
    Scene* scene;

};

#endif // MAIN_CONTROLLER_H
