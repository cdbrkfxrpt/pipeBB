// Copyright 2017 Ingenieurbüro Krug <info@ingenieurbuero-krug.de>
//
// Maintainer: Florian Eich <florian.eich@ingenieurbuero-krug.de>
//
// All rights reserved.

#include <cmath>

#include "catch.h"

#include "channel.h"
#include "moving_average.h"


//
// explicitly instantiate class to make sure compiler generates the class fully
// (enables meaningful test coverage analysis)
//
template class scrubb::moving_average<scrubb::channel<double>, 50>;
//


TEST_CASE("functionality of the moving_average gates", "[moving_average]") {
  scrubb::channel<double> p_manifold{"p_manifold", "mbar", 1.0, 0.0};

  SECTION("moving_average") {
    const unsigned S = 5;
    scrubb::moving_average<decltype(p_manifold), S> mavg{p_manifold};

    p_manifold << 2500.0;

    REQUIRE(mavg() == p_manifold() / S);
    REQUIRE(mavg() == 2 * p_manifold() / S);
    REQUIRE(mavg() == 3 * p_manifold() / S);
    REQUIRE(mavg() == 4 * p_manifold() / S);
    REQUIRE(mavg() == 5 * p_manifold() / S);

    REQUIRE(mavg() == 5 * p_manifold() / S);
    REQUIRE(mavg() == 5 * p_manifold() / S);

    mavg.reset();
    REQUIRE(mavg() == 500.0);
  }
}
