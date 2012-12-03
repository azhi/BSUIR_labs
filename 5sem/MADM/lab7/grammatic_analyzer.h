#ifndef GRAMMATIC_ANALYZER_H
#define GRAMMATIC_ANALYZER_H

#include "tree.h"
#include "scene.h"
#include "sdl_controller.h"

using namespace std;

struct Terminal_element_combination
{
  int count;
  int count_in_each;
  int** data;

  Terminal_element_combination() : count(0), count_in_each(0), data(NULL) {};
  Terminal_element_combination(int count, int count_in_each) : count(count), count_in_each(count_in_each)
  {
    data = (int**) malloc( sizeof(int*) * count );
    for ( int i = 0; i < count; ++i )
      data[i] = (int*) malloc ( sizeof(int) * count_in_each );
  };

  void reserve(int count, int count_in_each)
  {
    if ( data )
    {
      for ( int i = 0; i < this->count; ++i )
        free(data[i]);
      free(data);
    }

    this->count = count;
    this->count_in_each = count_in_each;
    data = (int**) malloc( sizeof(int*) * count );
    for ( int i = 0; i < count; ++i )
      data[i] = (int*) malloc ( sizeof(int) * count_in_each );
  };

  ~Terminal_element_combination()
  {
    if ( data )
    {
      for ( int i = 0; i < this->count; ++i )
        free(data[i]);
      free(data);
    }
  }
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

    Terminal_element_combination* build_match_combinations(Tree_node* node, vector<Figure*>* figures);
    void find_figures_by_type(vector<Figure*>* figures, short figure_type, Terminal_element_combination* res);
    bool check_for_descriptor(short pos_descriptor, int* left_comb, int left_count, int* right_comb, int right_count, vector<Figure*>* figures);

    Rect_range draw_node(Tree_node* node, int x, int y, int width, int height, Scene* scene);
    Range generate_range(int base, int wid, short comp_flag, float coeff);
    Rect_range generate_pos(Tree_node* node, int x, int y, int width, int height);
    Rect_range draw_terminal_node(Tree_node* node, int x, int y, int width, int height, Scene* scene);
};

#endif // GRAMMATIC_ANALYZER_H
