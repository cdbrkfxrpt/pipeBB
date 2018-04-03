// Copyright 2017 Ingenieurbüro Krug <info@ingenieurbuero-krug.de>
//
// Maintainer: Florian Eich <florian.eich@ingenieurbuero-krug.de>
//
// All rights reserved.

#include <algorithm>
#include <cmath>
#include <iostream>

#include "catch.h"

#include "channel.h"


//
// explicitly instantiate class to make sure compiler generates the class fully
// (enables meaningful test coverage analysis)
//
template class scrubb::channel<double>;
//


TEST_CASE("basic functionality of channel container", "[channel_basics]") {
  scrubb::channel<double> chan{"test", "m", 1.0, 0.0};
  scrubb::channel<double> sec{"sec", "m", 1.0, 0.0};

  SECTION("constructors") {
    //
    // the following objects are only initialized but not ever used.
    scrubb::channel<uint32_t> default_ctor_1;
    scrubb::channel<double>   default_ctor_2;
    scrubb::channel<uint32_t> name_only_ctor_1{"name only"};
    scrubb::channel<double>   name_only_ctor_2{"name only"};
    //

    REQUIRE(chan.factor() == 1.0);
    REQUIRE(chan.offset() == 0.0);


    chan.factor(1.5);
    chan.offset(0.7);

    REQUIRE(chan.factor() == 1.5);
    REQUIRE(chan.offset() == 0.7);


    chan.factor(&sec);
    chan.offset(&sec);

    REQUIRE(chan.dynamic_factor() == &sec);
    REQUIRE(chan.dynamic_offset() == &sec);
  }

  SECTION("simulated operation") {
    for (unsigned i = 0; i < 5; ++i) { chan << static_cast<double>(i); }

    REQUIRE(chan() == static_cast<double>(4));

    chan.factor(&sec);
    chan.offset(&sec);

    for (unsigned i = 0; i < 5; ++i) {
      sec << static_cast<double>(i);
      chan << static_cast<double>(i);
      REQUIRE(chan() == static_cast<double>((i + i) * i));
    }
  }
}
