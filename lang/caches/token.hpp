#include "deleter_t.hpp"

namespace lang {

  template<>
  struct cache_t<node_t::Token>
  {
    std::unordered_map<token_t, u_ptr> cache;

    template<typename... Args>
    node_base* get_token(const token_t& token, Args&&... args)
    {
      auto it = cache.find(token);

      if( it == cache.end() )
      {
        return cache.emplace(token, std::unique_ptr<node_base, deleter_t>(
                                      new node<node_t::Token>(token, args...),
                                      deleter_t(node_t::Token)))
                    .first->second.get();
      } else
      {
        return it->second.get();
      }
    }
  };

}
