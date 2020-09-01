/*
   Copyright 2020 The Silkworm Authors

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "util.h"

#include <catch2/catch.hpp>
#include <silkworm/common/util.hpp>

namespace silkworm::db {
TEST_CASE("DB encode timestamp") {
  CHECK(to_hex(encode_timestamp(1)) == "21");
  CHECK(to_hex(encode_timestamp(8)) == "28");
  CHECK(to_hex(encode_timestamp(0x9f101c)) == "809f101c");
}
}  // namespace silkworm::db
