#include "lang/language.hpp"
using namespace lang;


auto deriv(Lang& l, std::initializer_list<wrt_t> lst)
{
  for( auto& x : lst )
  {
    std::cerr << "Derivative wrt " << x.second << " :" << "\n";
    l.delta(x);
    std::cerr << "\nNullable: " << l.nullable()
              << "\nAccepted: " << l.accepted() << "\n\n";
    if( l.nullable() && l.accepted() )
    {
      l.print();
    }
    // l.print();

    // std::cerr << "\nNullable: " << null
    //           << "\nAccepted: " << l.accepted() << "\n\n";
  }
}


int main(int argc, char* argv[])
{
  auto open = std::make_pair(0, "(");
  auto close = std::make_pair(1, ")");

  lang::Lang l;

  // tokens
  auto lp = l.cache.get_token(0);
  auto rp = l.cache.get_token(1);
  auto eps = l.cache.get_empty();

  // setup head
  l.head_type = node_t::Or;
  l.head = l.cache.get_or(node_t::Null, l.cache.get_null(),
                          node_t::Empty, l.cache.get_empty());

  // substructure
  auto rght = l.cache.get_and(node_t::Or, l.head,
                               node_t::Token, rp);

  auto mid = l.cache.get_and(node_t::Token, lp,
                             node_t::And, rght);

  auto lft = l.cache.get_and(node_t::Or, l.head,
                              node_t::And, mid);

  // recursive on the left
  left<node_t::Or>(l.head) = lft;
  left_type<node_t::Or>(l.head) = node_t::And;

  l.reset();
  std::cerr << "\n";
  l.print();
  std::cerr << "\nNullable: " << l.nullable()
            << "\nAccepted: " << l.accepted()
            << "\n\n";
  // deriv(l, {open, close});
  // deriv(l, {open, open, close, close});
     deriv(l, {open, close, open, close});
  // deriv(l, {open, open, close, open, close, close});
  std::cerr << "size: "
            << l.cache.size()
            << "\n";

  return 0;
}
