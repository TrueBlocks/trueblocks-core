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

#ifndef SILKWORM_DB_HISTORY_INDEX_H_
#define SILKWORM_DB_HISTORY_INDEX_H_

/*
Part of the compatibility layer with the Turbo-Geth DB format;
see its common/dbutils/history_index.go.
*/

#include <optional>
#include "./common/base.hpp"

namespace silkworm::db::history_index {

struct SearchResult {
  uint64_t change_block{0};
  bool new_record{false};
};

// Turbo-Geth (HistoryIndexBytes)Search
std::optional<SearchResult> find(ByteView index, uint64_t v);

}  // namespace silkworm::db::history_index

#endif  // SILKWORM_DB_HISTORY_INDEX_H_
