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

#include <algorithm>
#include <cmath>
#include <iostream>

#include "catch.h"

#include "channel.h"


//
// explicitly instantiate class to make sure compiler generates the class fully
// (enables meaningful test coverage analysis)
//
template class pipebb::channel<double>;
//


TEST_CASE("basic functionality of channel container", "[channel_basics]") {
  pipebb::channel<double> chan{"test", "m", 1.0, 0.0};
  pipebb::channel<double> sec{"sec", "m", 1.0, 0.0};

  SECTION("constructors") {
    //
    // the following objects are only initialized but not ever used.
    pipebb::channel<uint32_t> default_ctor_1;
    pipebb::channel<double>   default_ctor_2;
    pipebb::channel<uint32_t> name_only_ctor_1{"name only"};
    pipebb::channel<double>   name_only_ctor_2{"name only"};
    //

    REQUIRE(chan.factor() == 1.0);
    REQUIRE(chan.offset() == 0.0);


    chan.factor(1.5);
    chan.offset(0.7);

    REQUIRE(chan.factor() == 1.5);
    REQUIRE(chan.offset() == 0.7);


    chan.factor(&sec);
    chan.offset(&sec);

    REQUIRE(chan.dynamic_factor() == &sec);
    REQUIRE(chan.dynamic_offset() == &sec);
  }

  SECTION("simulated operation") {
    for (unsigned i = 0; i < 5; ++i) { chan << static_cast<double>(i); }

    REQUIRE(chan() == static_cast<double>(4));

    chan.factor(&sec);
    chan.offset(&sec);

    for (unsigned i = 0; i < 5; ++i) {
      sec << static_cast<double>(i);
      chan << static_cast<double>(i);
      REQUIRE(chan() == static_cast<double>((i + i) * i));
    }
  }
}
