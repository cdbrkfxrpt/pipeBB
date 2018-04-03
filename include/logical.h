/* ------------------------------------------------------------------------- *
 * Copyright 2017 Ingenieurbüro Krug. All rights reserved.
 *
 * Author: Florian Eich <florian.eich@ingenieurbuero-krug.de>
 * ------------------------------------------------------------------------- */

#ifndef SCRUBB_CORE_LOGIC_H_
#define SCRUBB_CORE_LOGIC_H_

#include <functional>
#include <utility>


namespace scrubb {


template <class L, class... Is>
class logic_gate
{};

template <class L, class I, class... Is>
class logic_gate<L, I, Is...> : logic_gate<L, Is...>
{
  using self_t     = logic_gate<L, I, Is...>;
  using parent_t   = logic_gate<L, Is...>;
  using logical_op = L;

public:
  using value_t = bool;

public:
  logic_gate(I & input, Is &... inputs) noexcept
   : logic_gate<logical_op, Is...>(inputs...), _input(input) {}

  value_t operator()() noexcept {
    return logical_op()(_input(), parent_t::operator()());
  }

private:
  I & _input;
};

template <class L, class I, class J>
class logic_gate<L, I, J>
{
  using self_t     = logic_gate<L, I, J>;
  using logical_op = L;
  using lhs_t      = I;
  using rhs_t      = J;

public:
  using value_t = bool;

public:
  logic_gate(lhs_t & first, rhs_t & second) noexcept
   : _first(first), _second(second) {}

  value_t operator()() noexcept { return logical_op()(_first(), _second()); }

private:
  lhs_t & _first;
  rhs_t & _second;
};


template <class... Is>
class and_gate : public logic_gate<std::logical_and<bool>, Is...>
{};

template <class I, class... Is>
class and_gate<I, Is...> : public logic_gate<std::logical_and<bool>, I, Is...>
{
  using self_t   = and_gate<I, Is...>;
  using parent_t = logic_gate<std::logical_and<bool>, I, Is...>;

public:
  and_gate(I & input, Is &... inputs) noexcept : parent_t(input, inputs...) {}
};

template <class I, class J>
class and_gate<I, J> : public logic_gate<std::logical_and<bool>, I, J>
{
  using self_t   = and_gate<I, J>;
  using parent_t = logic_gate<std::logical_and<bool>, I, J>;
  using lhs_t    = I;
  using rhs_t    = J;

public:
  and_gate(lhs_t & first, rhs_t & second) noexcept : parent_t(first, second) {}
};


template <class... Is>
inline and_gate<Is...> make_and_gate(Is &... inputs) noexcept {
  return {inputs...};
}


template <class... Is>
class or_gate : public logic_gate<std::logical_or<bool>, Is...>
{};

template <class I, class... Is>
class or_gate<I, Is...> : public logic_gate<std::logical_or<bool>, I, Is...>
{
  using self_t   = or_gate<I, Is...>;
  using parent_t = logic_gate<std::logical_or<bool>, I, Is...>;

public:
  or_gate(I & input, Is &... inputs) noexcept : parent_t(input, inputs...) {}
};

template <class I, class J>
class or_gate<I, J> : public logic_gate<std::logical_or<bool>, I, J>
{
  using self_t   = or_gate<I, J>;
  using parent_t = logic_gate<std::logical_or<bool>, I, J>;
  using lhs_t    = I;
  using rhs_t    = J;

public:
  or_gate(lhs_t & first, rhs_t & second) noexcept : parent_t(first, second) {}
};


template <class... Is>
inline or_gate<Is...> make_or_gate(Is &... inputs) noexcept {
  return {inputs...};
}


template <class I>
class not_gate
{
  using self_t  = not_gate<I>;
  using input_t = I;

public:
  using value_t = bool;

public:
  explicit not_gate(input_t & input) noexcept : _input(input) {}

  value_t operator()() noexcept { return !_input(); }

private:
  input_t & _input;
};


template <class I>
inline not_gate<I> make_not_gate(I & input) noexcept {
  return {input};
}

}  // namespace scrubb

#endif  // SCRUBB_CORE_LOGIC_H_
