#ifndef TAPPO_ARG_HPP_
#define TAPPO_ARG_HPP_

#include <iostream>
#include <memory>
#include <variant>
#include <any>
#include <optional>
#include <type_traits>
#include <string>
#include <vector>
#include <map>

namespace tappo {

using Index = std::variant<char, std::string_view>;

/**
 * ENUM CLASS Option.
 * This class simply groups the attributes of an argument into several
 * categories.
 *
 * @desc  Description of an argument, available in all kinds of arguments.
 * @value Value of an argument, only available in arguments with values.
 */
enum class Option {
  desc, value
};

/**
 * Placeholders.
 * It is mainly used when derived arguments requires other attributes like
 * a user-specified value and only one index option (short or long).
 * Simply use tappo::placeholders::_ to replace alias in constructor, and
 * the constructor will automatically ignore this value without specifying
 * the alias attribute. In other words, @_alias will remain a nullopt.
 */
namespace placeholders {
std::monostate _;
}

class ArgBase {
public:
  /**
   * Return the description of the argument.
   * This function is required especially when we tend to print the help
   * message on the screen.
   */
  constexpr const auto desc() const { return _desc; }
  
  /** Set a new description of this argument. */
  template<Option op, typename VType>
  void set(VType&& val) {
    Setter<op, VType>::set(*this, val);
  }
  
protected:
  /** Default constructors. */
  constexpr ArgBase() = default;
  constexpr ArgBase(const std::string_view& desc) : _desc(desc) {  }

  /** Helper type for static assert. */
  template<Option op> struct always_false : std::false_type {};

  /**
   * An assistant template struct for setting values.
   * The first template argument @op is constrainted to be in an enum class,
   * in which each type will be used to specialize this template struct.
   */
  template<Option op, typename VType> struct Setter {
    static void set(ArgBase& arg, VType&& val) {  }
    static_assert(always_false<op>::value, "Invalid type in setter.");
  };
  
  /**
   * Partial-specialization of @Setter struct.
   * Other specialization of this function will be introduced in the derived
   * classes, like Option::value, etc.
   */
  template<typename VType>
  struct Setter<Option::desc, VType> {
    static void set(ArgBase& arg, VType&& desc) {
      arg._desc = desc;
    }
  };

  /** Description of the current argument (in detail). */
  std::string_view _desc;
};

template<typename IType>
class Arg : public ArgBase {
  /**
   * Type deduction.
   * The template type should be deducted from the arguments in constructor,
   * which is only allowed to be either char or std::string_view.
   *
   * Valid @IType will be considered as the index type: single character for
   * short options and a string for long options. Usually a pair of short
   * and long options aims to accomplish one task, so exact one of them is
   * considered to be the index while the other is its alias. However, the
   * alias might not exist.
   */
  using DecayedType = std::decay_t<IType>;
  static_assert(std::is_same_v<DecayedType, char> ||
                std::is_same_v<DecayedType, std::string_view>,
                "Invalid argument index type.");
public:
  using BaseType  = ArgBase;
  using IndexType = std::conditional_t<
    std::is_same_v<DecayedType, char>, char, std::string_view>;
  using AliasType = std::conditional_t<
    std::is_same_v<DecayedType, char>, std::string_view, char>;

  /** Default constructors. */
  constexpr Arg() = delete;
  constexpr Arg(const IType& name) : _index(name) {  }
  constexpr Arg(const IType& name, const AliasType& alias)
      : _index(name), _alias(alias) {  }

  /**
   * Return the short/long option of this argument.
   * The short/long option can be either @_index or @_alias, so basically
   * this function checks index type in compile-time and then returns the
   * corresponding option.
   */
  constexpr const auto short_option() const {
    if constexpr (std::is_same_v<IndexType, char>)
      return std::get<0>(_index);
    else
      // Alias is allowed to be not exist, so this function might throw
      // bad optional access exception.
      return std::get<0>(_alias.value());
  }
  constexpr const auto long_option() const {
    if constexpr (std::is_same_v<IndexType, std::string_view>)
      return std::get<1>(_index);
    else
      return std::get<1>(_alias.value());
  }

