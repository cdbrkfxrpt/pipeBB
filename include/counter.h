/* ------------------------------------------------------------------------- *
 * Copyright 2017 Ingenieurbüro Krug. All rights reserved.
 *
 * Author: Florian Eich <florian.eich@ingenieurbuero-krug.de>
 * ------------------------------------------------------------------------- */

#ifndef SCRUBB_CORE_COUNTER_H_
#define SCRUBB_CORE_COUNTER_H_


namespace scrubb {


/*!
 * \brief Very simple gate to count up, e.g. execution cycles.
 *
 * The `counter` gate is as simple as it gets and technically only provided for
 * consistency with the `scruBB` paradigm that everything must be callable. The
 * `make_counter` function is provided for library consistency as well.
 *
 * **Usage**
 * \include make_counter.cc
 *
 * For further examples, look into the tests.
 */
class counter
{
public:
  using value_t = uint64_t;

public:
  /*!
   * \brief Default constructor is defaulted.
   */
  counter() = default;

  /*!
   * \brief Constructor to initialize counter with preloaded value.
   */
  counter(const uint64_t & initval) noexcept : _counter(initval) {}

  /*!
   * \brief Function to reset `counter` value to 0.
   */
  void reset() noexcept { _counter = 0; }

  /*!
   * \brief   Get current value of `counter`.
   * \returns Current value of the gate.
   *
   * Every scruBB gate is callable. When called, it calls the input object and
   * takes appropriate action with the result.
   */
  value_t operator()() noexcept { return _counter; }

  /*!
   * \brief   Function to move `counter` up by one, i.e. increment it by one.
   * \returns New value of `counter` (after stepping).
   */
  value_t step() noexcept { return ++_counter; }

  /*!
   * \brief   Synonymous with `step()`.
   * \returns New value of `counter` (after stepping).
   *
   * This function is provided to be consistent with probable user
   * expectation,,
   * i.e. the user will expect a `counter` to be incrementable in the usual
   * manner.
   */
  value_t operator++() noexcept { return step(); }

  /*!
   * \brief   Increments `counter`'s value by one.
   * \returns Old value of `counter` (before stepping).
   *
   * Like `operator++()` (the prefix incrementation operator) and `step()`,
   * this function increments the current value of `counter`. However, it
   * returns the value _before_ the incrementation to the user. This function
   * is provided to be consistent with probably user expectation, i.e. the user
   * will expect a `counter` to be incrementable in the usual manner.
   */
  value_t operator++(int)noexcept { return ++_counter; }

private:
  value_t _counter{0};
};


/*!
 * \brief   Maker function to facilitate creating `counter` objects.
 * \returns Ready to use `counter` object.
 *
 * A `counter` object can be instantiated by calling the constructor of the
 * `counter` class. This function is provided to ensure consistency with the
 * other `scruBB` gates, thus making usage uniform and expectable.
 *
 * **Usage**
 * \include make_counter.cc
 */
inline counter make_counter() noexcept {
  return {};
}


/*!
 * \brief Class handling a `counter` and makes it resettable via input.
 * \param I Template parameter specifying the type of the input object.
 *
 * The `reset_counter` gate is used to handle a `counter` object and make it
 * resettable through an input object. So, for example, if you need to reset a
 * counter whenever a value is over a certain threshold, you plug a `threshold`
 * object into a `reset_counter` object and you're good to go.
 *
 * **Usage**
 * \include make_counter.cc
 *
 * For further examples, look into the tests.
 */
template <class I>
class reset_counter
{
  using self_t  = reset_counter<I>;
  using input_t = I;

public:
  using value_t = typename counter::value_t;

public:
  /*!
   * \brief Constructor for `reset_counter` class.
   * \param input Input object.
   */
  reset_counter(input_t & input) noexcept : _input(input) {}

  /*!
   * \brief Move copy constructor for `reset_counter` class.
   * \param other Universal reference to object to acquire ownership of.
   *
   * This constructor is used in the generator function `make_reset_counter`
   * but shouldn't otherwise be used (unless of course you have a very good
   * reason to do so). It is compiler generated.
   */
  reset_counter(self_t && other) = default;

