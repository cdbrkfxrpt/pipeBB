#
# Copyright 2018- Florian Eich <florian.eich@gmail.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

#
# set test target list
set (TEST_TARGET_LIST
  accumulator
  approximate
  buffer
  channel
  combined
  counter
  factor
  gradient
  inverter
  logical
  moving_average
  offset
  pass_through
  resetter
  ringbuffer
  threshold
)
#

#
# set target include directories
set (INCLUDE_DIRECTORIES
  ${pipeBB_SOURCE_DIR}/include
)
#

#
# add and configure executables and add them as tests
foreach (TARGET ${TEST_TARGET_LIST})
  #
  # create executable for target
  add_executable (${TARGET} main.cc ${TARGET}.cc)
  #

  #
  # specify target include directories
  target_include_directories (${TARGET} PUBLIC ${INCLUDE_DIRECTORIES})
  #

  #
  # add test to CTest
  add_test (NAME ${TARGET} COMMAND ${TARGET})
  #
endforeach (TARGET)
#

#
# set up and build target for Gitlab Runner / CI and test coverage analysis
set (CI_TARGET ci_runner)
set (CI_SOURCES main.cc)
#
foreach (TARGET ${TEST_TARGET_LIST})
  list (APPEND CI_SOURCES ${TARGET}.cc)
endforeach ()
#
add_executable (${CI_TARGET} ${CI_SOURCES})
#
target_compile_options (${CI_TARGET} PRIVATE
  -fprofile-arcs
  -ftest-coverage
  -fno-inline
  -fno-inline-small-functions
  -fno-default-inline
  -fPIC
  -O0
  --coverage
)
#
target_include_directories (${CI_TARGET} PRIVATE ${INCLUDE_DIRECTORIES})
#
target_link_libraries (${CI_TARGET} gcov)
#

