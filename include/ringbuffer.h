/* ------------------------------------------------------------------------- *
 * Copyright 2017 Ingenieurbüro Krug. All rights reserved.
 *
 * Author: Florian Eich <florian.eich@ingenieurbuero-krug.de>
 * ------------------------------------------------------------------------- */

#ifndef SCRUBB_CORE_RINGBUFFER_H_
#define SCRUBB_CORE_RINGBUFFER_H_

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>


namespace scrubb {


/* forward declarations ---------------------------------------------------- */
template <typename T, std::size_t N>
class ringbuffer;

template <typename T, std::size_t N>
std::ostream & operator<<(std::ostream &, const ringbuffer<T, N> &);
/* ------------------------------------------------------------------------- */


namespace detail {

template <class ringbuffer_t>
class ringbuffer_iterator
{
  using self_t  = ringbuffer_iterator<ringbuffer_t>;
  using index_t = std::size_t;

  using value_t = typename ringbuffer_t::value_t;

public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type        = typename ringbuffer_t::value_t;
  using difference_type   = std::ptrdiff_t;
  using pointer           = self_t *;
  using reference         = self_t &;

public:
  ringbuffer_iterator(ringbuffer_t & rb, index_t pos) : _rb(rb), _pos(pos) {}

  self_t & operator=(const self_t & other) noexcept {
    _rb  = other._rb;
    _pos = other._pos;

    return *this;
  }

  const value_t & operator*() const noexcept {
    return _rb[_pos - _rb._begin_pos];
  }

  const value_t * operator->() const noexcept {
    return &_rb[_pos - _rb._begin_pos];
  }

  self_t & operator++() noexcept {
    _pos == _rb._capacity ? _pos = 0 : ++_pos;
    return *this;
  }

  self_t operator++(int)noexcept {
    self_t tmp{*this};
    operator++();
    return tmp;
  }

  self_t & operator--() noexcept {
    _pos == 0 ? _pos = _rb._capacity : --_pos;
    return *this;
  }

  self_t operator--(int)noexcept {
    self_t tmp(*this);
    operator--();
    return tmp;
  }

  bool operator==(const self_t & other) const noexcept {
    return _rb == other._rb && _pos == other._pos;
  }

  bool operator!=(const self_t & other) const noexcept {
    return !(*this == other);
  }

  std::size_t getPos() noexcept { return _pos; }

private:
  ringbuffer_t _rb;
  index_t      _pos;
};

}  // namespace detail


template <typename T, std::size_t N>
class ringbuffer
{
  using self_t  = ringbuffer<T, N>;
  using value_t = T;

public:
  using value_type      = value_t;
  using size_type       = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference       = value_t &;
  using const_reference = const value_t &;
  using pointer         = value_t *;
  using const_pointer   = const value_t *;
  using iterator        = detail::ringbuffer_iterator<self_t>;
  using const_iterator  = detail::ringbuffer_iterator<const self_t>;

  friend iterator;
  friend const_iterator;

public:
  // friend void swap(self_t & first, self_t & second) {
  //   using std::swap;

  //   swap(first._pos, second._pos);
  //   swap(first._capacity, second._capacity);
  //   swap(first._data, second._data);
  // }

  /* Element access -------------------------------------------------------- */
  void push(value_t value) noexcept {
    _data[_end_pos] = value;

    if (_size == _capacity) {
      // check if we have reached the end of array, if yes jump to beginning
      _end_pos == _capacity ? _end_pos = 0 : ++_end_pos;
      // check if _end_pos points to last element now, if yes reset _begin_pos
      // THIS IS NOT THE SAME AS ABOVE! _end_pos MAY HAVE BEEN INCREMENTED!
      _end_pos == _capacity ? _begin_pos = 0 : ++_begin_pos;
    } else {
      // when array is filled, reset both _pos. before, only inc _end_pos.
      if (_end_pos == _capacity) {
        _end_pos   = 0;
        _begin_pos = 1;
      } else {
        ++_end_pos;
        ++_size;
      }
    }

    _data[_end_pos] = value_t();
  }

  void operator<<(value_t value) noexcept { this->push(value); }

  reference at(std::size_t i) noexcept { this->operator[](i); }

  reference operator[](const std::size_t i) noexcept {
    return _data[(_begin_pos + i) % (_capacity + 1)];
  }

  const_reference operator[](const std::size_t i) const noexcept {
    return _data[(_begin_pos + i) % (_capacity + 1)];
  }

  const_reference front() const noexcept { return _data[_begin_pos]; }

  const_reference back() const noexcept {
    return (_end_pos == 0 ? _data[_capacity] : _data[_end_pos - 1]);
  }

  pointer data() noexcept { return _data.data(); }

  const std::array<T, N + 1> & raw() const noexcept { return _data; }
  /* ----------------------------------------------------------------------- */

  /* Iterator -------------------------------------------------------------- */
  iterator begin() noexcept { return iterator(*this, _begin_pos); }

  const_iterator begin() const noexcept {
    return const_iterator(*this, _begin_pos);
  }

  const_iterator cbegin() const noexcept {
    return const_iterator(*this, _begin_pos);
  }

  iterator end() noexcept { return iterator(*this, _end_pos); }

  const_iterator end() const noexcept {
    return const_iterator(*this, _end_pos);
  }

  const_iterator cend() const noexcept {
    return const_iterator(*this, _end_pos);
  }

  iterator rbegin() noexcept { return --(this->end()); }

  const_iterator rbegin() const noexcept { return --(this->end()); }

  const_iterator crbegin() const noexcept { return --(this->end()); }

  iterator rend() noexcept { return this->end(); }

  const_iterator rend() const noexcept { return this->end(); }

  const_iterator crend() const noexcept { return this->end(); }
  /* ----------------------------------------------------------------------- */

  /* Capacity -------------------------------------------------------------- */
  bool empty() const noexcept { return _size == 0; }

  std::size_t size() const noexcept { return _size; }

  std::size_t max_size() const noexcept { return _capacity; }
  /* ----------------------------------------------------------------------- */

  /* Operations ------------------------------------------------------------ */
  void fill(value_t value) noexcept {
    for (std::size_t i = 0; i < _capacity; ++i) { push(value); }
  }
  /* ----------------------------------------------------------------------- */

  /* Comparison ------------------------------------------------------------ */
  bool operator==(const self_t & other) const noexcept {
    if (this->_capacity != other._capacity) { return false; }

    auto this_it  = this->begin();
    auto other_it = other.begin();

    for (unsigned i = 0; i < _capacity; ++i, ++this_it, ++other_it) {
      if (*this_it != *other_it) { return false; }
    }
    return true;
  }

  bool operator!=(const self_t & other) const noexcept {
    return !(*this == other);
  }
  /* ----------------------------------------------------------------------- */

private:
  std::size_t _begin_pos{0};
  std::size_t _end_pos{0};
  std::size_t _capacity{N};
  std::size_t _size{0};
  std::array<value_t, N + 1> _data{};
};


template <typename T, std::size_t N>
std::ostream & operator<<(std::ostream & os, const ringbuffer<T, N> & rb) {
  std::ostringstream ss;

  ss << "[";

  for (auto & element : rb) { ss << element << ", "; }

  // if elements have been written, get rid of the comma at the end
  if (rb.size()) { ss.seekp(-2, std::ios_base::end); }

  ss << "]";

  return operator<<(os, ss.str());
}

}  // namespace scrubb

#endif  // SCRUBB_CORE_RINGBUFFER_H_
