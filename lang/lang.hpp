#include "cache.hpp"

#include <iostream>

namespace lang {
  /*
  * Node Flags are:
  * 1 - nullable, 2 - visited, 4 - accepted 8 - evaluated
  */

  bool nullable(node_t& type, node_base*& n, Cache& cache);

  template<node_t T>
  bool clean_null(node_t& type, node_base*& n, Cache& cache);

  template<>
  bool clean_null<node_t::Or>(node_t& type, node_base*& n, Cache& cache)
  {
    auto m = dynamic_cast<node<node_t::Or>*>(n);

    auto null = nullable(m->left_type, m->left, cache) ||
                nullable(m->right_type, m->right, cache);

    // nullable forced evaluation
    // now we can clean up
    if( m->left_type == node_t::Null || m->right_type == node_t::Null )
    {
      auto lt = m->left_type, rt = m->right_type;
      auto l = m->left, r = m->right;

      if( lt == node_t::Null && rt == node_t::Null )
      {
        type = node_t::Null;
        n = cache.get_null();
      } else if( lt == node_t::Null )
      {
        type = rt;
        n = r;
      } else
      {
        type = lt;
        n = l;
      }
    } else if( accepted(m->left) ||
               accepted(m->right) )
    {
      accept(n);
    }
    // for Or - if both accept, Or is marked accept
    // and we skip it in derivatives.  If one side is marked
    // but not the other, the other is used in derivatives.

    return null;
  }


  template<>
  bool clean_null<node_t::And>(node_t& type, node_base*& n, Cache& cache)
  {
    auto m = dynamic_cast<node<node_t::And>*>(n);
    auto null = nullable(m->left_type, m->left, cache) &&
                nullable(m->right_type, m->right, cache);


    if( m->left_type == node_t::Null || m->right_type == node_t::Null )
    {
      type = node_t::Null;
      n = cache.get_null();
    } else if( accepted(m->left) && accepted(m->right) )
    {
      accept(n);
    }

    return null;
  }


  node_base* eval(node_t& type, node_base* n, Cache& cache);

  bool nullable(node_t& type, node_base*& n, Cache& cache)
  {
    if( visited(n) )
    {
      return nullable(n);
    } else
    {
      visit(n);

      bool null;

      switch( type )
      {
        case node_t::Or:
          null = clean_null<node_t::Or>(type, n, cache);
          break;

        case node_t::And:
          null = clean_null<node_t::And>(type, n, cache);
          break;

        case node_t::Uneval:
          n = eval(type, n, cache);
          null = nullable(type, n, cache);
          break;

        default:
        std::cerr << "Bad node type in nullable switch, type: "
                  << static_cast<int>(type) << " "
                  << n << std::endl;
          exit(1);
          break;
      }

      if( type == node_t::And || type == node_t::Or )
      {
        unvisit(n);
      }

      return null;
    }
  }

  using deriv_t = std::pair<node_t, node_base*>;

  template<node_t type>
  deriv_t derivative(node<type>& n, const wrt_t& wrt, Cache& cache);

  template<>
  deriv_t derivative(node<node_t::Token>& n, const wrt_t& wrt, Cache& cache);

  template<>
  deriv_t derivative(node<node_t::And>& n, const wrt_t& wrt, Cache& cache);

  template<>
  deriv_t derivative(node<node_t::Or>& n, const wrt_t& wrt, Cache& cache);

  template<>
  deriv_t derivative(node<node_t::Star>& n, const wrt_t& wrt, Cache& cache);


  deriv_t derivative(node_t& type, node_base*& n, const wrt_t& wrt, Cache& cache)
  {
    switch( type )
    {
      case node_t::Uneval:
        n = eval(type, n, cache);
        return std::make_pair(type, n);
        break;

      case node_t::Null:
        return std::make_pair(type, n);
        break;

      case node_t::Accept:
      case node_t::Empty:
        return std::make_pair(node_t::Null, cache.get_null());
        break;

      case node_t::Token:
        return derivative(*dynamic_cast<node<node_t::Token>*>(n), wrt, cache);
        break;

      case node_t::Or:
      case node_t::And:
      case node_t::Star:
        return std::make_pair(node_t::Uneval, cache.get_uneval(type, n, wrt));
        break;
    }
  }


