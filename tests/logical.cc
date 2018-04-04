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
#include "logical.h"
#include "threshold.h"

//
// explicitly instantiate class to make sure compiler generates the class fully
// (enables meaningful test coverage analysis)
//
template class pipebb::logic_gate<std::logical_and<bool>,
                                  pipebb::threshold<pipebb::channel<bool>>,
                                  pipebb::threshold<pipebb::channel<bool>>>;
template class pipebb::and_gate<pipebb::threshold<pipebb::channel<bool>>,
                                pipebb::threshold<pipebb::channel<bool>>>;
template class pipebb::or_gate<pipebb::threshold<pipebb::channel<bool>>,
                               pipebb::threshold<pipebb::channel<bool>>>;
template class pipebb::not_gate<pipebb::threshold<pipebb::channel<bool>>>;
//

TEST_CASE("functionality of the logic gates", "[logic]") {
  pipebb::channel<unsigned>          n_rpm{"n_rpm", "unit", 1, 0};
  pipebb::threshold<decltype(n_rpm)> thresh_rpm{n_rpm, 3000};

  pipebb::channel<double> alpha_throttle{"alpha_trottle", "rad", 1.0, 0.0};
  pipebb::threshold<decltype(alpha_throttle)> thresh_throttle{alpha_throttle,
                                                              0.5};

  pipebb::channel<double>              T_water{"T_water", "deg", 1.0, 0.0};
  pipebb::threshold<decltype(T_water)> thresh_watertemp{T_water, 120.0};

  SECTION("logic_gate") {
    pipebb::logic_gate<std::logical_and<bool>,
                       decltype(thresh_rpm),
                       decltype(thresh_throttle)>
      lgt{thresh_rpm, thresh_throttle};

    pipebb::logic_gate<std::logical_and<bool>,
                       decltype(thresh_rpm),
                       decltype(thresh_throttle),
                       decltype(thresh_watertemp)>
      lgd{thresh_rpm, thresh_throttle, thresh_watertemp};

    n_rpm << 2500;
    alpha_throttle << 0.2;
    T_water << 114.0;

    REQUIRE(!lgt());
    REQUIRE(!lgd());

    n_rpm << 3200;
    T_water << 124.0;

    REQUIRE(!lgt());
    REQUIRE(!lgd());

    alpha_throttle << 0.9;

    REQUIRE(lgt());
    REQUIRE(lgd());
  }

  SECTION("and_gate") {
    pipebb::and_gate<decltype(thresh_rpm), decltype(thresh_throttle)> agt{
      thresh_rpm, thresh_throttle};

    pipebb::and_gate<decltype(thresh_rpm),
                     decltype(thresh_throttle),
                     decltype(thresh_watertemp)>
      agd{thresh_rpm, thresh_throttle, thresh_watertemp};

    n_rpm << 2500;
    alpha_throttle << 0.2;
    T_water << 114.0;

    REQUIRE(!agt());
    REQUIRE(!agd());

    n_rpm << 3200;
    T_water << 124.0;

    REQUIRE(!agt());
    REQUIRE(!agd());

    alpha_throttle << 0.9;

    REQUIRE(agt());
    REQUIRE(agd());
  }

  SECTION("or_gate") {
    pipebb::or_gate<decltype(thresh_rpm), decltype(thresh_throttle)> ogt{
      thresh_rpm, thresh_throttle};

    pipebb::or_gate<decltype(thresh_rpm),
                    decltype(thresh_throttle),
                    decltype(thresh_watertemp)>
      ogd{thresh_rpm, thresh_throttle, thresh_watertemp};

    n_rpm << 2500;
    alpha_throttle << 0.2;
    T_water << 114.0;

    REQUIRE(!ogt());
    REQUIRE(!ogd());

    n_rpm << 3200;
    T_water << 124.0;

    REQUIRE(ogt());
    REQUIRE(ogd());

    alpha_throttle << 0.9;

    REQUIRE(ogt());
    REQUIRE(ogd());
  }
}
