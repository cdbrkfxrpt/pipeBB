/* ------------------------------------------------------------------------- *
 * Copyright 2017 Ingenieurbüro Krug. All rights reserved.
 *
 * Author: Florian Eich <florian.eich@ingenieurbuero-krug.de>
 * ------------------------------------------------------------------------- */

#ifndef SCRUBB_CORE_BUFFER_H_
#define SCRUBB_CORE_BUFFER_H_


namespace scrubb {


/*!
 * \brief Class which buffers values from its input object.
 * \param I Template parameter specifying the type of the input object.
 *
 * The `buffer` gate is used to store values over several calls. On the first
 * call, it retrieves the current value from its input object and returns that
 * value on every subsequent call until the `reset()` function is called, at
 * which point it retrieves its input objects value again on the next call to
 * `value()`. This is useful for control flow, i.e. when you want to retrieve a
 * gate's value several times during the same time sequence.
 *
 * **Usage**
 * \include make_buffer.cc
 *
 * For further examples, look into the tests.
 */
template <class I>
class buffer
{
  using self_t  = buffer<I>;
  using input_t = I;

public:
  using value_t = typename input_t::value_t;

public:
  /*!
   * \brief Constructor for `buffer` class.
   * \param input Input object.
   */
  buffer(input_t & input) noexcept : _input(input) {}

  /*!
   * \brief Reset function for `buffer` class/gate.
   *
   * This function resets the `buffer` and makes it retrieve the value of its
   * input object anew on the next call to `value()`. Call this at the end of
   * your loop.
   */
  void reset() noexcept { _value = {}; }

  /*!
   * \brief   Provides the `buffer`'s value.
   * \returns The current value of the `buffer`'s input object if the `buffer`
   *          is empty or the value stored in the `buffer`.
   */
  value_t operator()() noexcept {
    if (!_value) { _value = _input(); }

    return _value;
  }

private:
  input_t & _input;
  value_t   _value{};
};


/*!
 * \brief   Maker function to facilitate creating `buffer` objects.
 * \param   I Template parameter specifying the type of the input.
 * \param   input The input object on which `value()` will be called.
 * \returns Ready to use `buffer` object.
 *
 * A `buffer` object can be instantiated by calling the constructor of the
 * `buffer` class. To eliminate the need for calls to `decltype`, which can
 * clutter up code, and to ensure consistency throughout the library, this
 * helper function is provided.
 *
 * **Usage**
 * \include make_buffer.cc
 */
template <class I>
inline buffer<I> make_buffer(I & input) noexcept {
  return {input};
}


/*!
 * \brief Class which buffers values from its input object.
 * \param I Template parameter specifying the type of the input object.
 *
 * The `switched_buffer` gate is the same as the `buffer` gate. However, it
 * offers two additional functions: `toggle()`, used to toggle buffering, so it
 * can also be actively turned off, and `state()`, returning a boolean which
 * tells us whether the buffer is currently storing a value (true) or waiting
 * to retrieve one (false).
 *
 * **Usage**
 * \include make_switched_buffer.cc
 *
 * For further examples, look into the tests.
 */
template <class I>
class switched_buffer
{
  using self_t  = switched_buffer<I>;
  using input_t = I;

public:
  using value_t = typename input_t::value_t;

public:
  /*!
   * \brief Constructor for `switched_buffer` class.
   * \param input Input object.
   */
  switched_buffer(input_t & input) : _input(input) {}

  /*!
   * \brief Move copy constructor for `buffer` class.
   * \param other Universal reference to object to acquire ownership of.
   *
   * This constructor is used in the generator function `make_buffer` but
   * shouldn't otherwise be used (unless of course you have a very good reason
   * to do so). It is compiler generated.
   *
   * _Note: This is the only other way - aside from the main constructor - to
   * instantiate an `buffer` object._
   */
  switched_buffer(self_t && other) = default;

  /*!
   * \brief Toggles state of the `switched_buffer`.
   *
   * The `switched_buffer` gate can either be storing a value or waiting to
   * retrieve one from its input object. This function toggles between these
   * states.
   */
  void toggle() noexcept {
    if (_switch) {
      _switch = false;
    } else {
      _switch = true;
    }
  }

  /*!
   * \brief Reset function for `switched_buffer` class/gate.
   *
   * This function resets the buffer and makes it retrieve the value of its
   * input object anew on the next call to `value()`. Call this at the end of
   * your loop.
   */
  void reset() noexcept { _switch = false; }

  /*!
   * \brief   Provides the buffer's value.
   * \returns The current value of the buffer's input object if the buffer is
   *          inactive (switch is `false`) or the value stored in the buffer if
   *          the buffer is active (switch is `true`).
   */
  auto operator()() noexcept {
    if (!_switch) {
      _switch = true;
      _value  = _input();
    }
    return _value;
  }

  /*!
   * \brief   Provides information on the state of the `switched_buffer`.
   * \returns `true` if the buffer is storing a value, `false` if not.
   */
  bool state() noexcept { return _switch; }

private:
  input_t & _input;
  value_t   _value;
  bool      _switch{false};
};


/*!
 * \brief   Maker function to facilitate creating switched_buffer objects.
 * \param   I Template parameter specifying the type of the input.
 * \param   input The input object on whcih `value()` will be called.
 * \returns Ready to use `buffer` object.
 *
 * A switched_buffer object can be instantiated by calling the constructor of
 * the accumulator class. To eliminate the need for calls to `decltype`, which
 * can clutter up code, and to ensure consistency throughout the library, this
 * helper function is provided.
 *
 * **Usage**
 * \include make_switched_buffer.cc
 */
template <class I>
inline switched_buffer<I> make_switched_buffer(I & input) noexcept {
  return {input};
}

}  // namespace scrubb

#endif  // SCRUBB_CORE_BUFFER_H_
