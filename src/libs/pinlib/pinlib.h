#pragma once
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
#include "acctlib.h"

#include "classes/pinnedchunk.h"
#include "classes/manifest.h"

namespace qblocks {

extern void pinlib_init(QUITHANDLER qh);
extern void pinlib_cleanup(void);

extern bool pinlib_readManifest(CPinnedChunkArray& pList);
extern bool pinlib_updateManifest(CPinnedChunkArray& pList);

extern bool pinlib_pinChunk(CPinnedChunkArray& pList, const string_q& fn, CPinnedChunk& item);
extern bool pinlib_findChunk(CPinnedChunkArray& pList, const string_q& fn, CPinnedChunk& item);

}  // namespace qblocks

using namespace qblocks;  // NOLINT
