#pragma once

#include "basetypes.h"
#include "filenames.h"

namespace qblocks {

extern string_q getPathToCommands(const string_q& _part);
extern void loadEnvironmentPaths(void);

}  // namespace qblocks
