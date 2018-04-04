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

#include <iostream>

#include "catch.h"

#include "channel.h"
#include "pass_through.h"
#include "threshold.h"


//
// explicitly instantiate class to make sure compiler generates the class fully
// (enables meaningful test coverage analysis)
//
template class pipebb::pass_through<
  pipebb::channel<double>,
  pipebb::threshold<pipebb::channel<double>>>;
template class pipebb::threshold_pass_through<pipebb::channel<double>>;
template class pipebb::buffered_pass_through<
  pipebb::channel<double>,
  pipebb::threshold<pipebb::channel<double>>>;
//


TEST_CASE("functionality of pass through gates", "[pass_through]") {
  pipebb::channel<double> p_manifold{"p_manifold", "mbar", 1.0, 0.0};
  pipebb::threshold<decltype(p_manifold)> thresh{p_manifold, 2570.0};
  p_manifold << 2500.0;

  SECTION("pass_through") {
    pipebb::pass_through<decltype(p_manifold), decltype(thresh)> pass_through{
      p_manifold, thresh};

    REQUIRE(!pass_through());

    p_manifold << 2600.0;
    REQUIRE(pass_through() == p_manifold());
  }

  SECTION("threshold_pass_through") {
    pipebb::threshold_pass_through<decltype(p_manifold)> pass_through{
      p_manifold, 2550.0};

    REQUIRE(!pass_through());

    p_manifold << 2600.0;
    REQUIRE(pass_through() == p_manifold());
  }

  SECTION("buffered_pass_through") {
    pipebb::buffered_pass_through<decltype(p_manifold), decltype(thresh)>
      buffered_pass_through{p_manifold, thresh};

    REQUIRE(!buffered_pass_through());

    p_manifold << 2600.0;
    REQUIRE(buffered_pass_through() == p_manifold());
  }
}
