// Copyright 2017 Ingenieurbüro Krug <info@ingenieurbuero-krug.de>
//
// Maintainer: Florian Eich <florian.eich@ingenieurbuero-krug.de>
//
// All rights reserved.

#include <algorithm>
#include <iostream>

#include "catch.h"

#include "ringbuffer.h"

constexpr int RBSIZE = 16;


TEST_CASE("basic functionality of ringbuffer container",
          "[ringbuffer_basics]") {
  scrubb::ringbuffer<int, RBSIZE> buf;

  SECTION("constructors") {
    REQUIRE(buf.empty());
    REQUIRE(buf.size() == 0);
    REQUIRE(buf.max_size() == RBSIZE);

    scrubb::ringbuffer<int, RBSIZE> other_buf(buf);

    REQUIRE(buf == other_buf);
    REQUIRE(!(buf != other_buf));

    REQUIRE(buf.begin() == buf.end());
  }

  SECTION("manipulators") {
    buf.push(5);
    buf.push(6);

    REQUIRE(*(buf.begin()) == 5);
    REQUIRE(*(buf.rbegin()) == 6);
    REQUIRE(*(buf.end()) == int());
    REQUIRE(*(buf.rend()) == int());

    REQUIRE(buf.front() == 5);
    REQUIRE(buf.back() == 6);

    for (int i = 0; i < RBSIZE; ++i) {
      // buf.push(i);  // alternative option
      buf << i;
      REQUIRE(buf.back() == i);
    }

    REQUIRE(*(buf.begin()) == buf.front());
    REQUIRE(*(buf.end()) == int());

    int i = 0;
    std::for_each(buf.begin(), buf.end(), [&i](auto element) {
      REQUIRE((element == i));
      // increment i and mod by RBSIZE + 1
      ++i %= RBSIZE + 1;
    });

    buf.fill(42);
    std::for_each(
      buf.begin(), buf.end(), [](auto element) { REQUIRE((element == 42)); });
  }
}
