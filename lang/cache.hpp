#ifndef CACHE_HPP
#define CACHE_HPP

#include "caches/empty.hpp"
#include "caches/token.hpp"
#include "caches/accept.hpp"
#include "caches/or.hpp"
#include "caches/and.hpp"
#include "caches/star.hpp"
#include "caches/uneval.hpp"
#include "caches/deleter_t.hpp"

namespace lang {

struct Cache : public cache_t<node_t::Null>, cache_t<node_t::Empty>,
                      cache_t<node_t::Token>, cache_t<node_t::Accept>,
                      cache_t<node_t::Or>, cache_t<node_t::And>,
                      cache_t<node_t::Star>, cache_t<node_t::Uneval>
{
  Cache()
  {}

  ~Cache()
  {}

  void clear()
  {
    cache_t<node_t::Token>::cache.clear();
    cache_t<node_t::Accept>::cache.clear();
    cache_t<node_t::Or>::cache.clear();
    cache_t<node_t::And>::cache.clear();
    cache_t<node_t::Star>::cache.clear();
    cache_t<node_t::Uneval>::cache.clear();
  }

  size_t size()
  {
    return 2 + cache_t<node_t::Token>::cache.size()
             + cache_t<node_t::Accept>::cache.size()
             + cache_t<node_t::Or>::cache.size()
             + cache_t<node_t::And>::cache.size()
             + cache_t<node_t::Star>::cache.size()
             + cache_t<node_t::Uneval>::cache.size();
  }
};

}

#endif
