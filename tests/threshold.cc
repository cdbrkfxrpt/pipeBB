// Copyright 2017 Ingenieurbüro Krug <info@ingenieurbuero-krug.de>
//
// Maintainer: Florian Eich <florian.eich@ingenieurbuero-krug.de>
//
// All rights reserved.

#include "catch.h"

#include "channel.h"
#include "threshold.h"


//
// explicitly instantiate class to make sure compiler generates the class fully
// (enables meaningful test coverage analysis)
//
template class scrubb::threshold<scrubb::channel<double>>;
//


TEST_CASE("functionality of threshold gates", "[threshold]") {
  SECTION("threshold") {
    scrubb::channel<unsigned>          n_rpm{"n_rpm", "unit", 1, 0};
    scrubb::threshold<decltype(n_rpm)> thresh{n_rpm, 3000};

    n_rpm << 2500;
    REQUIRE(!thresh());

    n_rpm << 3200;
    REQUIRE(thresh());
  }
}
