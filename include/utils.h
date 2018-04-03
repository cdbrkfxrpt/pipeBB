/* ------------------------------------------------------------------------- *
 * Copyright 2017 Ingenieurbüro Krug. All rights reserved.
 *
 * Author: Florian Eich <florian.eich@ingenieurbuero-krug.de>
 * ------------------------------------------------------------------------- */

#ifndef SCRUBB_CORE_UTILS_H_
#define SCRUBB_CORE_UTILS_H_

#define REQUIRES(...) std::enable_if_t<(__VA_ARGS__)>...


/*!
 * \namespace scrubb
 * \brief The namespace of the scruBB library.
 *
 * scruBB uses only one namespace, mainly for brevity and simplicity.
 * Functional grouping is achieved by combining compilation units; however, it
 * is generally not possible to use them separately as they rely on each other.
 * On the other hand, scruBB has no dependencies other than the C++ standard
 * library.
 */
namespace scrubb {}  // namespace scrubb

#endif  // SCRUBB_CORE_UTILS_H_
