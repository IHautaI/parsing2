#include "lang.hpp"
#include <vector>
#include <algorithm>
#include <sstream>

namespace lang {
  using print_t = std::vector<std::string>;
  const char FILL = '-';
  struct Lang
  {
    Cache cache;

    node_base* head;
    node_t head_type;

    node_base* current;
    node_t current_type;

    auto delta(const wrt_t& wrt)
    {
      std::tie(current_type, current) = derivative(current_type, current,
                                                   wrt, cache);
    }

    void reset()
    {
      current_type = head_type;
      current = head;
    }

    void clear()
    {
      cache.clear();
    }

    bool nullable()
    {
      return ::lang::nullable(current_type, current, cache);
    }

    bool accepted()
    {
      return nullable() ? ::lang::accepted(current) : false;
    }

    Lang()
    : cache()
    , head(nullptr)
    , current(nullptr)
    {}

    ~Lang(){}


    template<node_t T>
    print_t print(node_base* n, std::vector<node_base*>& prev);

    print_t print(node_t type, node_base* current, std::vector<node_base*>& prev);


    void print()
    {
      auto vec = std::vector<node_base*>();
      auto out = print(current_type, current, vec);

      for(auto& x : out)
      {
        std::cerr << x << "\n";
      }
    }
  };

  template<>
  print_t Lang::print<node_t::Accept>(node_base* n, std::vector<node_base*>& prev)
  {
    auto m = dynamic_cast<node<node_t::Accept>*>(n);
    return print_t{"{" + m->str + "}"};
  }

  template<>
  print_t Lang::print<node_t::Token>(node_base* n, std::vector<node_base*>& prev)
  {
    auto m = dynamic_cast<node<node_t::Token>*>(n);
    return print_t{"[" + std::to_string(m->value) + "]"};
  }

  template<>
  print_t Lang::print<node_t::Or>(node_base* n, std::vector<node_base*>& prev)
  {
    auto it = std::find(prev.begin(), prev.end(), n);

    if( it == prev.end() )
    {
      prev.push_back(n);
      auto k = std::to_string(prev.size() - 1);
      auto m = dynamic_cast<node<node_t::Or>*>(n);

      auto block1 = print(m->left_type, m->left, prev);
      auto block2 = print(m->right_type, m->right, prev);

      auto kp = 3 + k.length();
      auto t = block1[0].length() + block2[0].length();
      auto pos = block1[0].length();

      auto lpad = pos - block1[0].length()/2;
      auto lblock = pos - lpad;

      auto rblock = block2[0].length()/2;
      auto rpad = t - pos - rblock;

      auto out = print_t{std::string(lpad, ' ') + std::string(lblock, '-')
                         +"(|" + k + ")"
                         + std::string(rblock, '-') + std::string(rpad, ' ')};

      out.resize(std::max(block1.size(), block2.size()) + 1);

      for( auto i = 0; i < std::max(block1.size(), block2.size()); ++i )
      {
        if( i < block1.size() )
        {
          out[i + 1] = block1[i];
        } else
        {
          out[i + 1] = std::string(block1[0].size(), ' ');
        }

        if( i < block2.size() )
        {
          out[i + 1] += std::string(kp, ' ') + block2[i];
        } else
        {
          out[i + 1] += std::string(block2[0].size() + kp, ' ');
        }
      }

      return out;
    } else
    {
      return print_t{"<" + std::to_string(it - prev.begin()) + ">"};
    }
  }

  template<>
  print_t Lang::print<node_t::And>(node_base* n, std::vector<node_base*>& prev)
  {
    auto it = std::find(prev.begin(), prev.end(), n);

    if( it == prev.end() )
    {
      prev.push_back(n);
      auto k = std::to_string(prev.size() - 1);

      auto m = dynamic_cast<node<node_t::And>*>(n);

      auto block1 = print(m->left_type, m->left, prev);
      auto block2 = print(m->right_type, m->right, prev);

      auto kp = 3 + k.length();
      auto t = block1[0].length() + block2[0].length();
      auto pos = block1[0].length();

      auto lpad = pos - block1[0].length()/2;
      auto lblock = pos - lpad;

      auto rblock = block2[0].length()/2;
      auto rpad = t - pos - rblock;

      auto out = print_t{std::string(lpad, ' ') + std::string(lblock, '-')
                         + "(&" + k + ")"
                         + std::string(rblock, '-') + std::string(rpad, ' ')};

      out.resize(std::max(block1.size(), block2.size()) + 1);

      for( auto i = 0; i < std::max(block1.size(), block2.size()); ++i )
      {
        if( i < block1.size() )
        {
          out[i + 1] = block1[i];
        } else
        {
          out[i + 1] = std::string(block1[0].size(), ' ');
        }

        if( i < block2.size() )
        {
          out[i + 1] += std::string(kp, ' ') + block2[i];
        } else
        {
          out[i + 1] += std::string(block2[0].size() + kp, ' ');
        }
      }

      return out;
    } else
    {
      return print_t{"<" + std::to_string(it - prev.begin()) + ">"};
    }
  }

  template<>
  print_t Lang::print<node_t::Star>(node_base* n, std::vector<node_base*>& prev)
  {
    auto m = dynamic_cast<node<node_t::Star>*>(n);

    auto block = print(m->left_type, m->left, prev);
    auto out = print_t{"(*" + std::string(block[0].length(), FILL) + ")"};

    for( auto& x : block )
    {
      out.push_back(std::string(2, FILL) + x + FILL);
    }

    return out;
  }

  template<>
  print_t Lang::print<node_t::Uneval>(node_base* n, std::vector<node_base*>& prev)
  {
    auto m = dynamic_cast<node<node_t::Uneval>*>(n);
    auto it = std::find(prev.begin(), prev.end(), m->left);

    std::stringstream ss;
    ss << "D{" << m->wrt.first << "}" << "(<" << it - prev.begin() << ">)";
    return print_t{ss.str()};
  }

  template<>
  print_t Lang::print<node_t::Empty>(node_base* n, std::vector<node_base*>& prev)
  {
    return print_t{"[EMPTY]"};
  }

  template<>
  print_t Lang::print<node_t::Null>(node_base* n, std::vector<node_base*>& prev)
  {
    return print_t{"[NULL]"};
  }


  print_t Lang::print(node_t type, node_base* current, std::vector<node_base*>& prev)
  {
    switch(type)
    {
      case node_t::Null:
        return print<node_t::Null>(current, prev);
        break;
      case node_t::Empty:
        return print<node_t::Empty>(current, prev);
        break;
      case node_t::Accept:
        return print<node_t::Accept>(current, prev);
        break;
      case node_t::Token:
        return print<node_t::Token>(current, prev);
        break;
      case node_t::Or:
        return print<node_t::Or>(current, prev);
        break;
      case node_t::And:
        return print<node_t::And>(current, prev);
        break;
      case node_t::Star:
        return print<node_t::Star>(current, prev);
        break;
      case node_t::Uneval:
        return print<node_t::Uneval>(current, prev);
        break;
    }
  }

}
