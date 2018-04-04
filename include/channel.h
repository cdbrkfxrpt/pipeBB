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

#ifndef PIPEBB_CHANNEL_H_
#define PIPEBB_CHANNEL_H_

#include <ostream>
#include <sstream>
#include <string>

#include "utils.h"


namespace pipebb {


/* forward declarations ---------------------------------------------------- */
template <typename T, REQUIRES(std::is_arithmetic<T>())>
class channel;

template <typename T>
std::ostream & operator<<(std::ostream &, const channel<T> &);
/* ------------------------------------------------------------------------- */


/*!
 * \brief Class which acts as external data interface for pipeBB code.
 * \param T Template parameter specifying the data type accepted by the
 *        channel. Must be arithmetic.
 *
 * The channel gate is the external data interface for pipeBB code. In user
 * code, this is "where it begins": the user creates channel objects for all
 * his data sources and connects other pipeBB elements up to them.
 *
 * **Usage**
 * \include channel.cc
 *
 * For further examples, look into the tests.
 */
template <typename T, REQUIRES(std::is_arithmetic<T>())>
class channel
{
public:
  using self_t  = channel<T>;
  using value_t = T;

  using value_type      = value_t;
  using size_type       = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference       = value_t &;
  using const_reference = const value_t &;
  using pointer         = value_t *;
  using const_pointer   = const value_t *;

public:
  /*!
   * \brief Constructor for the channel class.
   * \param name Name of the channel.
   * \param unit Unit for the channel.
   * \param factor Factor applied to incoming data.
   * \param offset Offset applied to incoming data.
   */
  explicit channel(std::string name,
                   std::string unit,
                   value_t     factor,
                   value_t     offset) noexcept
   : _name(name), _unit(unit), _factor(factor), _offset(offset) {}

  /*!
   * \brief Simplified constructor for the channel class.
   * \param name Nmae of the channel.
   *
   * Using this constructor, the channel will have an empty unit, the factor
   * will be 1.0 and the offset will be 0.0.
   */
  explicit channel(std::string name) noexcept : _name(name) {}

  /*!
   * \brief Default constructor for the channel class.
   */
  explicit channel() = default;

  /*! \name Getters */ /*!@{*/
  /*! Getter. */       /* -------------------------------------------------- */
  std::string    name() const noexcept { return _name; }
  std::string    unit() const noexcept { return _unit; }
  value_t        factor() const noexcept { return _factor; }
  const self_t * dynamic_factor() const noexcept { return _dynamic_factor; }
  value_t        offset() const noexcept { return _offset; }
  const self_t * dynamic_offset() const noexcept { return _dynamic_offset; }
  /*!@}*/ /* --------------------------------------------------------------- */

  /*! \name Setters */ /*!@{*/
  /*! Setter. */       /* -------------------------------------------------- */
  void factor(value_t factor) noexcept { _factor = factor; }
  void factor(self_t * other) noexcept { _dynamic_factor = other; }
  void offset(value_t offset) noexcept { _offset = offset; }
  void offset(self_t * other) noexcept { _dynamic_offset = other; }
  /*!@}*/ /* --------------------------------------------------------------- */

  /*!
   * \brief Stream operator for the channel class.
   * \param Data value.
   *
   * This is implemented so you can use channel objects like so:
   *
   * `my_channel << my_data_value;`
   */
  void operator<<(value_t value) noexcept {
    if (value != _raw_val) {
      _raw_val = value;
      _updated = true;
    }
  }

  /*!
   * \brief   Retrieve data currently stored in channel.
   * \returns Normalized data value.
   *
   * Every pipeBB class implements operator(). In the channel object, it is
   * first checked if the stored value has been updated; if so, the
   * normalization factor (static and/or from another channel or gate) and the
   * normalization offset (static and/or from another channel or gate) are
   * applied, then the normalized value is returned.
   */
  value_t operator()() noexcept {
    if (_updated) {
      _updated = false;

      value_t factor = _factor;
      if (_dynamic_factor) { factor *= _dynamic_factor->operator()(); }

      value_t offset = _offset;
      if (_dynamic_offset) { offset += _dynamic_offset->operator()(); }

      _out_val = (_raw_val + offset) * factor;
    }

    return _out_val;
  }

private:
  template <typename U>
  friend std::ostream & operator<<(std::ostream &, channel<U> &);

private:
  std::string _name;
  std::string _unit;
  value_t     _factor{1};
  self_t *    _dynamic_factor{nullptr};
  value_t     _offset{0};
  self_t *    _dynamic_offset{nullptr};
  value_t     _raw_val;
  value_t     _out_val;
  bool        _updated{false};
};


/*!
 * \brief   Specialization of the channel template for bool (channel<bool>).
 *
 * The normalization using a factor and an offset inside the channel object
 * is not viable when dealing with a channel<bool> object. Hence, this
 * specialization removes the unnecessary logic from the channel object.
 */
template <>
class channel<bool>
{
public:
  using self_t  = channel<bool>;
  using value_t = bool;

