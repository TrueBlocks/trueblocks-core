#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

#include "etherlib.h"

namespace qblocks {

    //-----------------------------------------------------------------------
    extern bool     readOneBlock_fromJson   (      CBlock& block,   const SFString& fileName);
    extern bool     readOneBlock_fromBinary (      CBlock& block,   const SFString& fileName);
    extern void     writeToJson             (const CBaseNode& node, const SFString& fileName);
    extern void     writeToBinary           (const CBaseNode& node, const SFString& fileName);
    extern bool     readFromJson            (      CBaseNode& node, const SFString& fileName);
    extern bool     readFromBinary          (      CBaseNode& node, const SFString& fileName);
    extern bool     verifyBlock             (const CBlock& block,   SFString& result);

    //-------------------------------------------------------------------------
    extern SFString compileSolidity         (const SFString& sol);
    extern SFString getVersionFromClient    (void);
    extern bool     getAccounts             (SFAddressArray& addrs);
    extern bool     isNodeRunning           (void);
    extern uint64_t getLatestBlockFromClient(void);
    extern uint64_t getLatestBlockFromCache (CSharedResource *res=NULL);
    extern uint64_t getLatestBloomFromCache (void);
    extern bool     getLatestBlocks         (uint64_t& cache, uint64_t& client, CSharedResource *res=NULL);

    //-------------------------------------------------------------------------
    extern bool     getCode                 (const SFAddress& addr, SFString& theCode);
    inline SFString getCode                 (const SFAddress& addr) { SFString ret; getCode(addr, ret); return ret; }
    inline bool     isContract              (const SFAddress& addr) { return !getCode(addr).Substitute("0x","").empty(); }
    extern SFUintBN getBalance              (const SFAddress& addr, blknum_t blockNum, bool isDemo);
    extern SFUintBN getTokenBalance         (const SFAddress& token, const SFAddress& holder, blknum_t blockNum);
    extern bool     getSha3                 (const SFString& hexIn, SFString& shaOut);
    inline SFString getSha3                 (const SFString& hexIn) { SFString ret; getSha3(hexIn,ret); return ret; }

    //-------------------------------------------------------------------------
    extern bool     getBlock                (CBlock& block,       blknum_t num);
    extern bool     getBlock                (CBlock& block,       const SFHash& hash);
    extern bool     getTransaction          (CTransaction& trans, const SFHash& hash);
    extern bool     getTransaction          (CTransaction& trans, const SFHash& hash, uint64_t transID);
    extern bool     getTransaction          (CTransaction& trans, blknum_t blockNum, uint64_t transID);
    extern bool     getReceipt              (CReceipt& receipt,   const SFString& hash);
    extern bool     getLogEntry             (CLogEntry& log,      const SFString& hash);
    extern void     getTraces               (CTraceArray& traces, const SFHash& hash);

    //-------------------------------------------------------------------------
    extern bool     queryBlock              (CBlock& block,       const SFString& num, bool needTrace, bool byHash, uint32_t& nTraces);
    extern bool     queryBlock              (CBlock& block,       const SFString& num, bool needTrace, bool byHash);
    extern bool     queryRawBlock           (SFString& block,     const SFString& num, bool needTrace, bool hashesOnly);
    extern bool     queryRawTransaction     (SFString& results,   const SFHash& txHash);
    extern bool     queryRawReceipt         (SFString& results,   const SFHash& txHash);
    extern bool     queryRawLogs            (const SFString& results, const SFAddress& addr, uint64_t fromBlock, uint64_t toBlock);
    extern bool     queryRawTrace           (SFString& trace,     const SFString& hashIn);

    //-------------------------------------------------------------------------
    extern void     etherlib_init           (const SFString& client);
    extern void     etherlib_cleanup        (void);

