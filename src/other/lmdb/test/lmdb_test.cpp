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

#include <lmdbxx/lmdb++.h>

#include <catch2/catch.hpp>

#include "temp_lmdb_test.h"

namespace silkworm::db {

TEST_CASE("LMDB") {
  const char* bucket_name{"XXX"};
  Bytes key{from_hex("b1a4F4f387732B107D4F8e8816058bAB6D16397b")};
  Bytes val{from_hex("abba")};

  TemporaryLmdbDatabase db{};
  auto txn{db.begin_ro_transaction()};

  CHECK_THROWS_MATCHES(txn->create_bucket(bucket_name), lmdb::error,
                       Catch::Message("mdb_dbi_open: Permission denied"));

  txn = db.begin_rw_transaction();
  CHECK_THROWS_MATCHES(
      txn->get_bucket(bucket_name), lmdb::error,
      Catch::Message("mdb_dbi_open: MDB_NOTFOUND: No matching key/data pair found"));

  auto bucket{txn->create_bucket(bucket_name)};
  CHECK(!bucket->get(key));

  bucket->put(key, val);
  CHECK(bucket->get(key) == val);

  SECTION("cursor") {
    auto cursor{bucket->cursor()};
    std::optional<Entry> entry{cursor->seek(from_hex("a0"))};
    REQUIRE(entry);
    CHECK(entry->key == key);
    CHECK(entry->value == val);

    entry = cursor->seek(from_hex("c0"));
    CHECK(!entry);
  }
}
}  // namespace silkworm::db
