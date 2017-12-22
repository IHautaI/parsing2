#include "deleter_t.hpp"


namespace lang {

template<>
struct cache_t<node_t::Or>
{
  using key_t = std::tuple<node_t, node_base*, node_t, node_base*>;

  struct hash_t {
    std::size_t operator()(const key_t& key) const
    {
      return (std::hash<node_t>()(std::get<0>(key)) ^
             (std::hash<node_base*>()(std::get<1>(key)) << 1))
             ^
             (std::hash<node_t>()(std::get<2>(key)) ^
             (std::hash<node_base*>()(std::get<3>(key)) << 2));
    }
  };

  std::unordered_map<key_t, u_ptr, hash_t> cache;

  template<typename... Args>
  node_base* get_or(const node_t& lt, node_base* left,
                    const node_t& rt, node_base* right, Args&&... args)
  {
    auto it = cache.find(std::make_tuple(lt, left, rt, right));
    if( it == cache.end() )
    {
      return cache.emplace(std::make_tuple(lt, left, rt, right),
                           std::unique_ptr<node_base, deleter_t>(
                             new node<node_t::Or>(lt, left, rt, right, args...),
                             deleter_t(node_t::Or)))
                   .first->second.get();
    } else
    {
      return it->second.get();
    }
  }

  // void remove_or(node_base* n)
  // {
  //   auto m = dynamic_cast<node<node_t::Or>*>(n);
  //   cache.erase(key_t(m->left_type,
  //                     m->left,
  //                     m->right_type,
  //                     m->right));
  // }

};

}
