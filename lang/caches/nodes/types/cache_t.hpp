#ifndef CACHE_T_HPP
#define CACHE_T_HPP

#include <memory>
#include <unordered_map>
#include "node_base.hpp"

namespace lang {
struct deleter_t;

using u_ptr = std::unique_ptr<node_base, deleter_t>;

template<node_t Type>
struct cache_t;

}

#endif
