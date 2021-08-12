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

extern bool pinlib_downloadManifest(void);
extern bool pinlib_readManifest(CPinnedChunkArray& pList);
extern bool pinlib_updateManifest(CPinnedChunkArray& pList);

extern bool pinlib_pinChunk(CPinnedChunkArray& pList, const string_q& fn, CPinnedChunk& item);
extern bool pinlib_unpinChunk(CPinnedChunkArray& pList, const string_q& fn, CPinnedChunk& item);

typedef enum { NO_TYPE = 0, BLOOM_TYPE = 1, CHUNK_TYPE = 2 } ipfsdown_t;
extern bool pinlib_getChunkFromRemote(CPinnedChunk& pin, ipfsdown_t which, double sleep);
extern bool pinlib_getChunkByHash(CPinnedChunkArray& pList, const string_q& fn, CPinnedChunk& item);
extern bool pinlib_findChunk(CPinnedChunkArray& pList, const string_q& fn, CPinnedChunk& item);

typedef bool (*PINFUNC)(CPinnedChunk& pin, void* data);
extern bool pinlib_forEveryPin(CPinnedChunkArray& pList, PINFUNC func, void* data);

extern bool freshenTimestamps(blknum_t minBlock);
extern bool loadTimestamps(uint32_t** theArray, size_t& cnt);

}  // namespace qblocks

//-------------------------------------------------------------------------
#define hashToIndexFormatFile string_q("Qmart6XP9XjL43p72PGR93QKytbK8jWWcMguhFgxATTya2")
#define hashToBloomFormatFile string_q("QmNhPk39DUFoEdhUmtGARqiFECUHeghyeryxZM9kyRxzHD")
#define unchainedIndexAddr string_q("0xcfd7f3b24f3551741f922fd8c4381aa4e00fc8fd")
#define manifestHashEncoding string_q("0x337f3f32")

using namespace qblocks;  // NOLINT
