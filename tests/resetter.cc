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

#include "buffer.h"
#include "channel.h"
#include "counter.h"
#include "resetter.h"
#include "threshold.h"


//
// explicitly instantiate class to make sure compiler generates the class fully
// (enables meaningful test coverage analysis)
//
template class pipebb::resetter<pipebb::channel<double>, pipebb::counter>;
//


TEST_CASE("functionality of resetter gates", "[resetter]") {
  pipebb::channel<double> p_manifold{"p_manifold", "mbar", 1.0, 0.0};
  pipebb::threshold<decltype(p_manifold)> thresh{p_manifold, 2570.0};

  p_manifold << 2500.0;

  SECTION("resetter") {
    pipebb::counter counter;
    pipebb::resetter<decltype(thresh), decltype(counter)> resetter{thresh,
                                                                   counter};

    for (unsigned i = 0; i < 5; ++i) {
      counter.step();
      resetter();
    }

    REQUIRE(counter() == 5);

    p_manifold << 2600.0;

    REQUIRE(resetter());

    REQUIRE(counter() == 0);
  }
}
