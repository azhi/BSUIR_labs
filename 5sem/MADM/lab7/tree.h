#ifndef TREE_H
#define TREE_H

#include "scene.h"

using namespace std;

// position descriptors
#define POS_ABOVE 1
#define POS_LEFT 2
#define POS_INSIDE 3
#define POS_CONNECTED_BENEATH 4

// length descriptors
#define LENGTH_HORIZONTAL 1
#define LENGTH_HORIZONTAL_FULL 2
#define LENGTH_VERTICAL 3
#define LENGTH_VERTICAL_FULL 4

// length comparision direction
#define COMP_GREATER 1
#define COMP_LESS 2
#define COMP_EQUAL 3

// terminal elements
#define FIG_HZ_LINE 1
#define FIG_RECTANGLE 2
#define FIG_CIRCLE 3
#define FIG_ROOF 4

struct Tree_node
{
  Tree_node* left;
  Tree_node* right;

  short pos_descriptor;

  short length_h_descriptor;
  short length_h_comp;
  float length_h_coeff;
  short length_v_descriptor;
  short length_v_comp;
  float length_v_coeff;

  short figure_type;
  Tree_node(short pos_d) : left(NULL), right(NULL), pos_descriptor(pos_d),
                    length_h_descriptor(0), length_h_comp(0), length_h_coeff(1),
                    length_v_descriptor(0), length_v_comp(0), length_v_coeff(1),
                                                        figure_type(0) {};

  Tree_node(short fig_type, short len_h_d, short len_h_c, float len_h,
            short len_v_d, short len_v_c, float len_v) :
                                  left(NULL), right(NULL), pos_descriptor(0),
 length_h_descriptor(len_h_d), length_h_comp(len_h_c), length_h_coeff(len_h),
 length_v_descriptor(len_v_d), length_v_comp(len_v_c), length_v_coeff(len_v),
                                                    figure_type(fig_type) {};

  ~Tree_node()
  {
    if (!left) delete left;
    if (!right) delete right;
  };
};

class Tree
{
  public:
    Tree();
    virtual ~Tree();

    void clear();
    void add_node(Tree_node* node, Tree_node* parent);
    void destroy_node(Tree_node* node);

    Tree_node* root;
};

#endif // TREE_H
