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

#include "approximate.h"
#include "channel.h"


//
// explicitly instantiate class to make sure compiler generates the class fully
// (enables meaningful test coverage analysis)
//
template class pipebb::approximate<pipebb::channel<double>,
                                   pipebb::channel<double>>;
//


TEST_CASE("functionality of approximator gates", "[approximator]") {
  SECTION("approximate") {
    pipebb::channel<unsigned> n_rpm_0{"n_rpm", "unit", 1, 0};
    pipebb::channel<double>   n_rpm_1{"n_rpm", "unit", 1.0, 0.0};

    auto approx = pipebb::make_approximate(n_rpm_0, n_rpm_1, 15);

    //
    // Using C++17, the following usage is also possible and (in this case)
    // equally elegant since C++17 supports template class parameter deduction.
    // The make_approximate function is provided for consistency with the other
    // library elements.
    //
    // pipebb::approximate cxx17_ctor_test{n_rpm_0, n_rpm_1, 15};
    //
    //
    // Without a C++17-capable compiler, this becomes:
    //
    pipebb::approximate<decltype(n_rpm_0), decltype(n_rpm_1)> cxx14_ctor_test{
      n_rpm_0, n_rpm_1, 15};
    //

    n_rpm_0 << 3200;
    n_rpm_1 << 3215.0;

    REQUIRE(approx());

    n_rpm_1 << 3216.0;

    REQUIRE(!approx());

    n_rpm_1 << 3185.0;

    REQUIRE(approx());

    n_rpm_1 << 3184.0;

    REQUIRE(!approx());
  }
}
