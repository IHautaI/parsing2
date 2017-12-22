#ifndef AND_HPP
#define AND_HPP

#include "types/cache_t.hpp"
#include "types/token_t.hpp"

namespace lang {

template<>
struct node<node_t::And> : public node_base
{
  node_t left_type;
  node_base* left;

  node_t right_type;
  node_base* right;

  node(const node_t& lt, node_base* left, const node_t& rt, node_base* right)
  : node_base()
  , left_type(lt)
  , left(left)
  , right_type(rt)
  , right(right)
  {}

  ~node(){}
};

}

#endif
