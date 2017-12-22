#include "deleter_t.hpp"

namespace lang {

  template<>
  struct cache_t<node_t::Star>
  {
    using key_t = std::pair<node_t, node_base*>;

    struct hash_t {
      std::size_t operator()(const key_t& key) const
      {
        return std::hash<node_t>()(key.first) ^
               (std::hash<node_base*>()(key.second) << 1);
      }
    };


    std::unordered_map<key_t, u_ptr, hash_t> cache;

    node_base* get_star(const node_t& type, node_base* left)
    {
      auto it = cache.find(std::make_pair(type, left));
      if( it != cache.end() )
      {
        return cache.emplace(std::make_pair(type, left),
                             std::unique_ptr<node_base, deleter_t>(
                                      new node<node_t::Star>(type, left),
                                      deleter_t(node_t::Star)))
                    .first->second.get();
      } else
      {
        return it->second.get();
      }
    }
  };

}
