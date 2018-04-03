// Copyright 2017 Ingenieurbüro Krug <info@ingenieurbuero-krug.de>
//
// Maintainer: Florian Eich <florian.eich@ingenieurbuero-krug.de>
//
// All rights reserved.

#include "catch.h"

#include "approximate.h"
#include "channel.h"


//
// explicitly instantiate class to make sure compiler generates the class fully
// (enables meaningful test coverage analysis)
//
template class scrubb::approximate<scrubb::channel<double>,
                                   scrubb::channel<double>>;
//


TEST_CASE("functionality of approximator gates", "[approximator]") {
  SECTION("approximate") {
    scrubb::channel<unsigned> n_rpm_0{"n_rpm", "unit", 1, 0};
    scrubb::channel<double>   n_rpm_1{"n_rpm", "unit", 1.0, 0.0};

    auto approx = scrubb::make_approximate(n_rpm_0, n_rpm_1, 15);

    //
    // Using C++17, the following usage is also possible and (in this case)
    // equally elegant since C++17 supports template class parameter deduction.
    // The make_approximate function is provided for consistency with the other
    // library elements.
    //
    // scrubb::approximate cxx17_ctor_test{n_rpm_0, n_rpm_1, 15};
    //
    //
    // Without a C++17-capable compiler, this becomes:
    //
    scrubb::approximate<decltype(n_rpm_0), decltype(n_rpm_1)> cxx14_ctor_test{
      n_rpm_0, n_rpm_1, 15};
    //

    n_rpm_0 << 3200;
    n_rpm_1 << 3215.0;

    REQUIRE(approx());

    n_rpm_1 << 3216.0;

    REQUIRE(!approx());

    n_rpm_1 << 3185.0;

    REQUIRE(approx());

    n_rpm_1 << 3184.0;

    REQUIRE(!approx());
  }
}
