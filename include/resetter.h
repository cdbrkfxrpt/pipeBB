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

#ifndef PIPEBB_RESETTER_H_
#define PIPEBB_RESETTER_H_


namespace pipebb {


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

}  // namespace pipebb

#endif  // PIPEBB_RESETTER_H_
