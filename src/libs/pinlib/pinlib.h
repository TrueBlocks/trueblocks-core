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

#include "pinapilicense.h"
#include "pinnedchunk.h"
#include "pinatalist.h"
#include "pinatapin.h"
#include "manifest.h"

namespace qblocks {

extern void pinlib_init(QUITHANDLER qh);
extern void pinlib_cleanup(void);

extern bool pinlib_getApiKeys(CPinApiLicense& lic);
extern bool pinlib_getPinList(const CPinApiLicense& lic, string_q& result);
extern bool pinlib_readPinList(CPinnedChunkArray& pList, bool required, bool local);
extern void pinlib_loadPinMaps(CIndexStringMap& fnMap, CIndexHashMap& bloomMap, CIndexHashMap& indexMap);
extern bool pinlib_writePinList(CPinnedChunkArray& pList, bool local);

typedef bool (*PINFUNC)(CPinnedChunk& pin, void* data);
extern bool pinlib_forEveryPin(CPinnedChunkArray& pList, PINFUNC func, void* data);

extern bool pinlib_pinChunk(CPinnedChunkArray& pList, const string_q& fileName, CPinnedChunk& item);
extern bool pinlib_unpinChunk(CPinnedChunkArray& pList, const string_q& fileName, CPinnedChunk& item);
extern bool pinlib_getChunkByHash(CPinnedChunkArray& pList, const string_q& fileName, CPinnedChunk& item);
extern bool pinlib_findChunk(CPinnedChunkArray& pList, const string_q& fileName, CPinnedChunk& item);

extern string_q pinlib_pinOneFile(const string_q& fileName, const string_q& type);
extern string_q pinlib_unpinOneFile(const string_q& hash);

}  // namespace qblocks

using namespace qblocks;  // NOLINT
