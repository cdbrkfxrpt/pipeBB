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
#include "offset.h"


//
// explicitly instantiate class to make sure compiler generates the class fully
// (enables meaningful test coverage analysis)
//
template class pipebb::offset<pipebb::channel<double>>;
//


TEST_CASE("functionality of the offset gates", "[offset]") {
  pipebb::channel<double> p_manifold{"p_manifold", "mbar", 1.0, 0.0};

  SECTION("offset") {
    pipebb::offset<decltype(p_manifold)> add_off{p_manifold, 200.0};
    pipebb::offset<decltype(p_manifold)> sub_off{p_manifold, -200.0};

    p_manifold << 2500.0;

    REQUIRE(add_off() == 2700.0);
    REQUIRE(sub_off() == 2300.0);

    add_off.set_offset(150.0);
    sub_off.set_offset(-150.0);

    REQUIRE(add_off() == 2650.0);
    REQUIRE(sub_off() == 2350.0);
  }
}
