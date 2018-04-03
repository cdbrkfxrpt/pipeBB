/* ------------------------------------------------------------------------- *
 * Copyright 2017 Ingenieurbüro Krug. All rights reserved.
 *
 * Author: Florian Eich <florian.eich@ingenieurbuero-krug.de>
 * ------------------------------------------------------------------------- */

#ifndef SCRUBB_CORE_OFFSET_H_
#define SCRUBB_CORE_OFFSET_H_


namespace scrubb {


template <class I>
class offset
{
  using self_t  = offset<I>;
  using input_t = I;

public:
  using value_t = typename input_t::value_t;

public:
  offset(input_t & input, value_t offset) noexcept
   : _input(input), _offset(offset) {}

  offset(self_t && other) = default;

  value_t get_offset() noexcept { return _offset; }

  void set_offset(value_t offset) noexcept { _offset = offset; }

  value_t operator()() noexcept { return _input() + _offset; }

private:
  input_t & _input;
  value_t   _offset;
};


template <class I>
inline offset<I> make_offset(I & input, typename I::value_t offset) noexcept {
  return {input, offset};
}

}  // namespace scrubb

#endif  // SCRUBB_CORE_OFFSET_H_
