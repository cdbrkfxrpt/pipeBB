/* ------------------------------------------------------------------------- *
 * Copyright 2017 Ingenieurbüro Krug. All rights reserved.
 *
 * Author: Florian Eich <florian.eich@ingenieurbuero-krug.de>
 * ------------------------------------------------------------------------- */

#ifndef SCRUBB_CORE_PASS_THROUGH_H_
#define SCRUBB_CORE_PASS_THROUGH_H_


namespace scrubb {


template <class I, class A>
class pass_through
{
  using self_t      = pass_through<I, A>;
  using input_t     = I;
  using activator_t = A;

public:
  using value_t = typename input_t::value_t;

public:
  pass_through(input_t & input, activator_t & activator) noexcept
   : _input(input), _activator(activator) {}

  pass_through(self_t && other) = default;

  value_t operator()() noexcept {
    if (_activator()) { return _input(); }

    return {};
  }

private:
  input_t &     _input;
  activator_t & _activator;
};


template <class I, class A>
inline pass_through<I, A> make_pass_through(I & input,
                                            A & activator) noexcept {
  return {input, activator};
}


template <class I>
class threshold_pass_through
{
  using self_t  = threshold_pass_through<I>;
  using input_t = I;

public:
  using value_t = typename input_t::value_t;

public:
  threshold_pass_through(input_t & input, value_t limit) noexcept
   : _input(input), _limit(limit) {}

  threshold_pass_through(self_t && other) = default;

  value_t operator()() noexcept {
    auto val = _input();
    if (val > _limit) {
      return val;
    } else {
      return {};
    }
  }

private:
  input_t & _input;
  value_t   _limit;
};


template <class I>
inline threshold_pass_through<I> make_threshold_pass_through(
  I & input, typename I::value_t limit) noexcept {
  return {input, limit};
}


template <class I, class A>
class buffered_pass_through
{
  using self_t      = buffered_pass_through<I, A>;
  using input_t     = I;
  using activator_t = A;

public:
  using value_t = typename input_t::value_t;

public:
  buffered_pass_through(input_t & input, activator_t & activator) noexcept
   : _input(input), _activator(activator) {}

  buffered_pass_through(self_t && other) = default;

  value_t operator()() noexcept {
    if (_activator()) { _value = _input(); }
    return _value;
  }

private:
  input_t &     _input;
  activator_t & _activator;
  bool          _init{false};
  value_t       _value{};
};


template <class I, class A>
inline buffered_pass_through<I, A> make_buffered_pass_through(
  I & input, A & activator) noexcept {
  return {input, activator};
}

}  // namespace scrubb

#endif  // SCRUBB_CORE_PASS_THROUGH_H_
