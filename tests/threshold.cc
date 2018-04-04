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
#include "threshold.h"


//
// explicitly instantiate class to make sure compiler generates the class fully
// (enables meaningful test coverage analysis)
//
template class pipebb::threshold<pipebb::channel<double>>;
//


TEST_CASE("functionality of threshold gates", "[threshold]") {
  SECTION("threshold") {
    pipebb::channel<unsigned>          n_rpm{"n_rpm", "unit", 1, 0};
    pipebb::threshold<decltype(n_rpm)> thresh{n_rpm, 3000};

    n_rpm << 2500;
    REQUIRE(!thresh());

    n_rpm << 3200;
    REQUIRE(thresh());
  }
}
