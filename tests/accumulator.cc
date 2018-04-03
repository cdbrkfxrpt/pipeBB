// Copyright 2017 Ingenieurbüro Krug <info@ingenieurbuero-krug.de>
//
// Maintainer: Florian Eich <florian.eich@ingenieurbuero-krug.de>
//
// All rights reserved.

#include <cmath>

#include "catch.h"

#include "accumulator.h"
#include "channel.h"


//
// explicitly instantiate class to make sure compiler generates the class fully
// (enables meaningful test coverage analysis)
//
template class scrubb::accumulator<scrubb::channel<double>, 5>;
//


TEST_CASE("functionality of the accumulator gates", "[accumulator]") {
  scrubb::channel<double> overboost{"overboost", "mbar", 1.0, 0.0};

  SECTION("accumulator") {
    auto acc = scrubb::make_accumulator<5>(overboost);

    //
    // The following usage is possible, but riddles the code with "decltype":
    //
    scrubb::accumulator<decltype(overboost), 5> ctot_test{overboost};
    //

    overboost << 12.5;

    REQUIRE(acc() == 12.5);
    REQUIRE(acc() == 25.0);

    overboost << 5.0;

    acc();
    acc();

    REQUIRE(acc() == 40.0);
    REQUIRE(acc() == 32.5);
    REQUIRE(acc() == 25.0);
    REQUIRE(acc() == 25.0);
  }
}
