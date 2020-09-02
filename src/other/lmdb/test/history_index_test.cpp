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

#include "history_index.h"

#include <catch2/catch.hpp>
#include <silkworm/common/util.hpp>

namespace silkworm::db::history_index {

TEST_CASE("History index search") {
  Bytes index{from_hex("0000000000000003000000000002000005")};

  CHECK(find(index, 1)->change_block == 3);
  CHECK(find(index, 3)->change_block == 3);
  CHECK(find(index, 4)->change_block == 5);
  CHECK(find(index, 5)->change_block == 5);
  CHECK(find(index, 7)->change_block == 8);
  CHECK(find(index, 8)->change_block == 8);
  CHECK(!find(index, 9));
}
}  // namespace silkworm::db::history_index
