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

#include <cmath>

#include "catch.h"

#include "channel.h"
#include "moving_average.h"


//
// explicitly instantiate class to make sure compiler generates the class fully
// (enables meaningful test coverage analysis)
//
template class pipebb::moving_average<pipebb::channel<double>, 50>;
//


TEST_CASE("functionality of the moving_average gates", "[moving_average]") {
  pipebb::channel<double> p_manifold{"p_manifold", "mbar", 1.0, 0.0};

  SECTION("moving_average") {
    const unsigned S = 5;
    pipebb::moving_average<decltype(p_manifold), S> mavg{p_manifold};

    p_manifold << 2500.0;

    REQUIRE(mavg() == p_manifold() / S);
    REQUIRE(mavg() == 2 * p_manifold() / S);
    REQUIRE(mavg() == 3 * p_manifold() / S);
    REQUIRE(mavg() == 4 * p_manifold() / S);
    REQUIRE(mavg() == 5 * p_manifold() / S);

    REQUIRE(mavg() == 5 * p_manifold() / S);
    REQUIRE(mavg() == 5 * p_manifold() / S);

    mavg.reset();
    REQUIRE(mavg() == 500.0);
  }
}
