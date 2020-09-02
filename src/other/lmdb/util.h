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

#ifndef SILKWORM_DB_UTIL_H_
#define SILKWORM_DB_UTIL_H_

/*
Part of the compatibility layer with the Turbo-Geth DB format;
see its package dbutils.
*/

#include "common/base.hpp"
#include <string>

namespace silkworm::db {
// Turbo-Geth PlainGenerateStoragePrefix
Bytes storage_prefix(const evmc::address& address, uint64_t incarnation);

// Turbo-Geth PlainGenerateCompositeStorageKey
Bytes storage_key(const evmc::address& address, uint64_t incarnation, const evmc::bytes32& key);

// Turbo-Geth HeaderHashKey
Bytes header_hash_key(uint64_t block_number);

// Turbo-Geth HeaderKey & BlockBodyKey
Bytes block_key(uint64_t block_number, const evmc::bytes32& hash);

// Turbo-Geth IndexChunkKey
Bytes history_index_key(ByteView key, uint64_t block_number);

// Turbo-Geth EncodeTimestamp
// If a < b, then Encoding(a) < Encoding(b) lexicographically
Bytes encode_timestamp(uint64_t block_number);

// Default database path
std::string default_path();
}  // namespace silkworm::db

#endif  // SILKWORM_DB_UTIL_H_
