#include "grammatic_analyzer.h"

Grammatic_analyzer::Grammatic_analyzer()
{
  grammar = new Tree();

  // whole picture (axiom)
  Tree_node* picture = new Tree_node(POS_ABOVE);
  grammar->add_node(picture, NULL);

  // sun
  Tree_node* sun = new Tree_node(FIG_CIRCLE, LENGTH_HORIZONTAL, COMP_LESS, 1 / 10.0, LENGTH_VERTICAL, COMP_LESS, 1 / 10.0);
  grammar->add_node(sun, picture);

  // skyline and all beneath
  Tree_node* skyline_and_all_beneath = new Tree_node(POS_ABOVE);
  grammar->add_node(skyline_and_all_beneath, picture);

  // skyline
  Tree_node* skyline = new Tree_node(FIG_HZ_LINE, LENGTH_HORIZONTAL_FULL, 0, 1, 0, 0, 1);
  grammar->add_node(skyline, skyline_and_all_beneath);

  // house
  Tree_node* house = new Tree_node(POS_CONNECTED_BENEATH);
  grammar->add_node(house, skyline_and_all_beneath);

  // roof
  Tree_node* roof = new Tree_node(FIG_ROOF, LENGTH_HORIZONTAL, COMP_GREATER, 1 / 3.0, LENGTH_VERTICAL, COMP_GREATER, 1 / 5.0);
  grammar->add_node(roof, house);

  // house body
  Tree_node* body = new Tree_node(POS_INSIDE);
  grammar->add_node(body, house);

  // walls
  Tree_node* walls = new Tree_node(FIG_RECTANGLE, LENGTH_HORIZONTAL_FULL, 0, 1, LENGTH_VERTICAL_FULL, 0, 1);
  grammar->add_node(walls, body);

  // window_and_door
  Tree_node* window_and_door = new Tree_node(POS_LEFT);
  grammar->add_node(window_and_door, body);

  // window
  Tree_node* window = new Tree_node(FIG_RECTANGLE, LENGTH_HORIZONTAL, COMP_LESS, 1 / 5.0, LENGTH_VERTICAL, COMP_EQUAL, 1 / 5.0);
  grammar->add_node(window, window_and_door);

  // door
  Tree_node* door = new Tree_node(FIG_RECTANGLE, LENGTH_HORIZONTAL, COMP_LESS, 1 / 2.0, LENGTH_VERTICAL, COMP_EQUAL, 4 / 5.0);
  grammar->add_node(door, window_and_door);
}

Grammatic_analyzer::~Grammatic_analyzer()
{
  delete grammar;
}

bool Grammatic_analyzer::match_grammar(Scene* scene)
{
  //TODO: ascending bypass of grammar tree with all possible combination of
  //terminal elements
  return false;
}


void Grammatic_analyzer::build_match_scene(Scene* scene)
{
  draw_node(grammar->root, 0, 0, SCREEN_WIDTH - 10, SCREEN_HEIGTH - 10, scene);
}

Range Grammatic_analyzer::generate_range(int base, int wid, short comp_flag, float coeff)
{
  int desired_width;
  int comp_width = wid * coeff;
  switch ( comp_flag )
  {
    case COMP_GREATER:
      desired_width = rand() % (wid - comp_width) + comp_width; 
      break;
    case COMP_EQUAL:
      desired_width = comp_width;
      break;
    case COMP_LESS:
      desired_width = rand() % comp_width; 
      break;
    default:
      desired_width = rand() % (wid - 10) + 10;
      break;
  }
  int p1 = rand() % (wid - desired_width) + base;
  int p2 = p1 + desired_width;
  return Range(p1, p2);
}

Rect_range Grammatic_analyzer::generate_pos(Tree_node* node, int x, int y, int width, int height)
{
  Rect_range res;
  if ( node->length_h_descriptor == LENGTH_HORIZONTAL_FULL )
    res.xrange = Range(x, x+width);
  else
    if ( node->length_h_descriptor == LENGTH_HORIZONTAL )
      res.xrange = generate_range(x, width, node->length_h_comp, node->length_h_coeff);
    else
      res.xrange = generate_range(x, width, 0, 1);

  if ( node->length_v_descriptor == LENGTH_VERTICAL_FULL )
    res.yrange = Range(y, y+height);
  else
    if ( node->length_v_descriptor == LENGTH_VERTICAL )
      res.yrange = generate_range(y, height, node->length_v_comp, node->length_v_coeff);
    else
      res.yrange = generate_range(y, height, 0, 1);

  return res;
}

