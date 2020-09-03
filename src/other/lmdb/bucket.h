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

#ifndef SILKWORM_DB_BUCKET_H_
#define SILKWORM_DB_BUCKET_H_

/*
Part of the compatibility layer with the Turbo-Geth DB format;
see its common/dbutils/bucket.go.
*/
namespace silkworm::db::bucket {

constexpr const char* kPlainState{"PLAIN-CST"};
constexpr const char* kAccountChanges{"PLAIN-ACS"};
constexpr const char* kStorageChanges{"PLAIN-SCS"};
constexpr const char* kAccountHistory{"hAT"};
constexpr const char* kStorageHistory{"hST"};
constexpr const char* kCode{"CODE"};
constexpr const char* kCodeHash{"PLAIN-contractCode"};
constexpr const char* kBlockHeaders{"h"};
constexpr const char* kBlockBodies{"b"};
constexpr const char* kSenders{"txSenders"};

constexpr const char* kBuckets[]{
    kPlainState, kAccountChanges, kStorageChanges, kAccountHistory, kStorageHistory,
    kCode,       kCodeHash,       kBlockHeaders,   kBlockBodies,    kSenders,
};
}  // namespace silkworm::db::bucket

#endif  // SILKWORM_DB_BUCKET_H_
