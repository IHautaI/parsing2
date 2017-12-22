#include "deleter_t.hpp"
#include "accessors.hpp"

namespace lang {

wrt_t wrt(node_base* n)
{
  return dynamic_cast<node<node_t::Uneval>*>(n)->wrt;
}

template<>
struct cache_t<node_t::Uneval>
{
  using key_t = std::tuple<node_t, node_base*, token_t, std::string>;

  struct hash_t {
    std::size_t operator()(const key_t& key) const
    {
      return (std::hash<node_t>()(std::get<0>(key)) ^
             (std::hash<node_base*>()(std::get<1>(key)) << 1))
             ^
             (std::hash<token_t>()(std::get<2>(key)) ^
              (std::hash<std::string>()(std::get<3>(key)) << 1));
    }
  };

  std::unordered_map<key_t, u_ptr, hash_t> cache;

  template<typename... Args>
  node_base* get_uneval(node_t type, node_base* n, const wrt_t& wrt, Args&&... args)
  {
    auto it = cache.find(key_t(type, n, wrt.first, wrt.second));
    if( it == cache.end() )
    {
      return cache.emplace(key_t(type, n, wrt.first, wrt.second),
                           std::unique_ptr<node_base, deleter_t>(
                             new node<node_t::Uneval>(type, n, wrt, args...),
                             deleter_t(node_t::Uneval)))
                   .first->second.get();
    } else
    {
      return it->second.get();
    }
  }

  // void remove_uneval(node_base* n)
  // {
  //   auto m = dynamic_cast<node<node_t::Uneval>*>(n);
  //   cache.erase(key_t(m->left_type,
  //                     m->left,
  //                     m->wrt.first,
  //                     m->wrt.second));
  // }
};


}
