#pragma once
#include "sfstring.h"

namespace qblocks {

extern bool fileExists(const string_q& file);
extern string_q getCWD(const string_q& filename = "");
extern uint64_t fileSize(const string_q& file);

}  // namespace qblocks
