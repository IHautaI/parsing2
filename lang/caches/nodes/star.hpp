#ifndef STAR_HPP
#define STAR_HPP

#include "types/cache_t.hpp"
#include "types/token_t.hpp"

namespace lang {


template<>
struct node<node_t::Star> : public node_base
{
  node_t left_type;
  node_base* left;

  node(const node_t& lt, node_base* left)
  : node_base(3)
  , left_type(lt)
  , left(left)
  {}

  ~node(){}
};

}


#endif
