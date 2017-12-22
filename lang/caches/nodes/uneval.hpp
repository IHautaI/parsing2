#ifndef UNEVAL_HPP
#define UNEVAL_HPP

#include "types/cache_t.hpp"
#include "types/token_t.hpp"

namespace lang {
  using wrt_t = std::pair<token_t, std::string>;


template<>
struct node<node_t::Uneval> : public node_base
{
  node_t left_type;
  node_base* left;
  wrt_t wrt;

  node(node_t left_type, node_base* left, const wrt_t& wrt)
  : node_base()
  , left_type(left_type)
  , left(left)
  , wrt(wrt)
  {}

  ~node(){}
};


}

#endif
