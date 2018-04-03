/* ------------------------------------------------------------------------- *
 * Copyright 2017 Ingenieurbüro Krug. All rights reserved.
 *
 * Author: Florian Eich <florian.eich@ingenieurbuero-krug.de>
 * ------------------------------------------------------------------------- */

#ifndef SCRUBB_CORE_THRESHOLD_H_
#define SCRUBB_CORE_THRESHOLD_H_


namespace scrubb {


template <class I>
class threshold
{
  using self_t  = threshold<I>;
  using input_t = I;

public:
  using value_t = typename input_t::value_t;

public:
  threshold(input_t & input, value_t limit) noexcept
   : _input(input), _limit(limit) {}

  threshold(self_t && other) = default;

  void set_limit(value_t limit) noexcept { _limit = limit; }

  bool operator()() noexcept { return _input() > _limit; }

private:
  input_t & _input;
  value_t   _limit;
};


template <class I>
inline threshold<I> make_threshold(I &                 input,
                                   typename I::value_t limit) noexcept {
  return {input, limit};
}

}  // namespace scrubb

#endif  // SCRUBB_CORE_THRESHOLD_H_
