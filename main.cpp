#include "arg.hpp"
#include "parser.hpp"

using namespace tappo;

template<bool F>
struct Foo {
  constexpr Foo() : flag(F) { };
  void print() { std::cout << flag << std::endl; }
  bool flag;
};

int main() {

  Arg arg1('p');
  Arg arg2(std::string_view("prefix"));
  Arg arg3('p', std::string_view("prefix"));
  Arg arg4(std::string_view("prefix"), 'p');
  arg4.set<Option::desc>("hello world");
  std::cout << arg4.desc() << std::endl;
  std::cout << arg3.index() << " " << arg3.alias() << std::endl;
  std::cout << arg4.index() << " " << arg4.alias() << std::endl;
  std::cout << arg4.short_option() << " "
            << arg4.long_option() << std::endl;

  ValueArg option('o', std::string_view("out"), 100);
  option.set<Option::value>(200.1);
  std::cout << option.value() << std::endl;
  option.set<Option::desc>("value hello world!");
  std::cout << option.desc() << std::endl;

  ValueArg op('o', tappo::placeholders::_, 100);
  std::cout << op.short_option() << std::endl;

  char x = 'c'; std::string_view sv = "string view";
  Arg arg5(x, sv);
  /*
  ValueArg option("test", std::string_view("hello"));
  std::cout << option.value() << std::endl;
  option.index();
  
  std::vector<std::shared_ptr<Arg> > container;
  container.emplace_back(std::make_shared<Arg>(option));
  */
  //Parser parser;
  //parser.add(option);
  //Foo<check_option("-tr")> foo;
  //foo.print();

  return 0;
}

