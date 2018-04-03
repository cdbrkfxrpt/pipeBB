// Copyright 2017 Ingenieurbüro Krug <info@ingenieurbuero-krug.de>
//
// Maintainer: Florian Eich <florian.eich@ingenieurbuero-krug.de>
//
// All rights reserved.

#include <cmath>

#include "catch.h"

#include "channel.h"
#include "gradient.h"


//
// explicitly instantiate class to make sure compiler generates the class fully
// (enables meaningful test coverage analysis)
//
template class scrubb::gradient<scrubb::channel<double>>;
//


TEST_CASE("functionality of the gradient gates", "[gradient]") {
  scrubb::channel<double> p_manifold{"p_manifold", "mbar", 1.0, 0.0};

  SECTION("gradient") {
    scrubb::gradient<decltype(p_manifold)> grad{p_manifold};

    p_manifold << 2500.0;
    REQUIRE(grad() == 2500.0);

    p_manifold << 2570.0;
    REQUIRE(grad() == 70.0);

    p_manifold << 2370.0;
    REQUIRE(grad() == -200.0);
  }
}
