#ifndef ACCEPT_HPP
#define ACCEPT_HPP

#include "types/cache_t.hpp"
#include "types/token_t.hpp"

namespace lang {

template<>
struct node<node_t::Accept> : public node_base
{
  token_t value;
  std::string str;

  node(const token_t& value, const std::string& str)
  : node_base(7)
  , value(value)
  , str(str)
  {}

  ~node(){}
};

}

#endif
