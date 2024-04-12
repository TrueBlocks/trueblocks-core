#pragma once
#include "sfstring.h"

namespace qblocks {

extern uint64_t fileSize(const string_q& file);
extern int copyFile(const string_q& from, const string_q& to);
extern int cleanFolder(const string_q& path, bool recurse = false, bool interactive = false);
extern bool folderExists(const string_q& path);
extern bool fileExists(const string_q& file);
extern string_q getCWD(const string_q& filename = "");
extern string_q makeValidName(const string_q& inOut);

}  // namespace qblocks
