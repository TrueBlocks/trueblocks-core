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

#include "temp_lmdb_test.h"

namespace silkworm::db {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
static char* temporary_file_name() { return std::tmpnam(nullptr); }
#pragma GCC diagnostic pop

TemporaryLmdbDatabase::TemporaryLmdbDatabase()
    : LmdbDatabase{temporary_file_name(),
                   LmdbOptions{
                       32 << 20,  // map_size = 32MiB
                       true,      // no_sync
                       true,      // no_meta_sync
                       true,      // .write_map
                       true,      // no_sub_dir
                   }} {}

TemporaryLmdbDatabase::~TemporaryLmdbDatabase() {
  if (!env_) {
    return;
  }

  const char* tmp_file{nullptr};
  mdb_env_get_path(env_, &tmp_file);

  mdb_env_close(env_);
  env_ = nullptr;

  if (tmp_file) {
    std::remove(tmp_file);
  }
}
}  // namespace silkworm::db
