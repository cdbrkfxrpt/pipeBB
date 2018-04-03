// Copyright 2017 Ingenieurbüro Krug <info@ingenieurbuero-krug.de>
//
// Maintainer: Florian Eich <florian.eich@ingenieurbuero-krug.de>
//
// All rights reserved.

#include <cmath>

#include "catch.h"

#include "channel.h"
#include "factor.h"


//
// explicitly instantiate class to make sure compiler generates the class fully
// (enables meaningful test coverage analysis)
//
template class scrubb::factor<scrubb::channel<double>>;
//


TEST_CASE("functionality of the grading gates", "[grading]") {
  scrubb::channel<double> p_manifold{"p_manifold", "mbar", 1.0, 0.0};

  SECTION("factor") {
    scrubb::factor<decltype(p_manifold)> mul_fac{p_manifold, 2.0};
    scrubb::factor<decltype(p_manifold)> div_fac{p_manifold, 0.5};

    p_manifold << 2200.0;

    REQUIRE(mul_fac() == 4400.0);
    REQUIRE(div_fac() == 1100.0);

    mul_fac.set_constant(3.0);
    div_fac.set_constant(0.25);

    REQUIRE(mul_fac() == 6600.0);
    REQUIRE(div_fac() == 550.0);
  }
}
