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

#ifndef SILKWORM_DB_CHANGE_H_
#define SILKWORM_DB_CHANGE_H_

/*
Part of the compatibility layer with the Turbo-Geth DB format;
see its package changeset.
*/

#include <evmc/evmc.hpp>
#include <map>
#include <optional>
#include <silkworm/common/base.hpp>

namespace silkworm::db {

class AccountChanges : public std::map<evmc::address, Bytes> {
 public:
  // Turbo-Geth decodeAccountsWithKeyLen
  static AccountChanges decode(ByteView encoded);

  // Turbo-Geth (AccountChangeSetPlainBytes)Find
  static std::optional<ByteView> find(ByteView encoded, ByteView key);
};

class StorageChanges : public std::map<Bytes, Bytes> {
 public:
  // Turbo-Geth EncodeStoragePlain
  Bytes encode() const;

  // Turbo-Geth (StorageChangeSetPlainBytes)FindWithIncarnation
  static std::optional<ByteView> find(ByteView encoded, ByteView key);
};
}  // namespace silkworm::db

#endif  // SILKWORM_DB_CHANGE_H_
