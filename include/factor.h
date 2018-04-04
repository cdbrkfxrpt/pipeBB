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

#ifndef PIPEBB_FACTOR_H_
#define PIPEBB_FACTOR_H_


namespace pipebb {


/*!
 * \brief Class which make it possible to multiply an output with a constant.
 * \param I Template parameter specifying the type of the input object.
 *
 * The `factor` gate is used to normalize the output of another gate by
 * applying a constant factor to it. We use a gate to create this behaviour
 * because we need all element of the process chain to be callable.
 *
 * **Usage**
 * \include make_factor.cc
 *
 * For further examples, look into the tests.
 */
template <class I>
class factor
{
  using self_t  = factor<I>;
  using input_t = I;

public:
  using value_t = typename input_t::value_t;

public:
  /*!
   * \brief Constructor for `factor` class.
   * \param input Input object.
   * \param constant Constant to multiply with.
   */
  factor(input_t & input, value_t constant) noexcept
   : _input(input), _constant(constant) {}

  /*!
   * \brief Move copy constructor for `factor` class.
   * \param other Universal reference to object to acquire ownership of.
   *
   * This constructor is used in the generator function `make_factor` but
   * shouldn't otherwise be used (unless of course you have a very good reason
   * to do so). It is compiler generated.
   *
   * _Note: This is the only other way - aside from the main constructor - to
   * instantiate an `approximate` object._
   */
  factor(self_t && other) = default;

  /*!
   * \brief Function to set the constant to a new value.
   * \param constant Constant to set.
   */
  void set_constant(value_t constant) noexcept { _constant = constant; }

  /*!
   * \brief   Get value from input object, multiply by the constant and return.
   * \returns Output from input object multiplied by the contained constant.
   *
   * Every pipeBB gate is callable. When called, it calls the input object and
   * takes appropriate action with the result.
   */
  value_t operator()() noexcept { return _input() * _constant; }

private:
  input_t & _input;
  value_t   _constant;
};


/*!
 * \brief  Maker function to facilitate creating `factor` objects.
 * \param  I Template parameter specifying type of input.
 * \param  input Input object.
 * \param  c Constant to multiply with.
 * \return Ready to use `factor` object.
 *
 * A `factor` object can also be instantiated by calling the constructor
 * of the `factor` class directly. Without a C++17 capable compiler, this
 * requires using the `decltype` built-in a lot, therefore (and for consistency
 * with the rest of the library) this function is provided.
 *
 * **Usage**
 * \include make_factor.cc
 */
template <class I>
inline factor<I> make_factor(I &                 input,
                             typename I::value_t constant) noexcept {
  return {input, constant};
}


}  // namespace pipebb

#endif  // PIPEBB_FACTOR_H_
