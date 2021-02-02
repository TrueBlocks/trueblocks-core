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
/*
 * This file was generated with makeClass. Edit only those parts of the code inside
 * of 'EXISTING_CODE' tags.
 */
#include "pinlib.h"
#include "pinatalicense.h"

namespace qblocks {

extern bool ipfsExists(void);

extern bool pinChunk(const string_q& fileName, CPinnedItem& item);
extern bool unpinChunk(const string_q& fileName, CPinnedItem& item);

extern bool publishManifest(ostream& os);
extern bool freshenBloomFilters(bool download);
extern hash_t getCurrentManifest(void);
extern hash_t getLastManifest(void);

extern bool findChunk(const string_q& fileName, CPinnedItem& item);

extern bool pinataListOfPins(string_q& result);
extern void loadPinMaps(CIndexHashMap& bloomMap, CIndexHashMap& indexMap);

extern bool unpinChunkByHash(const string_q& hash);
extern bool getChunkByHash(const string_q& fileName, CPinnedItem& item);
extern bool getFileByHash(const hash_t& hash, const string_q& outFilename);
extern string_q getFileContentsByHash(const hash_t& hash);

extern bool removeFromPinata(CPinnedItem& item, void* data);

typedef bool (*PINFUNC)(CPinnedItem& pin, void* data);
extern bool forEveryPin(PINFUNC func, void* data);

//---------------------------------------------------------------------------
#define hashToEmptyFile "QmP4i6ihnVrj8Tx7cTFw4aY6ungpaPYxDJEZ7Vg1RSNSdm"
#define hashToIndexFormatFile "Qmart6XP9XjL43p72PGR93QKytbK8jWWcMguhFgxATTya2"
#define hashToBloomFormatFile "QmNhPk39DUFoEdhUmtGARqiFECUHeghyeryxZM9kyRxzHD"
#define unchainedIndexAddr "0xcfd7f3b24f3551741f922fd8c4381aa4e00fc8fd"
#define manifestHashEncoding "0x337f3f32"

}  // namespace qblocks
