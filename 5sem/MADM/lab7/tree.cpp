#include "tree.h"

Tree::Tree() : root(NULL) {}

Tree::~Tree()
{
  clear();
}

void Tree::clear()
{
  if ( !root )
    delete root;
  root = NULL;
}

void Tree::add_node(Tree_node* node, Tree_node* parent)
{
  if ( !parent )
  {
    clear();
    root = node;
    return;
  }

  if ( !parent->left )
  {
    parent->left = node;
    return;
  }
  parent->right = node;
}

void destroy_node(Tree_node* node)
{
  delete node;
}
