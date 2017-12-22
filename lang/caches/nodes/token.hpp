#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "types/cache_t.hpp"
#include "types/token_t.hpp"

namespace lang {

template<>
struct node<node_t::Token> : public node_base
{
  token_t value;

  node(const token_t& value)
  : node_base(2)
  , value(value)
  {}

  ~node(){}
};

}

#endif