  /**
   * Return the RAW object of @_index and @_alias.
   * The member @_index has type of variant<char, string_view>.
   * The member @_alias has type of optional<variant<char, string_view> >.
   *
   * In other words, alias object is allowed to have no value. The raw
   * index is used in program option management by an argument parser.
   */
  constexpr const auto raw_index() const { return _index; }
  constexpr const auto raw_alias() const { return _alias; }

  /**
   * Return index and alias of this argument object.
   * Note that the alias is only optional so the visitor @alias() might
   * throw exception bad_optional_access when the optional object @_alias
   * actually does not contain a value.
   */
  constexpr const auto index() const {
    return std::get<IndexType>(_index);
  }
  constexpr const auto alias() const {
    return std::get<AliasType>(_alias.value());
  }
  
protected:
  template<Option op, typename Val>
  struct Setter : public BaseType::template Setter<op, Val> { };

  /**
   * Check whether a short option or a long option is valid.
   * In our argument class, only alphabet letters are allowed for both a
   * short option and the first letter of a long option.
   * @return the boolean variable indicating whether the input option is
   *         valid or not (short or long option).
   *
   * Note that a long option with only single letter is allowed here,
   * as long as indicated by '--' or other specified flags, ALTHOUGH it
   * is NOT encouraged because it is better to be made a short option. 
   */
  constexpr static bool check_option(int ch) {
    return (ch > 64 && ch < 91) || (ch > 96 && ch < 123);
  }
  constexpr static bool check_option(const std::string_view& str) {
    return !str.empty() && check_option(str[0]);
  }

  /**
   * The index of this argument.
   * It can be either a long option (std::string_view) or a short option
   * (char). We use std::variant<char, std::string_view> here because the
   * index type here cannot easily be determined as some arguments are only
   * allowed to be parsed via long or short options instead of both.
   */
  Index _index;

  /**
   * The alias of this argument.
   * It is possible that the alias @_alias of one argument actually does not
   * exist, so we use std::optional<Index> here.
   * If it does have a value, its value should be the counterpart of the
   * index. In other words, the alias should be a short/long option that
   * parses the same argument if the index is a long/short option.
   */
  std::optional<Index> _alias;
};

template<typename VType, typename IType>
class ValueArg : public Arg<IType> {
public:
  /** Type alias from base. */
  using ValueType = VType;
  using BaseType  = Arg<IType>;
  using IndexType = typename BaseType::IndexType;
  using AliasType = typename BaseType::AliasType;
  
  /**
   * Default constructors.
   * The type deduction follows the same way as its parent class @Arg.
   * Value @val in constructors are the default value of this argument.
   */
  constexpr ValueArg() = delete;
  constexpr ValueArg(const IType& name,
                     const std::monostate& _, const VType& val) :
      Arg<IType>(name), _value(val) { }
  constexpr ValueArg(const IType& name,
                     const AliasType& alias, const VType& val) :
      Arg<IType>(name, alias), _value(val) { }

  /**
   * Return the value of this argument. Usually an argument has at least
   * one value parsed to the program. Some options that provide no value
   * can be seen as flags, or in other words, they provide bool values.
   */
  const auto value() const { return _value; }

  /**
   * Set the value of this argument.
   * @param val The new value to replace the original one.
   */
  template<Option op, typename Val>
  void set(Val&& val) {
    Setter<op, Val>::set(*this, std::move(val));
  }
  
private:
  template<Option op, typename Val>
  struct Setter : public BaseType::template Setter<op, Val> { };

  /** Partial-specialization of @set member function. */
  template<typename Val>
  struct Setter<Option::value, Val> {
    static void set(ValueArg<VType, IType>& arg, Val&& val) {
      arg._value = val;
    }
  };
  
  /**
   * The value of this argument.
   * Usually we do not consider the value type as a huge class, as in normal
   * cases we only parse simple arguments from command line options to out
   * programs.
   */
  VType _value;
};

}

#endif

