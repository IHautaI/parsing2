#include "deleter_t.hpp"

namespace lang {

  template<>
  struct cache_t<node_t::Null>
  {
    u_ptr cache;

    node_base* get_null()
    {
      return cache.get();
    }

    cache_t()
    : cache(new node<node_t::Null>(), deleter_t(node_t::Null))
    {}

    ~cache_t(){}
  };

  template<>
  struct cache_t<node_t::Empty>
  {
    u_ptr cache;

    node_base* get_empty()
    {
      return cache.get();
    }

    cache_t()
    : cache(new node<node_t::Empty>(), deleter_t(node_t::Empty))
    {}

    ~cache_t(){}
  };

}
