#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
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

namespace qblocks {

    //-------------------------------------------------------------------------
    // setup and tear down of the library
    extern void     etherlib_init           (const string_q& primarySource, QUITHANDLER qh);
    inline void     etherlib_init           (QUITHANDLER qh) { etherlib_init("binary", qh); }
    extern void     etherlib_cleanup        (void);

    //-------------------------------------------------------------------------
    // fastest methods to access data
    extern bool     getBlock                (CBlock& block,       blknum_t blockNum);
    extern bool     getBlock                (CBlock& block,       const hash_t& blockHash);
    extern bool     getTransaction          (CTransaction& trans, blknum_t blockNum, txnum_t txID);
    extern bool     getTransaction          (CTransaction& trans, const hash_t& txHash);
    extern bool     getTransaction          (CTransaction& trans, const hash_t& blockHash, txnum_t txID);
    extern bool     getReceipt              (CReceipt& receipt,   const hash_t& txHash);
    extern bool     getLogEntry             (CLogEntry& log,      const hash_t& txHash);
    extern void     getTraces               (CTraceArray& traces, const hash_t& txHash);
    extern size_t   getTraceCount           (const hash_t& hashIn);
    extern bool     loadTraces              (CTransaction& trans, blknum_t bn, blknum_t txid, bool useCache, bool skipDdos);
    extern bool     getFullReceipt          (CTransaction *trans, bool needsTrace);

    //-------------------------------------------------------------------------
    extern bool     queryBlock              (CBlock& block, const string_q& num, bool needTrace);

    //-------------------------------------------------------------------------
    // lower level access to the node's responses
    extern bool     queryRawBlock           (string_q& results, const string_q& blockNum, bool needTrace, bool hashesOnly);
    extern bool     queryRawTransaction     (string_q& results, const hash_t& txHash);
    extern bool     queryRawReceipt         (string_q& results, const hash_t& txHash);
    extern bool     queryRawTrace           (string_q& results, const hash_t& hashIn);
    extern bool     queryRawLogs            (string_q& results, uint64_t fromBlock, uint64_t toBlock, const address_t& addr, const CTopicArray& topics);
    inline bool     queryRawLogs            (string_q& results, uint64_t fromBlock, uint64_t toBlock, const address_t& addr) { CTopicArray unused2; return queryRawLogs(results, fromBlock, toBlock, addr, unused2); }
    inline bool     queryRawLogs            (string_q& results, uint64_t fromBlock, uint64_t toBlock) { address_t unused1; CTopicArray unused2; return queryRawLogs(results, fromBlock, toBlock, unused1, unused2); }
    extern bool     queryRawLogs            (string_q& results, const hash_t& hash, const address_t& addr, const CTopicArray& topics);
    inline bool     queryRawLogs            (string_q& results, const hash_t& hash, const address_t& addr) { CTopicArray unused2; return queryRawLogs(results, hash, addr, unused2); }
    inline bool     queryRawLogs            (string_q& results, const hash_t& hash) { address_t unused1; CTopicArray unused2; return queryRawLogs(results, hash, unused1, unused2); }

    //-----------------------------------------------------------------------
    extern string_q getRawBlock             (blknum_t bn);
    extern hash_t   getRawBlockHash         (blknum_t bn);
    extern hash_t   getRawTransactionHash   (blknum_t bn, txnum_t tx);

    //-----------------------------------------------------------------------
    extern bool     getBlock_light          (CBlock& block, const string_q& val);
    extern bool     getBlock_light          (CBlock& block, blknum_t num);

    //-----------------------------------------------------------------------
    extern void     writeToJson             (const CBaseNode& node, const string_q& fileName);
    extern bool     readFromJson            (      CBaseNode& node, const string_q& fileName);

    //-----------------------------------------------------------------------
    extern bool     writeNodeToBinary       (const CBaseNode& node, const string_q& fileName);
    extern bool     readNodeFromBinary      (CBaseNode& item, const string_q& fileName);
    #define writeBlockToBinary  writeNodeToBinary
    #define writeTransToBinary  writeNodeToBinary
    #define readBlockFromBinary readNodeFromBinary
    #define readTransFromBinary readNodeFromBinary

    //-------------------------------------------------------------------------
    extern string_q getVersionFromClient    (void);
    inline bool     isGeth                  (void) { return contains(toLower(getVersionFromClient()), "geth"); }  // NOLINT
    inline bool     isParity                (void) { return contains(toLower(getVersionFromClient()), "parity"); }  // NOLINT
    extern bool     getAccounts             (CAddressArray& addrs);

