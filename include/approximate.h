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

#ifndef PIPEBB_APPROXIMATE_H_
#define PIPEBB_APPROXIMATE_H_


namespace pipebb {


/*!
 * \brief Class which evaluates distance between input objects values'.
 * \param I Template parameter specifying the type of the first input object.
 * \param J Template parameter specifying the type of the second input object.
 *
 * The `approximate` gate is used to check if two input objects, e.g. other
 * gates, currently have approximately the same values given a tolerance. So,
 * for example, if you want to check if two sensors give the same values
 * within a measurement error tolerance, this gate is your friend.
 *
 * **Usage**
 * \include make_approximate.cc
 *
 * For further examples, look into the tests.
 */
template <class I, class J>
class approximate
{
  using self_t  = approximate<I, J>;
  using input_t = I;

public:
  using value_t = typename input_t::value_t;

public:
  /*!
   * \brief Constructor for `approximate` class.
   * \param first First input object.
   * \param second Second input object.
   * \param tolerance Acceptable distance between input objects values'.
   */
  approximate(I & first, J & second, value_t tolerance) noexcept
   : _first(first), _second(second), _tolerance(tolerance) {}

  /*!
   * \brief Set new tolerance value for the gate.
   * \param tolerance New tolerance.
   */
  void set_tolerance(value_t tolerance) noexcept { _tolerance = tolerance; }

  /*!
   * \brief   Update `approximate` content, then get value.
   * \returns Current (updated) value of the gate.
   *
   * Every pipeBB gate is callable. When called, it calls the input object and
   * takes appropriate action with the result.
   */
  bool operator()() noexcept {
    auto lhs = _first();
    auto rhs = _second();

    if (lhs > rhs) {
      return (lhs - rhs) <= _tolerance;
    } else {
      return (rhs - lhs) <= _tolerance;
    }
  }

private:
  I &     _first;
  J &     _second;
  value_t _tolerance;
};


/*!
 * \brief  Maker function to facilitate creating `approximate` objects.
 * \param  I Template parameter specifying type of first input.
 * \param  J Template parameter specifying type of second input.
 * \param  first First input object.
 * \param  second Second input object.
 * \param  tolerance Specifies acceptable distance between first and second.
 * \return Ready to use `approximate` object.
 *
 * An `approximate` object can also be instantiated by calling the constructor
 * of the `approximate` class directly. Without a C++17 capable compiler, this
 * requires using the `decltype` built-in a lot, therefore (and for consistency
 * with the rest of the library) this function is provided.
 *
 * **Usage**
 * \include make_approximate.cc
 */
template <class I, class J>
inline approximate<I, J> make_approximate(
  I & first, J & second, typename I::value_t tolerance) noexcept {
  return {first, second, tolerance};
}

}  // namespace pipebb

#endif  // PIPEBB_APPROXIMATE_H_
