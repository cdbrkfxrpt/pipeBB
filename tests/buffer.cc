// Copyright 2017 Ingenieurbüro Krug <info@ingenieurbuero-krug.de>
//
// Maintainer: Florian Eich <florian.eich@ingenieurbuero-krug.de>
//
// All rights reserved.

#include "catch.h"

#include "buffer.h"
#include "channel.h"
#include "counter.h"


//
// explicitly instantiate class to make sure compiler generates the class fully
// (enables meaningful test coverage analysis)
//
template class scrubb::buffer<scrubb::channel<double>>;
//


TEST_CASE("functionality of buffer gates", "[buffer_gates]") {
  scrubb::channel<double> p_manifold{"p_manifold", "mbar", 1.0, 0.0};

  SECTION("buffer") {
    // Instantiate buffer object.
    auto buf = scrubb::make_buffer(p_manifold);

    //
    // The following usage is also possible and (in this case) equally elegant
    // since C++17 supports template class parameter deduction. The
    // make_approximate function is provided for consistency with the other
    // library elements.
    //
    // scrubb::buffer cxx_17_ctor_test{p_manifold};
    //
    //
    // Without a C++17-capable compiler (g++ 7 or later), this becomes:
    //
    scrubb::buffer<decltype(p_manifold)> cxx_14_ctor_test{p_manifold};
    //

    p_manifold << 2500.0;
    REQUIRE(buf() == 2500.0);

    p_manifold << 2600.0;
    REQUIRE(buf() == 2500.0);

    buf.reset();
    REQUIRE(buf() == 2600.0);

    buf.reset();
    p_manifold << 0.0;
    REQUIRE(buf() == 0.0);

    p_manifold << 2600.0;
    REQUIRE(buf() == 2600.0);
  }

  SECTION("switched_buffer") {
    // Instantiate switch_buffer object.
    auto sbuf = scrubb::make_switched_buffer(p_manifold);

    //
    // The following usage is also possible and (in this case) equally elegant
    // since C++17 supports template class parameter deduction. The
    // make_approximate function is provided for consistency with the other
    // library elements.
    //
    // scrubb::switched_buffer cxx17_ctor_test{p_manifold};
    //
    //
    // Without a C++17-capable compiler (g++ 7 or later), this becomes:
    //
    scrubb::switched_buffer<decltype(p_manifold)> cxx14_ctor_test{p_manifold};
    //

    REQUIRE(!sbuf.state());

    p_manifold << 2500.0;
    REQUIRE(sbuf() == 2500.0);

    p_manifold << 2600.0;
    REQUIRE(sbuf() == 2500.0);

    sbuf.toggle();
    REQUIRE(sbuf() == 2600.0);

    p_manifold << 2700.0;
    REQUIRE(sbuf() == 2600.0);

    sbuf.reset();
    REQUIRE(sbuf() == 2700.0);
  }
}
