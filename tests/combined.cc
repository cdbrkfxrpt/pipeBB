// Copyright 2017 Ingenieurbüro Krug <info@ingenieurbuero-krug.de>
//
// Maintainer: Florian Eich <florian.eich@ingenieurbuero-krug.de>
//
// All rights reserved.

#include "catch.h"

#include "channel.h"
#include "gradient.h"
#include "threshold.h"


TEST_CASE("combined gates functionality", "[combined]") {
  SECTION("comparative") {
    scrubb::channel<unsigned>          n_rpm{"n_rpm", "unit", 1, 0};
    scrubb::threshold<decltype(n_rpm)> thresh{n_rpm, 3000};

    n_rpm << 2500;
    REQUIRE(!thresh());

    n_rpm << 3200;
    REQUIRE(thresh());
  }

  SECTION("flow control") {}

  SECTION("block combinations") {
    scrubb::channel<double> fun{"fun", "rad", 1, 0};

    scrubb::gradient<decltype(fun)>   grad{fun};
    scrubb::threshold<decltype(grad)> thresh{grad, 50};

    fun << 200.0;
    REQUIRE(thresh());
    REQUIRE(!thresh());
  }

  SECTION("varstep_gradient") {
    scrubb::channel<double> fun{"fun", "rad", 1, 0};

    scrubb::varstep_gradient<decltype(fun), 3> grad{fun};

    fun << 200.0;
    REQUIRE(grad() == 200.0);
    REQUIRE(grad() == 200.0);
    REQUIRE(grad() == 0);
  }
}
