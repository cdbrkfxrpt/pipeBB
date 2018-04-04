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

#include "accumulator.h"
#include "channel.h"


//
// explicitly instantiate class to make sure compiler generates the class fully
// (enables meaningful test coverage analysis)
//
template class pipebb::accumulator<pipebb::channel<double>, 5>;
//


TEST_CASE("functionality of the accumulator gates", "[accumulator]") {
  pipebb::channel<double> overboost{"overboost", "mbar", 1.0, 0.0};

  SECTION("accumulator") {
    auto acc = pipebb::make_accumulator<5>(overboost);

    //
    // The following usage is possible, but riddles the code with "decltype":
    //
    pipebb::accumulator<decltype(overboost), 5> ctot_test{overboost};
    //

    overboost << 12.5;

    REQUIRE(acc() == 12.5);
    REQUIRE(acc() == 25.0);

    overboost << 5.0;

    acc();
    acc();

    REQUIRE(acc() == 40.0);
    REQUIRE(acc() == 32.5);
    REQUIRE(acc() == 25.0);
    REQUIRE(acc() == 25.0);
  }
}
