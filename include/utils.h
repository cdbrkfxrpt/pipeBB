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

#ifndef PIPEBB_UTILS_H_
#define PIPEBB_UTILS_H_

#define REQUIRES(...) std::enable_if_t<(__VA_ARGS__)>...


/*!
 * \namespace pipebb
 * \brief The namespace of the pipeBB library.
 *
 * pipeBB uses only one namespace, mainly for brevity and simplicity.
 * Functional grouping is achieved by combining compilation units; however, it
 * is generally not possible to use them separately as they rely on each other.
 * On the other hand, pipeBB has no dependencies other than the C++ standard
 * library.
 */
namespace pipebb {}  // namespace pipebb

#endif  // PIPEBB_UTILS_H_
