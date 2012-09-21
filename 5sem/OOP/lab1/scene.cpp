#include "scene.h"
#include <stdio.h>
#include <fstream>

Scene::Scene()
{
  figures = new vector<Figure*>;
}

Scene::~Scene()
{
  delete figures;
}

void Scene::add_figure(Figure* figure)
{
  figures->push_back(figure);
}

void Scene::set_working_surface(SDL_Surface* surface)
{
  vector<Figure*>::iterator cur_figure;
  for ( cur_figure = figures->begin(); cur_figure != figures->end(); ++cur_figure )
    (*cur_figure)->set_surface(surface);
}

void Scene::draw_all()
{
  vector<Figure*>::iterator cur_figure;
  for ( cur_figure = figures->begin(); cur_figure != figures->end(); ++cur_figure )
    (*cur_figure)->draw();
}

void Scene::save_to_file(const char* path)
{
  std::ofstream ofs(path, std::ios::out);
  vector<Figure*>::iterator cur_figure;
  for ( cur_figure = figures->begin(); cur_figure != figures->end(); ++cur_figure )
    ofs << (*cur_figure)->get_serialization_str() << std::endl;
  ofs.close();
}

void Scene::load_from_file(const char* path)
{
  std::ifstream ifs(path, std::ios::in);
  delete figures;
  figures = new vector<Figure*>;
  while ( !ifs.eof() )
  {
    std::string ser_str;
    ifs >> ser_str;
  }
}
