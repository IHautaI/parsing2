#ifndef NODE_BASE_HPP
#define NODE_BASE_HPP

#include <unordered_map>
#include <utility>
#include <tuple>

#include "node_t.hpp"
#include "token_t.hpp"

namespace lang {

  struct node_base
  {
    int nullable;
    virtual ~node_base() = default;

    node_base(int nullable = 0)
    : nullable(nullable)
    {}
  };

  template<node_t type>
  struct node : public node_base {};
}

#endif