    //-------------------------------------------------------------------------
    uint64_t        addFilter               (address_t addr, const CTopicArray& topics, blknum_t block);
    string_q        getSha3                 (const string_q& hexIn);

    //-------------------------------------------------------------------------
    extern string_q getBinaryCacheFilename  (cache_t ct, blknum_t bn, txnum_t txid=NOPOS, const string_q& trc_id="");
    extern string_q getBinaryCachePath      (cache_t ct, blknum_t bn, txnum_t txid=NOPOS, const string_q& trc_id="");
    extern string_q getBinaryCacheFilename  (cache_t ct, const address_t& addr);
    extern string_q getBinaryCachePath      (cache_t ct, const address_t& addr);

    //-------------------------------------------------------------------------
    // function pointer types for forEvery functions
    typedef bool (*BLOCKVISITFUNC)          (CBlock& block, void *data);
    typedef bool (*TRANSVISITFUNC)          (CTransaction& trans, void *data);
    typedef bool (*LOGVISITFUNC)            (CLogEntry& log, void *data);
    typedef bool (*TRACEVISITFUNC)          (CTrace& trace, void *data);

    //-------------------------------------------------------------------------
    // forEvery functions
    extern bool forEveryBlock                (BLOCKVISITFUNC func, void *data, const string_q& block_list);  // NOLINT
    extern bool forEveryBlock                (BLOCKVISITFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip=1);  // NOLINT
    extern bool forEveryBlockOnDisc          (BLOCKVISITFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip=1);  // NOLINT

    //-------------------------------------------------------------------------
    // forEvery functions
    extern bool forEveryTransactionInList    (TRANSVISITFUNC func, void *data, const string_q& trans_list);
    extern bool forEveryTransactionInBlock   (TRANSVISITFUNC func, void *data, const CBlock& block);

    //-------------------------------------------------------------------------
    // forEvery functions
    extern bool forEveryTraceInTransaction   (TRACEVISITFUNC func, void *data, const CTransaction& trans);
    extern bool forEveryTraceInBlock         (TRACEVISITFUNC func, void *data, const CBlock& block);
    extern bool forEveryLogInTransaction     (LOGVISITFUNC func,   void *data, const CTransaction& trans);
    extern bool forEveryLogInBlock           (LOGVISITFUNC func,   void *data, const CBlock& block);

    //-------------------------------------------------------------------------
    extern blknum_t getLastBlock_client       (void);
    extern blknum_t getLastBlock_cache_final  (void);
    extern bool     getLastBlocks             (blknum_t& unripe, blknum_t& ripe, blknum_t& staging, blknum_t& final, blknum_t& client);

    //-------------------------------------------------------------------------
    extern string_q getCachePath            (const string_q& _part);
    extern string_q getIndexPath            (const string_q& _part);
    extern string_q scraperStatus           (bool terse);

    //-------------------------------------------------------------------------
    #define blockFolder           (getCachePath("blocks/"))

    #define indexFolder           (getIndexPath(""))
    #define indexFolder_staging   (getIndexPath("staging/"))
    #define indexFolder_unripe    (getIndexPath("unripe/"))
    #define indexFolder_ripe      (getIndexPath("ripe/"))
    #define indexFolder_finalized (getIndexPath("finalized/"))
    #define indexFolder_blooms    (getIndexPath("blooms/"))

    //-------------------------------------------------------------------------
    extern biguint_t weiPerEther(void);

    //-------------------------------------------------------------------------
    extern void manageFields(const string_q& formatStr);
    extern void manageFields(const string_q& listIn, bool show);
    extern const string_q defHide;
    extern const string_q defShow;

    //-------------------------------------------------------------------------
    string_q exportPreamble(format_t fmt, const string_q& format, const CRuntimeClass *pClass);
    string_q exportPostamble(format_t fmt, const string_q& extra);

    //-------------------------------------------------------------------------
    bool findTimestamp_binarySearch(CBlock& block, size_t first, size_t last, bool progress=false);

    //-------------------------------------------------------------------------
    extern wei_t blockReward(blknum_t bn, blknum_t txid, bool txFee);
    extern bool excludeTrace(const CTransaction *trans, size_t maxTraces);

}  // namespace qblocks

//-------------------------------------------------------------------------
extern bool visitBlockNumber(blknum_t bn,         void *data);
extern bool visitBlock      (CBlock& block,       void *data);
extern bool visitTransaction(CTransaction& trans, void *data);
extern bool visitLog        (CLogEntry& log,      void *data);
extern bool visitTrace      (CTrace& trace,       void *data);
