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

#ifndef PIPEBB_THRESHOLD_H_
#define PIPEBB_THRESHOLD_H_


namespace pipebb {


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

}  // namespace pipebb

#endif  // PIPEBB_THRESHOLD_H_