  using value_type      = value_t;
  using size_type       = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference       = value_t &;
  using const_reference = const value_t &;
  using pointer         = value_t *;
  using const_pointer   = const value_t *;

public:
  /*!
   * \brief Simplified constructor for the channel class.
   * \param name Nmae of the channel.
   */
  explicit channel(std::string name) noexcept : _name(name) {}

  /*!
   * \brief Default constructor for the channel class.
   */
  explicit channel() = default;

  /*! \name Getters */ /*!@{*/
  /*! Getter. */       /* -------------------------------------------------- */
  std::string name() const noexcept { return _name; }
  std::string unit() const noexcept { return _unit; }
  /*!@}*/ /* --------------------------------------------------------------- */

  /*!
   * \brief Stream operator for the channel class.
   * \param Data value.
   *
   * This is implemented so you can use channel objects like so:
   *
   * `my_channel << my_data_value;`
   */
  void operator<<(value_t value) noexcept {
    if (value != _raw_val) {
      _raw_val = value;
      _updated = true;
    }
  }

  /*!
   * \brief   Retrieve data currently stored in channel.
   * \returns Boolean data value.
   */
  value_t operator()() noexcept {
    if (_updated) {
      _updated = false;
      _out_val = _raw_val;
    }
    return _out_val;
  }

private:
  friend std::ostream & operator<<(std::ostream &, channel<bool> &);

private:
  std::string _name;
  std::string _unit;
  value_t     _raw_val;
  value_t     _out_val;
  bool        _updated{false};
};


/*!
 * \brief   Implementation of stream operator on channel object.
 * \param   T Template parameter specifying data type in channel.
 * \param   os std::ostream for the operator to output to.
 * \param   chan channel for the operator to read from.
 * \returns std::ostream reference.
 *
 * This ensures you can use the channel with `std::ostream`s, like so:
 *
 * `std::cout << my_channel;`
 *
 * This will pretty print all information about the channel.
 */
template <typename T>
std::ostream & operator<<(std::ostream & os, channel<T> & chan) {
  std::ostringstream ss;

  auto unit    = (chan._unit.size() ? chan._unit : "none");
  auto dyn_fac = (chan._dynamic_factor ? chan._dynamic_factor->_name : "none");
  auto dyn_off = (chan._dynamic_offset ? chan._dynamic_offset->_name : "none");

  ss << "channel {\n  "
     << "name:           " << chan._name << ",\n  "
     << "unit:           " << unit << ",\n  "
     << "factor:         " << chan._factor << ",\n  "
     << "dynamic factor: " << dyn_fac << ",\n  "
     << "offset:         " << chan._offset << ",\n  "
     << "dynamic offset: " << dyn_off << ",\n  "
     << "current value:  " << chan() << '\n'
     << "}\n";

  return operator<<(os, ss.str());
}


/*!
 * \brief   Specialization of stream operator on channel<bool> object.
 * \param   os std::ostream for the operator to output to.
 * \param   chan channel for the operator to read from.
 * \returns std::ostream reference.
 *
 * This ensures you can use the channel with `std::ostream`s, like so:
 *
 * `std::cout << my_channel;`
 *
 * This will pretty print all information about the channel.
 */
inline std::ostream & operator<<(std::ostream & os, channel<bool> & chan) {
  std::ostringstream ss;

  auto unit = (chan._unit.size() ? chan._unit : "none");

  ss << "channel {\n  "
     << "name:           " << chan._name << ",\n  "
     << "unit:           " << unit << ",\n  "
     << "current value:  " << chan() << '\n'
     << "}\n";

  return operator<<(os, ss.str());
}

}  // namespace pipebb

#endif  // PIPEBB_CHANNEL_H_
