/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "basetypes.h"
#include "version.h"

namespace qblocks {

string_q GIT_COMMIT_BRANCH = D_COMMIT_BRANCH;
string_q GIT_COMMIT_HASH = D_COMMIT_HASH;
timestamp_t GIT_COMMIT_TS = D_COMMIT_TS;
string_q PIN_HASH = "";
string_q versionHash = "0x" + padLeft(PIN_HASH, 64, '0');

}  // namespace qblocks
