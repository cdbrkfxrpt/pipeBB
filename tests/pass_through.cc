// Copyright 2017 Ingenieurbüro Krug <info@ingenieurbuero-krug.de>
//
// Maintainer: Florian Eich <florian.eich@ingenieurbuero-krug.de>
//
// All rights reserved.

#include <iostream>

#include "catch.h"

#include "channel.h"
#include "pass_through.h"
#include "threshold.h"


//
// explicitly instantiate class to make sure compiler generates the class fully
// (enables meaningful test coverage analysis)
//
template class scrubb::pass_through<
  scrubb::channel<double>,
  scrubb::threshold<scrubb::channel<double>>>;
template class scrubb::threshold_pass_through<scrubb::channel<double>>;
template class scrubb::buffered_pass_through<
  scrubb::channel<double>,
  scrubb::threshold<scrubb::channel<double>>>;
//


TEST_CASE("functionality of pass through gates", "[pass_through]") {
  scrubb::channel<double> p_manifold{"p_manifold", "mbar", 1.0, 0.0};
  scrubb::threshold<decltype(p_manifold)> thresh{p_manifold, 2570.0};
  p_manifold << 2500.0;

  SECTION("pass_through") {
    scrubb::pass_through<decltype(p_manifold), decltype(thresh)> pass_through{
      p_manifold, thresh};

    REQUIRE(!pass_through());

    p_manifold << 2600.0;
    REQUIRE(pass_through() == p_manifold());
  }

  SECTION("threshold_pass_through") {
    scrubb::threshold_pass_through<decltype(p_manifold)> pass_through{
      p_manifold, 2550.0};

    REQUIRE(!pass_through());

    p_manifold << 2600.0;
    REQUIRE(pass_through() == p_manifold());
  }

  SECTION("buffered_pass_through") {
    scrubb::buffered_pass_through<decltype(p_manifold), decltype(thresh)>
      buffered_pass_through{p_manifold, thresh};

    REQUIRE(!buffered_pass_through());

    p_manifold << 2600.0;
    REQUIRE(buffered_pass_through() == p_manifold());
  }
}
