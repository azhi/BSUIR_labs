#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "models/figure.h"

using namespace std;

class Scene
{
  public:
    Scene();
    virtual ~Scene();
    void add_figure(Figure* figure);
    void set_working_surface(SDL_Surface* surface);
    void draw_all();
    template<class Archive>
      void serialize(Archive& ar, const unsigned int version)
      {
        ar & BOOST_SERIALIZATION_NVP(figures);
      }
    
  private:
    vector<Figure*>* figures;

};

#endif // SCENE_H
