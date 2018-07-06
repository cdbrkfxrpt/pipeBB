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
#include "gradient.h"
#include "threshold.h"


TEST_CASE("combined gates functionality", "[combined]") {
  SECTION("comparative") {
    pipebb::channel<unsigned>          n_rpm{"n_rpm", "unit", 1, 0};
    pipebb::threshold<decltype(n_rpm)> thresh{n_rpm, 3000};

    n_rpm << 2500;
    REQUIRE(!thresh());

    n_rpm << 3200;
    REQUIRE(thresh());
  }

  SECTION("flow control") {}

  SECTION("block combinations") {
    pipebb::channel<double> fun{"fun", "rad", 1.0, 0.0};

    pipebb::gradient<decltype(fun)>   grad{fun};
    pipebb::threshold<decltype(grad)> thresh{grad, 50};

    fun << 200.0;
    REQUIRE(thresh());
    REQUIRE(!thresh());
  }

  SECTION("varstep_gradient") {
    pipebb::channel<double> fun{"fun", "rad", 1.0, 0.0};

    pipebb::varstep_gradient<decltype(fun), 3> grad{fun};

    fun << 200.0;

    REQUIRE(grad() == 200.0);
    REQUIRE(grad() == 200.0);
    REQUIRE(grad() == 0);
  }
}