Rect_range Grammatic_analyzer::draw_terminal_node(Tree_node* node, int x, int y, int width, int height, Scene* scene)
{
  switch ( node->figure_type )
  {
    case FIG_HZ_LINE:
      {
        Rect_range coord = generate_pos(node, x, y, width, height);
        int p_x1 = coord.xrange.p1; int p_x2 = coord.xrange.p2;
        int p_y = coord.yrange.p1;

        fprintf(stderr, "HLine: %d %d %d\n", p_x1, p_x2, p_y);
        Line* line = new Line(p_x1, p_y, p_x2, p_y);
        scene->add_figure(line);
        return coord;
      }
      break;
    case FIG_RECTANGLE:
      {
        Rect_range coord = generate_pos(node, x, y, width, height);
        int p_x1 = coord.xrange.p1; int p_x2 = coord.xrange.p2;
        int p_y1 = coord.yrange.p1; int p_y2 = coord.yrange.p2;

        fprintf(stderr, "Rectangle: %d %d %d %d\n", p_x1, p_y1, p_x2, p_y2);
        Rectangle* rectangle = new Rectangle(p_x1, p_y1, p_x2, p_y2);
        scene->add_figure(rectangle);
        return coord;
      }
      break;
    case FIG_CIRCLE:
      {
        Rect_range coord;
        int dim = min(height, width);
        int d = rand() % dim;
        int p_x1 = rand() % (width - d) + x; int p_x2 = p_x1 + d;
        int p_y1 = rand() % (height - d) + y; int p_y2 = p_y1 + d;
        coord.xrange = Range(p_x1, p_x2);
        coord.yrange = Range(p_y1, p_y2);

        fprintf(stderr, "Ellipse: %d %d %d %d\n", p_x1, p_y1, p_x2, p_y2);
        Ellipse* ellipse = new Ellipse((p_x1 + p_x2) / 2, (p_y1 + p_y2) / 2, d / 2, d / 2);
        scene->add_figure(ellipse);
        return coord;
      }
      break;
    case FIG_ROOF:
      {
        Rect_range coord = generate_pos(node, x, y, width, height);
        int p_x1 = coord.xrange.p1; int p_x2 = coord.xrange.p2;
        int p_y1 = coord.yrange.p1; int p_y2 = coord.yrange.p2;

        fprintf(stderr, "Roof: %d %d %d %d\n", p_x1, p_y1, p_x2, p_y2);
        Line* line = new Line(p_x1, p_y2, (p_x1 + p_x2) / 2, p_y1);
        scene->add_figure(line);
        line = new Line((p_x1 + p_x2) / 2, p_y1, p_x2, p_y2);
        scene->add_figure(line);
        return coord;
      }
      break;
  }
}

Rect_range Grammatic_analyzer::draw_node(Tree_node* node, int x, int y, int width, int height, Scene* scene)
{
  Rect_range res;
  fprintf(stderr, "Drawing some node, %d %d %d %d\n", x, y, width, height);
  if ( node->figure_type != 0 )
  {
    res = draw_terminal_node(node, x, y, width, height, scene);
    return res;
  }

  node->left->length_h_descriptor |= node->length_h_descriptor;
  node->left->length_v_descriptor |= node->length_v_descriptor;
  node->right->length_h_descriptor |= node->length_h_descriptor;
  node->right->length_v_descriptor |= node->length_v_descriptor;
  switch ( node->pos_descriptor )
  {
    case POS_ABOVE:
      {
        res = draw_node(node->left, x, y, width, height / 5, scene);
        draw_node(node->right, x, res.yrange.p2, width, height - (res.yrange.p2 - y), scene);
      }
      break;
    case POS_LEFT:
      {
        res = draw_node(node->left, x, y, width / 2, height, scene);
        draw_node(node->right, res.xrange.p2, y, width - (res.xrange.p2 - x), height, scene);
      }
      break;
    case POS_INSIDE:
      {
        res = draw_node(node->left, x, y, width, height, scene);
        node->right->length_h_descriptor = 0;
        node->right->length_v_descriptor = 0;
        int c_width = abs(res.xrange.p2 - res.xrange.p1);
        int c_height = abs(res.yrange.p2 - res.yrange.p1);
        draw_node(node->right, res.xrange.p1, res.yrange.p1, c_width, c_height, scene);
      }
      break;
    case POS_CONNECTED_BENEATH:
      {
        res = draw_node(node->left, x, y, width, height / 3, scene);
        node->right->length_h_descriptor = LENGTH_HORIZONTAL_FULL;
        node->right->length_v_descriptor = LENGTH_VERTICAL_FULL;
        int c_width = abs(res.xrange.p2 - res.xrange.p1);
        draw_node(node->right, res.xrange.p1, res.yrange.p2, c_width, height - (res.yrange.p2 - y), scene);
      }
      break;
  }
}
