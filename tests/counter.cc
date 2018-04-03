// Copyright 2017 Ingenieurbüro Krug <info@ingenieurbuero-krug.de>
//
// Maintainer: Florian Eich <florian.eich@ingenieurbuero-krug.de>
//
// All rights reserved.

#include "catch.h"

#include "channel.h"
#include "counter.h"
#include "threshold.h"


//
// explicitly instantiate class to make sure compiler generates the class fully
// (enables meaningful test coverage analysis)
//
template class scrubb::reset_counter<scrubb::channel<double>>;
template class scrubb::boolean_counter<scrubb::channel<double>>;
//


TEST_CASE("functionality of counters", "[counters]") {
  SECTION("counter") {
    scrubb::counter counter;
    REQUIRE(counter() == 0);
    REQUIRE(counter.step() == 1);
    REQUIRE(counter() == 1);
    REQUIRE(counter.step() == 2);

    REQUIRE(++counter == 3);
    REQUIRE(counter++ == 4);
    REQUIRE(counter() == 4);
  }

  scrubb::channel<double> p_manifold{"p_manifold", "mbar", 1.0, 0.0};
  scrubb::threshold<decltype(p_manifold)> thresh{p_manifold, 2570.0};

  SECTION("resettable_counter") {
    scrubb::reset_counter<decltype(thresh)> rcounter{thresh};

    p_manifold << 2500.0;

    for (unsigned i = 1; i < 6; ++i) { REQUIRE(rcounter() == i); }

    p_manifold << 2600.0;

    for (unsigned i = 0; i < 5; ++i) { REQUIRE(rcounter() == 0); }
  }

  SECTION("boolean_counter") {
    scrubb::boolean_counter<decltype(thresh)> count{thresh};

    p_manifold << 2500.0;

    REQUIRE(count() == 0);

    p_manifold << 2500.0;

    REQUIRE(count() == 0);

    p_manifold << 2600.0;

    REQUIRE(count() == 1);
    REQUIRE(count() == 2);
    REQUIRE(count() == 3);

    p_manifold << 2500.0;

    REQUIRE(count() == 3);
  }

  SECTION("counter_watchdog") {
    scrubb::counter          counter;
    scrubb::counter_watchdog twd{counter};

    REQUIRE(!twd());

    counter.step();

    REQUIRE(twd());
    REQUIRE(!twd());
  }
}
