#ifndef MAIN_CONTROLLER_H
#define MAIN_CONTROLLER_H

#include "sdl_controller.h"
#include "scene.h"

using namespace std;

class Main_controller
{
  public:
    Main_controller();
    virtual ~Main_controller();
    void main_loop();

  private:
    SDL_controller* sdl_controller;
    Scene* scene;

};

#endif // MAIN_CONTROLLER_H
