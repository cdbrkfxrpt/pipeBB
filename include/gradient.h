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

#ifndef PIPEBB_GRADIENT_H_
#define PIPEBB_GRADIENT_H_

#include <utility>
#include <cstdint>

#include "ringbuffer.h"


namespace pipebb {


/*!
 * \brief Class which calculates the difference between two measurement points.
 * \param I Template parameter specifying the type of the input object.
 *
 * The `gradient` gate is used to calculate the difference between two adjacent
 * measurement points of an incoming signal, i.e. the gradient.
 *
 * **Usage**
 * \include make_gradient.cc
 *
 * For further examples, look into the tests.
 */
template <class I>
class gradient
{
  using self_t  = gradient<I>;
  using input_t = I;

public:
  using value_t = std::int_fast32_t;

private:
  using pair_t = std::pair<value_t, value_t>;

public:
  /*!
   * \brief Constructor for `gradient` class.
   * \param input Input object.
   */
  gradient(input_t & input) noexcept : _input(input) {}

  /*!
   * \brief Move copy constructor for `gradient` class.
   * \param other Universal reference to object to acquire membership of.
   */
  gradient(self_t && other) = default;

  /*!
   * \brief Function to reset the gate.
   */
  void reset() noexcept {
    _pair.first  = value_t();
    _pair.second = value_t();
  }

  /*!
   * \brief   Update `gradient` content, then get value.
   * \returns Current (updated) value of the gate.
   *
   * Every pipeBB gate is callable. When called, it calls the input object and
   * takes appropriate action with the result.
   */
  value_t operator()() noexcept {
    std::swap(_pair.first, _pair.second);
    _pair.second = _input();
    return _pair.second - _pair.first;
  }

private:
  input_t & _input;
  pair_t    _pair;
};


/*!
 * \brief  Maker function to facilitate creating `gradient` objects.
 * \param  I Template parameter specifying type of input.
 * \param  input Input object.
 * \return Ready to use `gradient` object.
 *
 * An `gradient` object can also be instantiated by calling the constructor of
 * the `gradient` class directly. Without a C++17 capable compiler, this
 * requires using the `decltype` built-in a lot, therefore (and for consistency
 * with the rest of the library) this function is provided.
 *
 * **Usage**
 * \include make_gradient.cc
 */
template <class I>
inline gradient<I> make_gradient(I & input) noexcept {
  return {input};
}


/*!
 * \brief Class which calculates the difference between two measurement points.
 * \param I Template parameter specifying the type of the input object.
 * \param N Template parameter specifying the distance between the measurement
 *        points.
 *
 * The `varstep_gradient` gate is used to calculate the difference between two
 * measurement points of an incoming signal, i.e. the gradient, at a fixed
 * distance between the measurement points.
 *
 * **Usage**
 * \include make_varstep_gradient.cc
 *
 * For further examples, look into the tests.
 */
template <class I, std::size_t N>
class varstep_gradient
{
  using self_t  = varstep_gradient<I, N>;
  using input_t = I;
  using range_t = ringbuffer<typename input_t::value_t, N>;

public:
  using value_t = typename input_t::value_t;

public:
  /*!
   * \brief Constructor for `varstep_gradient` class.
   * \param input Input object.
   */
  varstep_gradient(input_t & input) noexcept : _input(input) { reset(); }

  /*!
   * \brief Move copy constructor for `gradient` class.
   * \param other Universal reference to object to acquire membership of.
   */
  varstep_gradient(self_t && other) = default;

  /*!
   * \brief Function to reset gate.
   */
  void reset() noexcept { _range.fill(value_t()); }

  /*!
   * \brief   Update `varstep_gradient` content, then get value.
   * \returns Current (updated) value of the gate.
   *
   * Every pipeBB gate is callable. When called, it calls the input object and
   * takes appropriate action with the result.
   */
  value_t operator()() noexcept {
    _range.push(_input());
    return _range.back() - _range.front();
  }

  /*!
   * \brief   Get the range used for calculating the gradient.
   * \returns Circular buffer containing current range.
   *
   * The `varstep_gradient` works by stashing incoming values away into a
   * circular buffer, which is initialized with zeroes in the beginning. The
   * buffer has the sized of the distance N so that the difference between the
   * first element in the buffer and the last is exactly the gradient we are
   * looking for.
   *
   * This function gives back the entire circular buffer for inspection.
   */
  range_t & range() noexcept { return _range; }

private:
  input_t &   _input;
  std::size_t _size;
  range_t     _range;
};


/*!
 * \brief   Maker function to facilitate creating `varstep_gradient` objects.
 * \param   N Template parameter specifying number of steps for gradient.
 * \param   I Template parameter specifying the type of `input`.
 * \param   input Input object.
 * \returns Ready to use `varstep_gradient` object.
 *
 * A `varstep_gradient` object can be instantiated by calling the constructor
 * of the `varstep_gradient` class. However, in longer code segments using
 * pipeBB gates, using constructors (which ordinarily requires using the
 * `decltype` built-in) can make the code difficult to read. The
 * `make_varstep_gradient` function provides a simpler interface to instantiate
 * an `varstep_gradient` object.
 *
 * **Usage**
 * \include make_varstep_gradient.cc
 */
template <std::size_t N, class I>
inline varstep_gradient<I, N> make_varstep_gradient(I & input) noexcept {
  return {input};
}

}  // namespace pipebb

#endif  // PIPEBB_GRADIENT_H_
