// Copyright 2017 Ingenieurbüro Krug <info@ingenieurbuero-krug.de>
//
// Maintainer: Florian Eich <florian.eich@ingenieurbuero-krug.de>
//
// All rights reserved.

#include "catch.h"

#include "buffer.h"
#include "channel.h"
#include "counter.h"
#include "resetter.h"
#include "threshold.h"


//
// explicitly instantiate class to make sure compiler generates the class fully
// (enables meaningful test coverage analysis)
//
template class scrubb::resetter<scrubb::channel<double>, scrubb::counter>;
//


TEST_CASE("functionality of resetter gates", "[resetter]") {
  scrubb::channel<double> p_manifold{"p_manifold", "mbar", 1.0, 0.0};
  scrubb::threshold<decltype(p_manifold)> thresh{p_manifold, 2570.0};

  p_manifold << 2500.0;

  SECTION("resetter") {
    scrubb::counter counter;
    scrubb::resetter<decltype(thresh), decltype(counter)> resetter{thresh,
                                                                   counter};

    for (unsigned i = 0; i < 5; ++i) {
      counter.step();
      resetter();
    }

    REQUIRE(counter() == 5);

    p_manifold << 2600.0;

    REQUIRE(resetter());

    REQUIRE(counter() == 0);
  }
}
