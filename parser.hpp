#ifndef TAPPO_PARSER_HPP_
#define TAPPO_PARSER_HPP_

#include "arg.hpp"

namespace tappo {

class Parser {
public:
  Parser() { }

#if 0
  template<typename VType>
  void add(ValueArg<VType>& option) {
    Index index(option.raw_index());
    //if (auto iter = container.find(index); iter != container.end())
    //  throw;
    //else
    container.emplace(index, std::make_shared<Arg>(option));
  }
#endif
  
private:
  std::map<Index, std::shared_ptr<ArgBase> > container;
};

}

#endif


