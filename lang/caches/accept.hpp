#include "deleter_t.hpp"

namespace lang {

template<>
struct cache_t<node_t::Accept>
{
  using key_t = std::pair<token_t, std::string>;

  struct hash_t {
    std::size_t operator()(const key_t& key) const
    {
      return std::hash<token_t>()(key.first) ^
             (std::hash<std::string>()(key.second) << 1);
    }
  };

  std::unordered_map<key_t, u_ptr, hash_t> cache;

  template<typename... Args>
  node_base* get_accept(const token_t token, const std::string& value, Args&&... args)
  {
    auto it = cache.find(key_t(token, value));
    if(it == cache.end())
    {
      return cache.emplace(key_t(token, value),
                           std::unique_ptr<node_base, deleter_t>(
                             new node<node_t::Accept>(token, value, args...),
                             deleter_t(node_t::Accept)))
                   .first->second.get();
    } else
    {
      return it->second.get();
    }
  }
};


}