  /*!
   * \brief   Update `reset_counter` content, then get value.
   * \returns Current (updated) value of the gate.
   *
   * Every scruBB gate is callable. When called, it calls the input object and
   * takes appropriate action with the result.
   */
  value_t operator()() noexcept {
    if (_input()) {
      _counter.reset();
    } else {
      _counter.step();
    }

    return _counter();
  }

private:
  input_t & _input;
  counter   _counter;
};


/*!
 * \brief  Maker function to facilitate creating `reset_counter` objects.
 * \param  I Template parameter specifying type of input.
 * \param  input Input object.
 * \return Ready to use `reset_counter` object.
 *
 * An `reset_counter` object can also be instantiated by calling the
 * constructor of the `reset_counter` class directly. Without a C++17 capable
 * compiler, this requires using the `decltype` built-in a lot, therefore (and
 * for consistency with the rest of the library) this function is provided.
 *
 * **Usage**
 * \include make_counter.cc
 */
template <class I>
inline reset_counter<I> make_reset_counter(I & input) noexcept {
  return {input};
}


/*!
 * \brief Class handling a `counter` and makes it conditionally incrementable.
 * \param I Template parameter specifying the type of the input object.
 *
 * The `boolean_counter` gate is used to handle a `counter` object and make it
 * conditionally incrementable through an input object. So, for example, if you
 * need to increment a counter only when a value is over a certain threshold,
 * you plug a `threshold` object into a `boolean_counter` object and you're
 * good to go.
 *
 * **Usage**
 * \include make_counter.cc
 *
 * For further examples, look into the tests.
 */
template <typename I>
class boolean_counter
{
  using self_t  = boolean_counter<I>;
  using input_t = I;

public:
  using value_t = typename counter::value_t;

public:
  /*!
   * \brief Constructor for `boolean_counter` class.
   * \param input Input object.
   */
  boolean_counter(input_t & input) noexcept : _input(input) {}

  /*!
   * \brief Move copy constructor for `boolean_counter` class.
   * \param other Universal reference to object to acquire ownership of.
   *
   * This constructor is used in the generator function `make_boolean_counter`
   * but shouldn't otherwise be used (unless of course you have a very good
   * reason to do so). It is compiler generated.
   */
  boolean_counter(self_t && other) = default;

  /*!
   * \brief   Update `boolean_counter` content, then get value.
   * \returns Current (updated) value of the gate.
   *
   * Every scruBB gate is callable. When called, it calls the input object and
   * takes appropriate action with the result.
   */
  value_t operator()() noexcept {
    return (_input() ? _counter.step() : _counter());
  }

private:
  input_t & _input;
  counter   _counter;
};


/*!
 * \brief  Maker function to facilitate creating `boolean_counter` objects.
 * \param  I Template parameter specifying type of input.
 * \param  input Input object.
 * \return Ready to use `boolean_counter` object.
 *
 * An `boolean_counter` object can also be instantiated by calling the
 * constructor of the `boolean_counter` class directly. Without a C++17 capable
 * compiler, this requires using the `decltype` built-in a lot, therefore (and
 * for consistency with the rest of the library) this function is provided.
 *
 * **Usage**
 * \include make_counter.cc
 */
template <class I>
inline boolean_counter<I> make_boolean_counter(I & input) noexcept {
  return {input};
}


/*!
 * \brief Very simple gate to watch a counter, i.e. see if it has changed.
 *
 * The `counter_watchdog` enables you to "watch" a counter. Whenever it is
 * called, it checks if the `counter`'s value has change since the last time
 * the `counter_watchdog` was called.
 *
 * **Usage**
 * \include make_counter.cc
 *
 * For further examples, look into the tests.
 */
class counter_watchdog
{
public:
  using value_t = typename counter::value_t;

public:
  /*!
   * \brief Constructor for `counter_watchdog` class.
   * \param c The counter to watch.
   */
  counter_watchdog(counter & c) : _counter(c), _value(_counter()) {}

  /*!
   * \brief Move copy constructor for `counter_watchdog` class.
   * \param other Universal reference to object to acquire ownership of.
   *
   * This constructor is used in the generator function `make_counter_watchdog`
   * but shouldn't otherwise be used (unless of course you have a very good
   * reason to do so). It is compiler generated.
   */
  counter_watchdog(counter_watchdog && cwdg) = default;

  /*!
   * \brief   Get current value of `counter_watchdog`.
   * \returns Current value of the gate.
   *
   * Every scruBB gate is callable. When called, it calls the input object and
   * takes appropriate action with the result.
   */
  bool operator()() noexcept {
    auto val = _counter();

    if (_value != val) {
      _value = val;
      return true;
    } else {
      return false;
    }
  }

private:
  counter & _counter;
  value_t   _value;
};


/*!
 * \brief   Maker function to facilitate creating `counter_watchdog` objects.
 * \returns Ready to use `counter` object.
 *
 * A `counter` object can be instantiated by calling the constructor of the
 * `counter` class. This function is provided to ensure consistency with the
 * other `scruBB` gates, thus making usage uniform and expectable.
 *
 * **Usage**
 * \include make_counter.cc
 */
inline counter_watchdog make_counter_watchdog(counter & c) noexcept {
  return {c};
}

}  // namespace scrubb

#endif  // SCRUBB_CORE_COUNTER_H_
