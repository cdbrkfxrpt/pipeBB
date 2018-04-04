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

#ifndef PIPEBB_ACCUMULATOR_H_
#define PIPEBB_ACCUMULATOR_H_

#include <numeric>

#include "ringbuffer.h"


namespace pipebb {


/*!
 * \brief Class which accumulates values from its input object.
 * \param I Template parameter specifying the type of the input object.
 * \param N Template parameter specifying the size of the `accumulator`.
 *
 * The `accumulator` gate is used to collect values from another gate and sum
 * them up. Let's assume you have a throughput measurement and you want to know
 * how much material in total passed through in the last 5 time steps; in that
 * case, you could use the `accumulator` gate with a size of 5 and call its
 * `value()` function every time step.
 *
 * **Usage**
 * \include make_accumulator.cc
 *
 * For further examples, look into the tests.
 */
template <class I, std::size_t N>
class accumulator
{
  using self_t   = accumulator<I, N>;
  using input_t  = I;
  using buffer_t = ringbuffer<typename input_t::value_t, N>;

public:
  using value_t = typename input_t::value_t;

public:
  /*!
   * \brief Constructor for `accumulator` class.
   * \param input Input object.
   * \param use_zeros Specifies if zeros get used or not. Default: true.
   */
  accumulator(input_t & input, bool use_zeros = true) noexcept
   : _input(input), _use_zeros(use_zeros) {}

  /*!
   * \brief Resets the buffer inside `accumulator` object.
   */
  void reset() noexcept { _buffer.fill(value_t()); }

  /*!
   * \brief   Get value of `accumulator` object without updating content.
   * \returns Current (non-updated) value of the gate.
   */
  value_t report() noexcept {
    return std::accumulate(_buffer.cbegin(), _buffer.cend(), value_t());
  }

  /*!
   * \brief   Update `accumulator` content, then get value.
   * \returns Current (updated) value of the gate.
   *
   * Every pipeBB gate is callable. When called, it calls the input object and
   * takes appropriate action with the result.
   */
  value_t operator()() noexcept {
    value_t val = _input();
    if (val || _use_zeros) { _buffer << val; }

    /* --------------------------------------------------------------------- *
     * std::reduce is a C++17 feature; use when it is part of libstdc++.
     *
     * That is, libstdc++ has to implement execution_policy support, described
     * in proposal P0024R2 (called "The Parallelism TS should be Standardized")
     * first, the implement std::reduce.
     *
     * libstdc++ C++17 progress:
     * https://gcc.gnu.org/onlinedocs/libstdc++/manual/status.html#status.iso.201z
     *
     * If this has happened, uncomment the return call to std::reduce below and
     * get rid of the return call to std::accumulate below that. Afterwards,
     * this comment can be removed.
     * --------------------------------------------------------------------- */
    //
    // return std::reduce(std::execution::par, _buffer.begin(), _buffer.end());
    //

    return std::accumulate(_buffer.cbegin(), _buffer.cend(), value_t());
  }

private:
  input_t & _input;
  bool      _use_zeros;
  buffer_t  _buffer;
};


/*!
 * \brief   Maker function to facilitate creating `accumulator` objects.
 * \param   N Template parameter specifying number of accumulated values.
 * \param   I Template parameter specifying the type of `input`.
 * \param   input Input object.
 * \returns Ready to use `accumulator` object.
 *
 * An `accumulator` object can be instantiated by calling the constructor of
 * the `accumulator` class. However, in longer code segments using pipeBB
 * gates, using constructors (which ordinarily requires using the `decltype`
 * built-in) can make the code difficult to read. The `make_accumulator`
 * function provides a simpler interface to instantiate an `accumulator`
 * object.
 *
 * **Usage**
 * \include make_accumulator.cc
 */
template <std::size_t N, class I>
inline accumulator<I, N> make_accumulator(I & input) noexcept {
  return {input};
}

}  // namespace pipebb

#endif  // PIPEBB_ACCUMULATOR_H_
