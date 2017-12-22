#include "deleter_t.hpp"

namespace lang {

bool visited(node_base* n)
{
  return n->nullable & 2;
}

void visit(node_base* n)
{
  n->nullable |= 2;
}

void unvisit(node_base* n)
{
  n->nullable &= ~2;
}


bool nullable(node_base* n)
{
  return n->nullable & 1;
}


template<node_t T>
node_t& left_type(node_base* n)
{
  return dynamic_cast<node<T>*>(n)->left_type;
}


template<node_t T>
node_base*& left(node_base* n)
{
  return dynamic_cast<node<T>*>(n)->left;
}


template<node_t T>
node_t& right_type(node_base* n)
{
  return dynamic_cast<node<T>*>(n)->right_type;
}


template<node_t T>
node_base*& right(node_base* n)
{
  return dynamic_cast<node<T>*>(n)->right;
}


bool accepted(node_base* n)
{
  return n->nullable & 4;
}

void accept(node_base* n)
{
  n->nullable |= 4;
}


bool evaluated(node_base* n)
{
  return n->nullable & 8;
}


void evaluate(node_base* n)
{
  n->nullable |= 8;
}

}
