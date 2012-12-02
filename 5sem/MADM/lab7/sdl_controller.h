#ifndef SDL_CONTROLLER_H
#define SDL_CONTROLLER_H

#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

using namespace std;

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGTH 768

class SDL_controller
{
  public:
    SDL_controller();
    virtual ~SDL_controller();

    void init();
    // bool check_ranges(int x, int y);
    SDL_Event get_event();
    SDL_Surface* get_surface();

    void clear();
    void redraw();

  private:
    SDL_Surface* screen;

};

#endif // SDL_CONTROLLER_H
