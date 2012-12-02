#ifndef GRAMMATIC_ANALYZER_H
#define GRAMMATIC_ANALYZER_H

#include "tree.h"
#include "scene.h"
#include "sdl_controller.h"

using namespace std;

struct Range
{
  int p1, p2;
  Range() : p1(0), p2(0) {};
  Range(int p1, int p2) : p1(p1), p2(p2) {};
};

struct Rect_range
{
  Range xrange, yrange;
};

class Grammatic_analyzer
{
  public:
    Grammatic_analyzer();
    virtual ~Grammatic_analyzer();

    bool match_grammar(Scene* scene);
    void build_match_scene(Scene* scene);

  private:
    Tree* grammar;

    Rect_range draw_node(Tree_node* node, int x, int y, int width, int height, Scene* scene);
    Range generate_range(int base, int wid, short comp_flag, float coeff);
    Rect_range generate_pos(Tree_node* node, int x, int y, int width, int height);
    Rect_range draw_terminal_node(Tree_node* node, int x, int y, int width, int height, Scene* scene);
};

#endif // GRAMMATIC_ANALYZER_H
