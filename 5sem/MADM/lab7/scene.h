#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "models/figure.h"
#include "models/line.h"
#include "models/rectangle.h"
#include "models/ellipse.h"

using namespace std;

class Scene
{
  public:
    Scene();
    virtual ~Scene();
    void add_figure(Figure* figure);
    void set_working_surface(SDL_Surface* surface);
    void draw_all();
    void clear();
    size_t size() { return figures->size(); };

    vector<Figure*>* get_figures() { return figures; };

  private:
    vector<Figure*>* figures;

};

#endif // SCENE_H
