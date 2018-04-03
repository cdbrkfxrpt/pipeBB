// Copyright 2017 Ingenieurbüro Krug <info@ingenieurbuero-krug.de>
//
// Maintainer: Florian Eich <florian.eich@ingenieurbuero-krug.de>
//
// All rights reserved.

#include <cmath>

#include "catch.h"

#include "channel.h"
#include "inverter.h"


//
// explicitly instantiate class to make sure compiler generates the class fully
// (enables meaningful test coverage analysis)
//
template class scrubb::inverter<scrubb::channel<double>>;
//


TEST_CASE("functionality of the inverter gate", "[inverter]") {
  scrubb::channel<double> p_manifold{"p_manifold", "mbar", 1.0, 0.0};

  SECTION("offset") {
    scrubb::inverter<decltype(p_manifold)> inv{p_manifold};

    p_manifold << 2500.0;

    REQUIRE(inv() == -2500.0);
  }
}
