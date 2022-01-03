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
#include "etherlib.h"
#include "node_curl.h"
#include "cacheentry.h"
#include "rewards.h"
#include "logfilter.h"
#include "ethcall.h"
#include "metadata.h"

namespace qblocks {

//-------------------------------------------------------------------------
// setup and tear down of the library
extern void etherlib_init(QUITHANDLER qh);
extern void etherlib_cleanup(void);

//-------------------------------------------------------------------------
// fastest methods to access data
extern bool getBlock(CBlock& block, blknum_t blockNum);
extern bool getBlock(CBlock& block, const hash_t& blockHash);
extern bool getUncle(CBlock& block, blknum_t blockNum, size_t index);
extern bool getUncle(CBlock& block, const hash_t& blockHash, size_t index);
extern size_t getUncleCount(blknum_t blockNum);
extern size_t getUncleCount(const hash_t& blockHash);
extern bool getTransaction(CTransaction& trans, blknum_t blockNum, txnum_t txID);
extern bool getTransaction(CTransaction& trans, const hash_t& txHash);
extern bool getTransaction(CTransaction& trans, const hash_t& blockHash, txnum_t txID);
extern bool getReceipt(CReceipt& receipt, const hash_t& txHash);
extern bool getLogEntry(CLogEntry& log, const hash_t& txHash);
extern void getTraces(CTraceArray& traces, const hash_t& txHash);
extern void getStateDiffAddrs(CAddressArray& addrs, const hash_t& txHash);
extern void getTracesByFilter(CTraceArray& traces, const CTraceFilter& filter);
extern size_t getTraceCount(const hash_t& hashIn);
extern bool loadTraces(CTransaction& trans, blknum_t bn, blknum_t txid, bool useCache, bool skipDdos);
extern bool getFullReceipt(CTransaction* trans, bool needsTrace);

//-------------------------------------------------------------------------
extern bool queryBlock(CBlock& block, const string_q& num, bool needTrace);
extern bool queryUncle(CBlock& block, const string_q& num, size_t index);
extern size_t queryUncleCount(const string_q& num);

//-------------------------------------------------------------------------
// lower level access to the node's responses
extern bool queryRawBlock(string_q& results, const string_q& blockNum, bool needTrace, bool hashesOnly);
extern bool queryRawBlockTrace(string_q& results, const string_q& hexNum);
inline bool queryRawBlockTrace(string_q& results, blknum_t blockNum) {
    return queryRawBlockTrace(results, uint_2_Hex(blockNum));
}
extern bool queryRawUncle(string_q& results, const string_q& blockNum, uint64_t index);
extern bool queryRawTransaction(string_q& results, const hash_t& txHash);
extern bool queryRawReceipt(string_q& results, const hash_t& txHash);
extern bool queryRawTrace(string_q& results, const hash_t& hashIn);
extern bool queryRawStateDiff(string_q& results, const hash_t& hashIn);
extern bool queryRawLogs(string_q& results, const CLogFilter& query);

//-----------------------------------------------------------------------
extern string_q getTokenBalanceOf(const address_t& token, const address_t& holder, blknum_t blockNum);
inline bigint_t getTokenBalanceOf2(const address_t& token, const address_t& holder, blknum_t blockNum) {
    return str_2_BigInt(getTokenBalanceOf(token, holder, blockNum));
};
extern string_q getTokenSymbol(const address_t& token, blknum_t blockNum);
extern string_q getTokenState(const address_t& token, const string_q& whichState, const CAbi& abi_spec,
                              blknum_t blockNum, const string_q& bytes = "");

//-----------------------------------------------------------------------
// https://ethereum.stackexchange.com/questions/82822/obtaining-erc721-interface-ids
#define _INTERFACE_ID_ERC721 "0x80ac58cd"
#define _INTERFACE_ID_ERC721_METADATA "0x5b5e139f"
#define _INTERFACE_ID_ERC721_ENUMERABLE "0x780e9d63"

//-----------------------------------------------------------------------
extern string_q getRawBlock(blknum_t bn);
extern string_q getRawUncle(blknum_t bn, uint64_t index);
extern hash_t getRawBlockHash(blknum_t bn);
extern hash_t getRawTransactionHash(blknum_t bn, txnum_t tx);

//-----------------------------------------------------------------------
extern bool getBlock_light(CBlock& block, const string_q& val);
extern bool getBlock_light(CBlock& block, blknum_t num);
extern bool getBlock_header(CBlock& block, const string_q& val);
extern bool getBlock_header(CBlock& block, blknum_t num);

//-----------------------------------------------------------------------
extern void writeToJson(const CBaseNode& node, const string_q& fileName);
extern bool readFromJson(CBaseNode& node, const string_q& fileName);

//-----------------------------------------------------------------------
extern bool writeNodeToBinary(const CBaseNode& node, const string_q& fileName);
extern bool readNodeFromBinary(CBaseNode& item, const string_q& fileName);
#define writeBlockToBinary writeNodeToBinary
#define writeTransToBinary writeNodeToBinary
#define readBlockFromBinary readNodeFromBinary
#define readTransFromBinary readNodeFromBinary

//-------------------------------------------------------------------------
extern string_q getVersionFromClient(void);
extern bool isErigon(void);
extern bool isGeth(void);
extern bool isParity(void);
extern bool hasParityTraces(void);
extern bool getNodeIds(uint64_t& clientId, uint64_t& networkId);

//-------------------------------------------------------------------------
uint64_t addFilter(address_t addr, const CTopicArray& topics, blknum_t block);

//-------------------------------------------------------------------------
extern string_q getBinaryCacheFilename(cache_t ct, blknum_t bn, txnum_t txid = NOPOS, const string_q& trc_id = "");
extern string_q getPathToBinaryCache(cache_t ct, blknum_t bn, txnum_t txid = NOPOS, const string_q& trc_id = "");
extern string_q getBinaryCacheFilename(cache_t ct, const address_t& addr, blknum_t bn = NOPOS, txnum_t txid = NOPOS);
extern string_q getPathToBinaryCache(cache_t ct, const address_t& addr, blknum_t bn = NOPOS, txnum_t txid = NOPOS);

//-------------------------------------------------------------------------
// function pointer types for forEvery functions
typedef bool (*BLOCKVISITFUNC)(CBlock& block, void* data);
typedef bool (*TRANSVISITFUNC)(CTransaction& trans, void* data);
typedef bool (*LOGVISITFUNC)(CLogEntry& log, void* data);
typedef bool (*TRACEVISITFUNC)(CTrace& trace, void* data);
typedef bool (*ABIVISITFUNC)(CAbi& abi_spec, void* data);

//-------------------------------------------------------------------------
// forEvery functions
extern bool forEveryBlock(BLOCKVISITFUNC func, void* data, const string_q& block_list);  // NOLINT
extern bool forEveryBlock(BLOCKVISITFUNC func, void* data, uint64_t start, uint64_t count,
                          uint64_t skip = 1);  // NOLINT
extern bool forEveryBlock_light(BLOCKVISITFUNC func, void* data, uint64_t start, uint64_t count,
                                uint64_t skip = 1);  // NOLINT
extern bool forEveryTransaction(TRANSVISITFUNC func, void* data, const string_q& trans_list);
extern bool forEveryTimestamp(BLOCKVISITFUNC func, void* data);

//-------------------------------------------------------------------------
#define blockFolder (getPathToCache("blocks/"))

#define indexFolder (getPathToIndex(""))
#define indexFolder_staging (getPathToIndex("staging/"))
#define indexFolder_unripe (getPathToIndex("unripe/"))
#define indexFolder_ripe (getPathToIndex("ripe/"))
#define indexFolder_finalized (getPathToIndex("finalized/"))
#define indexFolder_blooms (getPathToIndex("blooms/"))
#define tsIndex getPathToIndex("ts.bin")

//-------------------------------------------------------------------------
extern biguint_t weiPerEther(void);

//-------------------------------------------------------------------------
extern const string_q defHide;
extern const string_q defShow;

//-------------------------------------------------------------------------
string_q exportPreamble(const string_q& format, const string_q& className);
inline string_q exportPreamble(const string_q& format, const CRuntimeClass* pClass) {
    return exportPreamble(format, (pClass ? pClass->m_ClassName : "unknown"));
}
string_q exportPostamble(const CStringArray& errors, const string_q& extra);

//-------------------------------------------------------------------------
extern bool excludeTrace(const CTransaction* trans, size_t maxTraces);

extern wei_t getBalanceAt(const address_t& addr, blknum_t blockNum);

#ifdef LOGGING_LEVEL
//--------------------------------------------------------------------------
inline string_q relativize(const string_q& path) {
    string_q ret = path;
    replace(ret, getPathToIndex(""), "$INDEX/");
    replace(ret, getPathToCache(""), "$CACHE/");
    replace(ret, getPathToConfig(""), "$CONFIG/");
    replace(ret, getHomeFolder(), "$HOME/");
    return ret;
}
//--------------------------------------------------------------------------
#define LOG_FN3(fn)                                                                                                    \
    {                                                                                                                  \
        string_q lfn8 = relativize((fn));                                                                              \
        LOG3(padRight((string_q(#fn) + ":"), 25), lfn8, " ", fileSize((fn)));                                          \
    }
#define LOG_DIR3(dir)                                                                                                  \
    {                                                                                                                  \
        string_q lfn8 = relativize((dir));                                                                             \
        LOG3(padRight((string_q(#dir) + ":"), 25), lfn8);                                                              \
    }
//--------------------------------------------------------------------------
#define LOG_FN8(fn)                                                                                                    \
    {                                                                                                                  \
        string_q lfn8 = relativize((fn));                                                                              \
        LOG8(padRight((string_q(#fn) + ":"), 25), lfn8, " ", fileSize((fn)));                                          \
    }
#define LOG_DIR8(dir)                                                                                                  \
    {                                                                                                                  \
        string_q lfn8 = relativize((dir));                                                                             \
        LOG8(padRight((string_q(#dir) + ":"), 25), lfn8);                                                              \
    }
#else
#define LOG_FN3(fn)
#define LOG_DIR3(dir)
#define LOG_FN8(fn)
#define LOG_DIR8(dir)
#endif
}  // namespace qblocks

//-------------------------------------------------------------------------
extern bool visitBlockNumber(blknum_t bn, void* data);
extern bool visitBlock(CBlock& block, void* data);
extern bool visitTransaction(CTransaction& trans, void* data);
extern bool visitLog(CLogEntry& log, void* data);
extern bool visitTrace(CTrace& trace, void* data);
