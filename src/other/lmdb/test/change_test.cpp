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

#include "change.h"

#include <catch2/catch.hpp>
#include <evmc/evmc.hpp>
#include <silkworm/common/util.hpp>
#include <silkworm/db/util.hpp>

namespace silkworm::db {
TEST_CASE("Storage change") {
  auto contract_a{0x6f0e0cdac6c716a00bd8db4d0eee4f2bfccf8e6a_address};
  auto contract_b{0xc5acb79c258108f288288bc26f7820d06f45f08c_address};
  auto contract_c{0x1cbdd8336800dc3fe27daf5fb5188f0502ac1fc7_address};
  auto contract_d{0xd88eba4c93123372a9f67215f80477bc3644e6ab_address};

  auto key1{0xa4e69cebbf4f8f3a1c6e493a6983d8a5879d22057a7c73b00e105d7c7e21efbc_bytes32};
  auto key2{0x0bece5a88f7b038f806dbef77c0b462506e4b566c5be7dd44e8e2fc7b1f6a99c_bytes32};
  auto key3{0x0000000000000000000000000000000000000000000000000000000000000001_bytes32};
  auto key4{0x4fdf6c1878d2469b49684effe69db8689d88a4f1695055538501ff197bc9e30e_bytes32};
  auto key5{0xaa2703c3ae5d0024b2c3ab77e5200bb2a8eb39a140fad01e89a495d73760297c_bytes32};
  auto key6{0x000000000000000000000000000000000000000000000000000000000000df77_bytes32};

  auto val1{0x33bf0d0c348a2ef1b3a12b6a535e1e25a56d3624e45603e469626d80fd78c762_bytes32};
  auto val2{0x0000000000000000000000000000000000000000000000000000000000000459_bytes32};
  auto val3{0x0000000000000000000000000000002506e4b566c5be7dd44e8e2fc7b1f6a99c_bytes32};
  auto val4{0x207a386cdf40716455365db189633e822d3a7598558901f2255e64cb5e424714_bytes32};
  auto val5{0x0000000000000000000000000000000000000000000000000000000000000000_bytes32};
  auto val6{0xec89478783348038046b42cc126a3c4e351977b5f4cf5e3c4f4d8385adbf8046_bytes32};

  Bytes encoded{from_hex(
      "000000041cbdd8336800dc3fe27daf5fb5188f0502ac1fc7000000016f0e0cdac6c716a00bd8db4d0eee4f2bfccf"
      "8e6a000000026f0e0cdac6c716a00bd8db4d0eee4f2bfccf8e6a00000004c5acb79c258108f288288bc26f7820d0"
      "6f45f08c00000006000000020000000000000000000000050000000200000000000000024fdf6c1878d2469b4968"
      "4effe69db8689d88a4f1695055538501ff197bc9e30eaa2703c3ae5d0024b2c3ab77e5200bb2a8eb39a140fad01e"
      "89a495d73760297c000000000000000000000000000000000000000000000000000000000000df77a4e69cebbf4f"
      "8f3a1c6e493a6983d8a5879d22057a7c73b00e105d7c7e21efbc0000000000000000000000000000000000000000"
      "0000000000000000000000010bece5a88f7b038f806dbef77c0b462506e4b566c5be7dd44e8e2fc7b1f6a99c0000"
      "00060000000000000000202040607173207a386cdf40716455365db189633e822d3a7598558901f2255e64cb5e42"
      "4714ec89478783348038046b42cc126a3c4e351977b5f4cf5e3c4f4d8385adbf804633bf0d0c348a2ef1b3a12b6a"
      "535e1e25a56d3624e45603e469626d80fd78c7622506e4b566c5be7dd44e8e2fc7b1f6a99c0459")};

  StorageChanges sc{};
  sc[storage_key(contract_a, 2, key1)] = zeroless_view(val1);
  sc[storage_key(contract_b, 1, key2)] = zeroless_view(val2);
  sc[storage_key(contract_b, 1, key3)] = zeroless_view(val3);
  sc[storage_key(contract_c, 5, key4)] = zeroless_view(val4);
  sc[storage_key(contract_a, 1, key5)] = zeroless_view(val5);
  sc[storage_key(contract_a, 2, key6)] = zeroless_view(val6);

  CHECK(to_hex(sc.encode()) == to_hex(encoded));

  CHECK(StorageChanges::find(encoded, storage_key(contract_a, 2, key1)) == zeroless_view(val1));
  CHECK(StorageChanges::find(encoded, storage_key(contract_b, 1, key2)) == zeroless_view(val2));
  CHECK(StorageChanges::find(encoded, storage_key(contract_b, 1, key3)) == zeroless_view(val3));
  CHECK(StorageChanges::find(encoded, storage_key(contract_c, 5, key4)) == zeroless_view(val4));
  CHECK(StorageChanges::find(encoded, storage_key(contract_a, 1, key5)) == zeroless_view(val5));
  CHECK(StorageChanges::find(encoded, storage_key(contract_a, 2, key6)) == zeroless_view(val6));

  CHECK(!StorageChanges::find(encoded, storage_key(contract_a, 1, key1)));
  CHECK(!StorageChanges::find(encoded, storage_key(contract_d, 2, key1)));
  CHECK(!StorageChanges::find(encoded, storage_key(contract_b, 1, {})));
}
}  // namespace silkworm::db
