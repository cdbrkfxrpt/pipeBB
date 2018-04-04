//
// Copyright 2018- Florian Eich <florian.eich@gmail.com>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef PIPEBB_OFFSET_H_
#define PIPEBB_OFFSET_H_


namespace pipebb {


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

}  // namespace pipebb

#endif  // PIPEBB_OFFSET_H_
