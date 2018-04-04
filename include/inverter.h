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

#ifndef PIPEBB_INVERTER_H_
#define PIPEBB_INVERTER_H_


namespace pipebb {


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

}  // namespace pipebb

#endif  // PIPEBB_INVERTER_H_
