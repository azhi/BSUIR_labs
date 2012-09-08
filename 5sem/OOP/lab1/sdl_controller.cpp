#include "sdl_controller.h"

SDL_controller::SDL_controller()
{
  init();
}

SDL_controller::~SDL_controller()
{
  SDL_Quit();
}

void SDL_controller::init()
{
  if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_EVENTTHREAD ) < 0 )
  {
    fprintf(stderr, "Unable to init SDL: %s", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGTH, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
  if ( !screen )
  {
    fprintf(stderr, "Unable to set videomode: %s", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  clear();
  redraw();
}

SDL_Event SDL_controller::get_event()
{
  SDL_Event event;
  SDL_PollEvent(&event);
  return event;
}

SDL_Surface* SDL_controller::get_surface()
{
  return screen;
}

void SDL_controller::clear()
{
  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
}

void SDL_controller::redraw()
{
  SDL_Flip(screen);
}

bool SDL_controller::check_ranges(int x, int y)
{
  if ( x < 0 || x >= SCREEN_WIDTH ||
      y < 0 || y >= SCREEN_HEIGTH )
    return false;
  return true;
}
