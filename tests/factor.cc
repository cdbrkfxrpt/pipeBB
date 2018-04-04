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
#include "factor.h"


//
// explicitly instantiate class to make sure compiler generates the class fully
// (enables meaningful test coverage analysis)
//
template class pipebb::factor<pipebb::channel<double>>;
//


TEST_CASE("functionality of the grading gates", "[grading]") {
  pipebb::channel<double> p_manifold{"p_manifold", "mbar", 1.0, 0.0};

  SECTION("factor") {
    pipebb::factor<decltype(p_manifold)> mul_fac{p_manifold, 2.0};
    pipebb::factor<decltype(p_manifold)> div_fac{p_manifold, 0.5};

    p_manifold << 2200.0;

    REQUIRE(mul_fac() == 4400.0);
    REQUIRE(div_fac() == 1100.0);

    mul_fac.set_constant(3.0);
    div_fac.set_constant(0.25);

    REQUIRE(mul_fac() == 6600.0);
    REQUIRE(div_fac() == 550.0);
  }
}
