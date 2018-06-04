#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks(tm) - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
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

namespace qblocks {

    //-------------------------------------------------------------------------
    // setup and tear down of the library
    extern void     etherlib_init           (const SFString& primarySource="binary", QUITHANDLER qh=defaultQuitHandler);
    inline void     etherlib_init           (QUITHANDLER qh) { etherlib_init("binary", qh); };
    extern void     etherlib_cleanup        (void);

    //-------------------------------------------------------------------------
    // fastest methods to access data
    extern bool     getBlock                (CBlock& block,       blknum_t blockNum);
    extern bool     getTransaction          (CTransaction& trans, blknum_t blockNum, txnum_t txID);
    extern bool     getReceipt              (CReceipt& receipt,   const SFHash& txHash);
    extern bool     getLogEntry             (CLogEntry& log,      const SFHash& txHash);
    extern void     getTraces               (CTraceArray& traces, const SFHash& txHash);
    extern uint32_t getTraceCount           (const SFHash& hashIn);

    //-------------------------------------------------------------------------
    // other methods to access data
    extern bool     getBlock                (CBlock& block,       const SFHash& blockHash);
    extern bool     getTransaction          (CTransaction& trans, const SFHash& txHash);
    extern bool     getTransaction          (CTransaction& trans, const SFHash& blockHash, txnum_t txID);

    //-------------------------------------------------------------------------
    extern bool     queryBlock              (CBlock& block,       const SFString& num, bool needTrace, bool byHash, uint32_t& nTraces);
    extern bool     queryBlock              (CBlock& block,       const SFString& num, bool needTrace, bool byHash);

    //-------------------------------------------------------------------------
    // lower level access to the node's responses
    extern bool     queryRawBlock           (SFString& results,   const SFString& blockNum, bool needTrace, bool hashesOnly);
    extern bool     queryRawTransaction     (SFString& results,   const SFHash& txHash);
    extern bool     queryRawReceipt         (SFString& results,   const SFHash& txHash);
    extern bool     queryRawLog             (SFString& results,   const SFHash& hashIn);
    extern bool     queryRawTrace           (SFString& results,   const SFHash& hashIn);
    extern bool     queryRawLogs            (SFString& results,   const SFAddress& addr, uint64_t fromBlock, uint64_t toBlock);

    //-----------------------------------------------------------------------
    extern SFHash   getRawBlock             (blknum_t bn);
    extern SFHash   getRawBlockHash         (blknum_t bn);
    extern SFHash   getRawTransactionHash   (blknum_t bn, txnum_t tx);

    //-----------------------------------------------------------------------
    extern void     writeToJson             (const CBaseNode& node, const SFString& fileName);
    extern bool     readFromJson            (      CBaseNode& node, const SFString& fileName);

    //-----------------------------------------------------------------------
    extern bool     writeBlockToBinary      (const CBlock& block, const SFString& fileName);
    extern bool     readBlockFromBinary     (      CBlock& block, const SFString& fileName);

    //-------------------------------------------------------------------------
    extern SFString getVersionFromClient    (void);
    inline bool     isGeth                  (void) { return getVersionFromClient().ContainsI("geth"); }
    inline bool     isParity                (void) { return getVersionFromClient().ContainsI("parity"); }
    extern bool     getAccounts             (SFAddressArray& addrs);
    extern uint64_t getLatestBlockFromClient(void);
    extern uint64_t getLatestBlockFromCache (void);
    extern bool     getLatestBlocks         (uint64_t& cache, uint64_t& client);

    //-------------------------------------------------------------------------
    extern bool     getCode                 (const SFAddress& addr, SFString& theCode);
    inline SFString getCode                 (const SFAddress& addr) { SFString ret; getCode(addr, ret); return ret; }
    inline bool     isContract              (const SFAddress& addr) { return !getCode(addr).Substitute("0x","").empty(); }
    extern SFUintBN getBalance              (const SFAddress& addr, blknum_t blockNum, bool isDemo);
    extern bool     getSha3                 (const SFString& hexIn, SFString& shaOut);
    inline SFString getSha3                 (const SFString& hexIn) { SFString ret; getSha3(hexIn,ret); return ret; }

    //-------------------------------------------------------------------------
    extern SFString getJsonFilename         (uint64_t num);
    extern SFString getBinaryFilename       (uint64_t num);
    extern SFString getBinaryPath           (uint64_t num);

    //-------------------------------------------------------------------------
    // function pointer types for forEvery functions
    typedef bool (*BLOCKVISITFUNC)          (CBlock& block, void *data);
    typedef bool (*TRANSVISITFUNC)          (CTransaction& trans, void *data);
    typedef bool (*LOGVISITFUNC)            (CLogEntry& log, void *data);
    typedef bool (*TRACEVISITFUNC)          (CTrace& trace, void *data);

    //-------------------------------------------------------------------------
    // forEvery functions
    extern bool forEveryBlock                (BLOCKVISITFUNC func, void *data, const SFString& block_list);
    extern bool forEveryBlock                (BLOCKVISITFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip=1);
    extern bool forEveryBlockOnDisc          (BLOCKVISITFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip=1);
    extern bool forEveryNonEmptyBlockOnDisc  (BLOCKVISITFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip=1);
    extern bool forEveryEmptyBlockOnDisc     (BLOCKVISITFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip=1);

    //-------------------------------------------------------------------------
    // forEvery functions
    extern bool forEveryBloomFile            (FILEVISITOR    func, void *data, uint64_t start, uint64_t count, uint64_t skip=1);

    //-------------------------------------------------------------------------
    // forEvery functions
    extern bool forEveryTransactionInList    (TRANSVISITFUNC func, void *data, const SFString& trans_list);
    extern bool forEveryTransactionInBlock   (TRANSVISITFUNC func, void *data, const CBlock& block);

    //-------------------------------------------------------------------------
    // forEvery functions
    extern bool forEveryTraceInTransaction   (TRACEVISITFUNC func, void *data, const CTransaction& trans);
    extern bool forEveryTraceInBlock         (TRACEVISITFUNC func, void *data, const CBlock& block);
    extern bool forEveryLogInTransaction     (LOGVISITFUNC func,   void *data, const CTransaction& trans);
    extern bool forEveryLogInBlock           (LOGVISITFUNC func,   void *data, const CBlock& block);

    //-------------------------------------------------------------------------
    extern SFString blockCachePath(const SFString& _part);

    #define fullBlockIndex (blockCachePath("fullBlocks.bin"))
    #define accountIndex   (blockCachePath("accountTree.bin"))
    #define miniBlockCache (blockCachePath("miniBlocks.bin"))
    #define miniTransCache (blockCachePath("miniTrans.bin"))
    #define blockFolder    (blockCachePath("blocks/"))
    #define bloomFolder    (blockCachePath("blooms/"))
    extern SFUintBN weiPerEther;

}  // namespace qblocks

//-------------------------------------------------------------------------
extern bool visitBlockNumber(blknum_t bn,         void *data);
extern bool visitBlock      (CBlock& block,       void *data);
extern bool visitTransaction(CTransaction& trans, void *data);
extern bool visitLog        (CLogEntry& log,      void *data);
extern bool visitTrace      (CTrace& trace,       void *data);

