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

//#include <boost/endian/conversion.hpp>
//#include <boost/iterator/counting_iterator.hpp>
#include "./common/util.hpp"

namespace silkworm::db::history_index {

constexpr size_t item_len{3};

uint64_t elem(ByteView elements, uint64_t min_element, uint32_t i) {
  uint64_t x{min_element};
  x += (elements[i * item_len] & 0x7f) << 16;
  x += elements[i * item_len + 1] << 8;
  x += elements[i * item_len + 2];
  return x;
};

std::optional<SearchResult> find(ByteView hi, uint64_t v) {
  if (hi.length() < 8) {
    throw DecodingError("minimal length of index chunk is 8");
  }
  if ((hi.length() - 8) % item_len) {
    throw DecodingError("length of index chunk should be 8 (mod 3)");
  }

  using boost::endian::load_big_u64;

  size_t n{(hi.length() - 8) / item_len};
  uint64_t min_element{load_big_u64(hi.data())};
  ByteView elements{hi.substr(8)};

  uint32_t i{*std::lower_bound(boost::counting_iterator<uint32_t>(0),
                               boost::counting_iterator<uint32_t>(n), v,
                               [elements, min_element](uint32_t i, uint64_t v) {
                                 return elem(elements, min_element, i) < v;
                               })};

  if (i == n) {
    return {};
  } else {
    return SearchResult{
        elem(elements, min_element, i),       // change_block
        (elements[i * item_len] & 0x80) != 0  // new_record
    };
  }
}
}  // namespace silkworm::db::history_index