    //-------------------------------------------------------------------------
    extern void     setStorageRoot          (const SFString& path);
    extern SFString getStorageRoot          (void);
    extern SFString getJsonFilename1        (uint64_t num);
    extern SFString getBinaryFilename1      (uint64_t num);
    extern SFString getJsonPath1            (uint64_t num);
    extern SFString getBinaryPath1          (uint64_t num);
    extern void     setNoTracing            (bool val);

    //-------------------------------------------------------------------------
    typedef bool (*BLOCKVISITFUNC)(CBlock& block, void *data);
    typedef bool (*TRANSVISITFUNC)(CTransaction& trans, void *data);

    //-------------------------------------------------------------------------
    typedef bool (*MINIBLOCKVISITFUNC)(CMiniBlock& block, const CMiniTrans *trans, void *data);
    typedef bool (*MINITRANSVISITFUNC)(CMiniTrans& trans, void *data);

    //-------------------------------------------------------------------------
    extern bool forEveryBlock                (BLOCKVISITFUNC func, void *data, const SFString& block_list);
    extern bool forEveryBlockOnDisc          (BLOCKVISITFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip=1);
    extern bool forEveryEmptyBlockOnDisc     (BLOCKVISITFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip=1);
    extern bool forEveryNonEmptyBlockOnDisc  (BLOCKVISITFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip=1);
    extern bool forEveryFullBlockIndex       (BLOCKVISITFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip=1);
    extern bool forEveryBlock                (BLOCKVISITFUNC func, void *data, uint64_t start, uint64_t count, uint64_t skip=1);
    extern bool forEveryBloomFile            (FILEVISITOR func,    void *data, uint64_t start, uint64_t count, uint64_t skip=1);

    //-------------------------------------------------------------------------
    extern bool forEveryTransaction          (TRANSVISITFUNC func, void *data, const SFString& trans_list);

    //-------------------------------------------------------------------------
    extern bool forEveryFullBlockInMemory    (BLOCKVISITFUNC     func, void *data, uint64_t start, uint64_t count);
    extern bool forEveryMiniBlockInMemory    (MINIBLOCKVISITFUNC func, void *data, uint64_t start, uint64_t count);
    extern bool forOnlyMiniBlocks            (MINIBLOCKVISITFUNC func, void *data, uint64_t start, uint64_t count);
    extern bool forOnlyMiniTransactions      (MINITRANSVISITFUNC func, void *data, uint64_t start, uint64_t count);
    extern void clearInMemoryCache           (void);

    //-------------------------------------------------------------------------
    class CBlockVisitor
    {
    public:
        CBlockVisitor(uint64_t fb, uint64_t c) : m_firstBlock(fb), m_cnt(c) { }
        uint64_t firstBlock() const { return m_firstBlock; }
        uint64_t getCount() const { return m_cnt; }
        void setFirst(uint64_t n) { m_firstBlock = n; }
        void setCount(uint64_t n) { m_cnt = n; }

    protected:
        uint64_t m_firstBlock;
        uint64_t m_cnt;

    private:
        CBlockVisitor(void) : m_firstBlock(0), m_cnt(0) { }
    };

    extern SFString blockCachePath(const SFString& _part);

#define fullBlockIndex (blockCachePath("fullBlocks.bin"))
#define miniBlockCache (blockCachePath("miniBlocks.bin"))
#define miniTransCache (blockCachePath("miniTrans.bin"))
#define blockFolder    (blockCachePath("blocks/"))
#define bloomFolder    (blockCachePath("blooms/"))

    //--------------------------------------------------------------------------
    inline SFString TIMER_IN(double& startTime) {
        CStringExportContext ctx;
        ctx << (qbNow()-startTime) << ": ";
        startTime = qbNow();
        return ctx.str;
    }

    //-------------------------------------------------------------------------
    inline SFString TIMER_TICK(double startTime) {
        CStringExportContext ctx;
        ctx << "in " << cGreen << (qbNow()-startTime) << cOff << " seconds.";
        return ctx.str;
    }

}  // namespace qblocks