  template<>
  deriv_t derivative(node<node_t::Token>& n, const wrt_t& wrt, Cache& cache)
  {
    if( n.value == wrt.first )
    {
      // return std::make_pair(node_t::Accept, cache.get_accept(wrt.first, wrt.second));
      auto acc = cache.get_accept(wrt.first, wrt.second);
      return std::make_pair(node_t::And,
                            cache.get_and(node_t::Accept, acc,
                                          node_t::Empty, cache.get_empty()));
    } else
    {
      return std::make_pair(node_t::Null, cache.get_null());
    }
  }


  /*
  * d: A | B ->  d(A) | d(B)
  */
  template<>
  deriv_t derivative(node<node_t::Or>& n, const wrt_t& wrt, Cache& cache)
  {
    auto r = cache.get_uneval(n.right_type, n.right, wrt);
    auto l = cache.get_uneval(n.left_type, n.left, wrt);

    return std::make_pair(node_t::Or, cache.get_or(node_t::Uneval, l,
                                                   node_t::Uneval, r));
  }

  /*
  * d: A & B -> A nullable? D(A) & B | d(B) : d(A) & B
  * Accepted left is caught by nullable check -
  * we'll have NULL & B | d(B) -> A & d(B)
  */
  template<>
  deriv_t derivative(node<node_t::And>& n, const wrt_t& wrt, Cache& cache)
  {
    auto dA = cache.get_uneval(n.left_type, n.left, wrt);
    auto dB = cache.get_uneval(n.right_type, n.right, wrt);

    auto dAB = cache.get_and(node_t::Uneval, dA, n.right_type, n.right);

    if( nullable(n.left_type, n.left, cache) )
    {
      auto dABordB = cache.get_or(node_t::And, dAB, node_t::Uneval, dB);

      if( accepted(n.left) ) // d(A) & B | d(B) | A & d(B)
      {
        auto AdB = cache.get_and(n.left_type, n.left, node_t::Uneval, dB);

        return std::make_pair(node_t::Or,
                              cache.get_or(node_t::Or, dABordB,
                                           node_t::And, AdB));
      }

      // d(A) & B | d(B)
      return std::make_pair(node_t::Or, dABordB);
    }

    // d(A) & B
    return std::make_pair(node_t::And, dAB);
  }


  // d(Left) & n
  template<>
  deriv_t derivative(node<node_t::Star>& n, const wrt_t& wrt, Cache& cache)
  {
    auto l = cache.get_uneval(n.left_type, n.left, wrt);
    return std::make_pair(node_t::And,
                          cache.get_and(node_t::Uneval, l,
                                        node_t::Star, &n));
  }


  // replaces Uneval with evaluated derivative
  node_base* eval(node_t& type, node_base* n, Cache& cache)
  {
    if( evaluated(n) )
    {
      type = left_type<node_t::Uneval>(n);
      return left<node_t::Uneval>(n);
    }

    auto m = dynamic_cast<node<node_t::Uneval>*>(n);

    deriv_t der;
    switch( m->left_type )
    {
      case node_t::Or:
        der = derivative(*dynamic_cast<node<node_t::Or>*>(m->left), m->wrt, cache);
        break;

      case node_t::And:
        der = derivative(*dynamic_cast<node<node_t::And>*>(m->left), m->wrt, cache);
        break;

      case node_t::Star:
        der = derivative(*dynamic_cast<node<node_t::Star>*>(m->left), m->wrt, cache);
        break;

      default:
        der = derivative(m->left_type, m->left, m->wrt, cache);
        break;
    }

    type = der.first;

    m->left_type = der.first;
    m->left = der.second;

    evaluate(n);

    return der.second;
  }

}
