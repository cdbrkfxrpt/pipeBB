// Copyright 2017 Ingenieurbüro Krug <info@ingenieurbuero-krug.de>
//
// Maintainer: Florian Eich <florian.eich@ingenieurbuero-krug.de>
//
// All rights reserved.

#include <cmath>

#include "catch.h"

#include "channel.h"
#include "offset.h"


//
// explicitly instantiate class to make sure compiler generates the class fully
// (enables meaningful test coverage analysis)
//
template class scrubb::offset<scrubb::channel<double>>;
//


TEST_CASE("functionality of the offset gates", "[offset]") {
  scrubb::channel<double> p_manifold{"p_manifold", "mbar", 1.0, 0.0};

  SECTION("offset") {
    scrubb::offset<decltype(p_manifold)> add_off{p_manifold, 200.0};
    scrubb::offset<decltype(p_manifold)> sub_off{p_manifold, -200.0};

    p_manifold << 2500.0;

    REQUIRE(add_off() == 2700.0);
    REQUIRE(sub_off() == 2300.0);

    add_off.set_offset(150.0);
    sub_off.set_offset(-150.0);

    REQUIRE(add_off() == 2650.0);
    REQUIRE(sub_off() == 2350.0);
  }
}
