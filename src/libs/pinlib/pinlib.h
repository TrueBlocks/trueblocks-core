#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
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

#include "pinatalicense.h"
#include "pinneditem.h"
#include "pinatalist.h"
#include "pinatapin.h"
#include "manifest.h"

namespace qblocks {

extern void pinlib_init(QUITHANDLER qh);
extern void pinlib_cleanup(void);

extern bool pinlib_pinChunk(CPinnedItemArray& pList, const string_q& fileName, CPinnedItem& item);
extern bool pinlib_unpinChunk(CPinnedItemArray& pList, const string_q& fileName, CPinnedItem& item);
extern bool pinlib_getChunkByHash(CPinnedItemArray& pList, const string_q& fileName, CPinnedItem& item);
extern bool pinlib_findChunk(CPinnedItemArray& pList, const string_q& fileName, CPinnedItem& item);
extern bool pinlib_readPinList(CPinnedItemArray& pList, bool required);
extern bool pinlib_writePinList(CPinnedItemArray& pList);

typedef bool (*PINFUNC)(CPinnedItem& pin, void* data);
bool pinlib_forEveryPin(CPinnedItemArray& pList, PINFUNC func, void* data);

extern string_q pinlib_pinOneFile(const string_q& fileName, const string_q& type, bool compress);
extern string_q pinlib_unpinOneFile(const string_q& hash);

}  // namespace qblocks

using namespace qblocks;  // NOLINT
