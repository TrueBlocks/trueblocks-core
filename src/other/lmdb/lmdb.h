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

// See http://www.lmdb.tech/doc/index.html

#ifndef SILKWORM_DB_LMDB_H_
#define SILKWORM_DB_LMDB_H_

#include <lmdb/lmdb.h>
#include <stdint.h>

#include <silkworm/db/database.hpp>

namespace silkworm::db {

struct LmdbOptions {
  uint64_t map_size = 2ull << 40;  // 2TiB by default
  bool no_sync = true;             // MDB_NOSYNC
  bool no_meta_sync = false;       // MDB_NOMETASYNC
  bool write_map = false;          // MDB_WRITEMAP
  bool no_sub_dir = false;         // MDB_NOSUBDIR
  unsigned max_buckets = 100;
};

class LmdbCursor : public Cursor {
 public:
  explicit LmdbCursor(MDB_cursor* cursor) : cursor_{cursor} {}

  ~LmdbCursor() override;

  std::optional<Entry> seek(ByteView prefix) override;

 private:
  MDB_cursor* cursor_{nullptr};
};

class LmdbBucket : public Bucket {
 public:
  LmdbBucket(MDB_dbi dbi, MDB_txn* txn);

  void put(ByteView key, ByteView value) override;

  std::optional<ByteView> get(ByteView key) const override;

  std::unique_ptr<Cursor> cursor() override;

 private:
  MDB_dbi dbi_{0};
  MDB_txn* txn_{nullptr};
};

class LmdbTransaction : public Transaction {
 public:
  explicit LmdbTransaction(MDB_txn* txn);

  ~LmdbTransaction() override;

  std::unique_ptr<Bucket> create_bucket(const char* name) override;
  std::unique_ptr<Bucket> get_bucket(const char* name) override;

  void commit() override;
  void rollback() override;

 private:
  MDB_txn* txn_{nullptr};
};

// Must not create several instances of the same database.
class LmdbDatabase : public Database {
 public:
  explicit LmdbDatabase(const char* path, const LmdbOptions& options = {});
  ~LmdbDatabase() override;

  std::unique_ptr<Transaction> begin_transaction(bool read_only) override;

 protected:
  MDB_env* env_{nullptr};
};
}  // namespace silkworm::db

#endif  // SILKWORM_DB_LMDB_H_
