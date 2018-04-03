/* ------------------------------------------------------------------------- *
 * Copyright 2017 Ingenieurbüro Krug. All rights reserved.
 *
 * Author: Florian Eich <florian.eich@ingenieurbuero-krug.de>
 * ------------------------------------------------------------------------- */

#ifndef SCRUBB_CORE_INVERTER_H_
#define SCRUBB_CORE_INVERTER_H_


namespace scrubb {


template <class I>
class inverter
{
  using self_t  = inverter<I>;
  using input_t = I;

public:
  using value_t = typename input_t::value_t;

public:
  inverter(input_t & input) noexcept : _input(input) {}

  inverter(self_t && other) = default;

  value_t operator()() noexcept { return -_input(); }

private:
  input_t & _input;
};


template <class I>
inline inverter<I> make_inverter(I & input) noexcept {
  return {input};
}

}  // namespace scrubb

#endif  // SCRUBB_CORE_INVERTER_H_
