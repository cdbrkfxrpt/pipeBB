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
#include "gradient.h"


//
// explicitly instantiate class to make sure compiler generates the class fully
// (enables meaningful test coverage analysis)
//
template class pipebb::gradient<pipebb::channel<double>>;
//


TEST_CASE("functionality of the gradient gates", "[gradient]") {
  pipebb::channel<double> p_manifold{"p_manifold", "mbar", 1.0, 0.0};

  SECTION("gradient") {
    pipebb::gradient<decltype(p_manifold)> grad{p_manifold};

    p_manifold << 2500.0;
    REQUIRE(grad() == 2500.0);

    p_manifold << 2570.0;
    REQUIRE(grad() == 70.0);

    p_manifold << 2370.0;
    REQUIRE(grad() == -200.0);
  }
}
