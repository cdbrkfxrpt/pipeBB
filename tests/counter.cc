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

#include "channel.h"
#include "counter.h"
#include "threshold.h"


//
// explicitly instantiate class to make sure compiler generates the class fully
// (enables meaningful test coverage analysis)
//
template class pipebb::reset_counter<pipebb::channel<double>>;
template class pipebb::boolean_counter<pipebb::channel<double>>;
//


TEST_CASE("functionality of counters", "[counters]") {
  SECTION("counter") {
    pipebb::counter counter;
    REQUIRE(counter() == 0);
    REQUIRE(counter.step() == 1);
    REQUIRE(counter() == 1);
    REQUIRE(counter.step() == 2);

    REQUIRE(++counter == 3);
    REQUIRE(counter++ == 4);
    REQUIRE(counter() == 4);
  }

  pipebb::channel<double> p_manifold{"p_manifold", "mbar", 1.0, 0.0};
  pipebb::threshold<decltype(p_manifold)> thresh{p_manifold, 2570.0};

  SECTION("resettable_counter") {
    pipebb::reset_counter<decltype(thresh)> rcounter{thresh};

    p_manifold << 2500.0;

    for (unsigned i = 1; i < 6; ++i) { REQUIRE(rcounter() == i); }

    p_manifold << 2600.0;

    for (unsigned i = 0; i < 5; ++i) { REQUIRE(rcounter() == 0); }
  }

  SECTION("boolean_counter") {
    pipebb::boolean_counter<decltype(thresh)> count{thresh};

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
    pipebb::counter          counter;
    pipebb::counter_watchdog twd{counter};

    REQUIRE(!twd());

    counter.step();

    REQUIRE(twd());
    REQUIRE(!twd());
  }
}
