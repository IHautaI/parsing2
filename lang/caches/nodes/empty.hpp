#ifndef EMPTY_HPP
#define EMPTY_HPP

#include "types/cache_t.hpp"
#include "types/token_t.hpp"

namespace lang {

template<>
struct node<node_t::Null> : public node_base
{
  node()
  : node_base(2)
  {}

  ~node(){}
};


template<>
struct node<node_t::Empty> : public node_base
{
  node()
  : node_base(7)
  {}

  ~node(){}
};

}

#endif
