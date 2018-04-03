// Copyright 2017 Ingenieurbüro Krug <info@ingenieurbuero-krug.de>
//
// Maintainer: Florian Eich <florian.eich@ingenieurbuero-krug.de>
//
// All rights reserved.

#include "catch.h"

#include "channel.h"
#include "logical.h"
#include "threshold.h"

//
// explicitly instantiate class to make sure compiler generates the class fully
// (enables meaningful test coverage analysis)
//
template class scrubb::logic_gate<std::logical_and<bool>,
                                  scrubb::threshold<scrubb::channel<bool>>,
                                  scrubb::threshold<scrubb::channel<bool>>>;
template class scrubb::and_gate<scrubb::threshold<scrubb::channel<bool>>,
                                scrubb::threshold<scrubb::channel<bool>>>;
template class scrubb::or_gate<scrubb::threshold<scrubb::channel<bool>>,
                               scrubb::threshold<scrubb::channel<bool>>>;
template class scrubb::not_gate<scrubb::threshold<scrubb::channel<bool>>>;
//

TEST_CASE("functionality of the logic gates", "[logic]") {
  scrubb::channel<unsigned>          n_rpm{"n_rpm", "unit", 1, 0};
  scrubb::threshold<decltype(n_rpm)> thresh_rpm{n_rpm, 3000};

  scrubb::channel<double> alpha_throttle{"alpha_trottle", "rad", 1.0, 0.0};
  scrubb::threshold<decltype(alpha_throttle)> thresh_throttle{alpha_throttle,
                                                              0.5};

  scrubb::channel<double>              T_water{"T_water", "deg", 1.0, 0.0};
  scrubb::threshold<decltype(T_water)> thresh_watertemp{T_water, 120.0};

  SECTION("logic_gate") {
    scrubb::logic_gate<std::logical_and<bool>,
                       decltype(thresh_rpm),
                       decltype(thresh_throttle)>
      lgt{thresh_rpm, thresh_throttle};

    scrubb::logic_gate<std::logical_and<bool>,
                       decltype(thresh_rpm),
                       decltype(thresh_throttle),
                       decltype(thresh_watertemp)>
      lgd{thresh_rpm, thresh_throttle, thresh_watertemp};

    n_rpm << 2500;
    alpha_throttle << 0.2;
    T_water << 114.0;

    REQUIRE(!lgt());
    REQUIRE(!lgd());

    n_rpm << 3200;
    T_water << 124.0;

    REQUIRE(!lgt());
    REQUIRE(!lgd());

    alpha_throttle << 0.9;

    REQUIRE(lgt());
    REQUIRE(lgd());
  }

  SECTION("and_gate") {
    scrubb::and_gate<decltype(thresh_rpm), decltype(thresh_throttle)> agt{
      thresh_rpm, thresh_throttle};

    scrubb::and_gate<decltype(thresh_rpm),
                     decltype(thresh_throttle),
                     decltype(thresh_watertemp)>
      agd{thresh_rpm, thresh_throttle, thresh_watertemp};

    n_rpm << 2500;
    alpha_throttle << 0.2;
    T_water << 114.0;

    REQUIRE(!agt());
    REQUIRE(!agd());

    n_rpm << 3200;
    T_water << 124.0;

    REQUIRE(!agt());
    REQUIRE(!agd());

    alpha_throttle << 0.9;

    REQUIRE(agt());
    REQUIRE(agd());
  }

  SECTION("or_gate") {
    scrubb::or_gate<decltype(thresh_rpm), decltype(thresh_throttle)> ogt{
      thresh_rpm, thresh_throttle};

    scrubb::or_gate<decltype(thresh_rpm),
                    decltype(thresh_throttle),
                    decltype(thresh_watertemp)>
      ogd{thresh_rpm, thresh_throttle, thresh_watertemp};

    n_rpm << 2500;
    alpha_throttle << 0.2;
    T_water << 114.0;

    REQUIRE(!ogt());
    REQUIRE(!ogd());

    n_rpm << 3200;
    T_water << 124.0;

    REQUIRE(ogt());
    REQUIRE(ogd());

    alpha_throttle << 0.9;

    REQUIRE(ogt());
    REQUIRE(ogd());
  }
}
