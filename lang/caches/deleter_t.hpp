#ifndef DELETER_T_HPP
#define DELETER_T_HPP

#include "nodes/empty.hpp"
#include "nodes/token.hpp"
#include "nodes/accept.hpp"
#include "nodes/or.hpp"
#include "nodes/and.hpp"
#include "nodes/star.hpp"
#include "nodes/uneval.hpp"

namespace lang {

struct deleter_t
{
  node_t type;

  void operator()(node_base* x)
  {
    switch (type)
    {
      case node_t::Null:
        delete dynamic_cast<node<node_t::Null>*>(x);
        break;
      case node_t::Empty:
        delete dynamic_cast<node<node_t::Empty>*>(x);
        break;
      case node_t::Token:
        delete dynamic_cast<node<node_t::Token>*>(x);
        break;
      case node_t::Accept:
        delete dynamic_cast<node<node_t::Accept>*>(x);
        break;
      case node_t::And:
        delete dynamic_cast<node<node_t::And>*>(x);
        break;
      case node_t::Or:
        delete dynamic_cast<node<node_t::Or>*>(x);
        break;
      case node_t::Star:
        delete dynamic_cast<node<node_t::Star>*>(x);
        break;
      case node_t::Uneval:
        delete dynamic_cast<node<node_t::Uneval>*>(x);
    }
  }

  deleter_t(node_t type)
  : type(type)
  {}
};

}


#endif
