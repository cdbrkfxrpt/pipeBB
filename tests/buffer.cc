//
// Copyright 2018- Florian Eich <florian.eich@gmail.com>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "catch.h"

#include "buffer.h"
#include "channel.h"
#include "counter.h"


//
// explicitly instantiate class to make sure compiler generates the class fully
// (enables meaningful test coverage analysis)
//
template class pipebb::buffer<pipebb::channel<double>>;
//


TEST_CASE("functionality of buffer gates", "[buffer_gates]") {
  pipebb::channel<double> p_manifold{"p_manifold", "mbar", 1.0, 0.0};

  SECTION("buffer") {
    // Instantiate buffer object.
    auto buf = pipebb::make_buffer(p_manifold);

    //
    // The following usage is also possible and (in this case) equally elegant
    // since C++17 supports template class parameter deduction. The
    // make_approximate function is provided for consistency with the other
    // library elements.
    //
    // pipebb::buffer cxx_17_ctor_test{p_manifold};
    //
    //
    // Without a C++17-capable compiler (g++ 7 or later), this becomes:
    //
    pipebb::buffer<decltype(p_manifold)> cxx_14_ctor_test{p_manifold};
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
    auto sbuf = pipebb::make_switched_buffer(p_manifold);

    //
    // The following usage is also possible and (in this case) equally elegant
    // since C++17 supports template class parameter deduction. The
    // make_approximate function is provided for consistency with the other
    // library elements.
    //
    // pipebb::switched_buffer cxx17_ctor_test{p_manifold};
    //
    //
    // Without a C++17-capable compiler (g++ 7 or later), this becomes:
    //
    pipebb::switched_buffer<decltype(p_manifold)> cxx14_ctor_test{p_manifold};
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
