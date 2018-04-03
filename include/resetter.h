/* ------------------------------------------------------------------------- *
 * Copyright 2017 Ingenieurbüro Krug. All rights reserved.
 *
 * Author: Florian Eich <florian.eich@ingenieurbuero-krug.de>
 * ------------------------------------------------------------------------- */

#ifndef SCRUBB_CORE_RESETTER_H_
#define SCRUBB_CORE_RESETTER_H_


namespace scrubb {


template <class I, class T>
class resetter
{
  using self_t   = resetter<I, T>;
  using input_t  = I;
  using target_t = T;

public:
  resetter(input_t & input, target_t & target) noexcept
   : _input(input), _target(target) {}

  resetter(self_t && other) = default;

  bool operator()() noexcept {
    if (_input()) { _target.reset(); }

    return true;
  }

private:
  input_t &  _input;
  target_t & _target;
};


template <class I, class T>
inline resetter<I, T> make_resetter(I & input, T & target) noexcept {
  return {input, target};
}

}  // namespace scrubb

#endif  // SCRUBB_CORE_RESETTER_H_
