#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

#include "etherlib.h"

#define BLOCK_CACHE SFString("/Volumes/Samsung_T3/scraper/")

#define contractAddrs  (BLOCK_CACHE+"contracts.bin")
#define fullBlockIndex (BLOCK_CACHE+"fullBlocks.bin")
#define miniBlockCache (BLOCK_CACHE+"miniBlocks.bin")
#define miniTransCache (BLOCK_CACHE+"miniTrans.bin")

//-----------------------------------------------------------------------
extern bool     readOneBlock_fromJson   (      CBlock& block,   const SFString& fileName);
extern bool     readOneBlock_fromBinary (      CBlock& block,   const SFString& fileName);
extern void     writeToJson             (const CBaseNode& node, const SFString& fileName);
extern void     writeToBinary           (const CBaseNode& node, const SFString& fileName);
extern bool     verifyBlock             (const CBlock& block,   SFString& result);

//-------------------------------------------------------------------------
extern SFString compileSolidity         (const SFString& sol);
extern SFString getClientVersion        (void);
extern SFUint32 getClientLatestBlk      (void);
extern bool     getLatestBlocks         (SFUint32& cache, SFUint32& client, CSharedResource *res=NULL);
extern void     freshenLocalCache       (bool indexOnly);

//-------------------------------------------------------------------------
extern bool     getCode                 (const SFAddress& addr, SFString& theCode);
inline SFString getCode                 (const SFAddress& addr) { SFString ret; getCode(addr, ret); return ret; }
extern SFUintBN getBalance              (const SFAddress& addr, blknum_t blockNum);
extern bool     getSha3                 (const SFString& hexIn, SFString& shaOut);
inline SFString getSha3                 (const SFString& hexIn) { SFString ret; getSha3(hexIn,ret); return ret; }

//-------------------------------------------------------------------------
extern bool     getBlock                (CBlock& block,       SFUint32 num);
extern bool     getTransaction          (CTransaction& trans, const SFString& hash);
extern bool     getReceipt              (CReceipt& receipt,   const SFString& hash);
extern bool     getLogEntry             (CLogEntry& log,      const SFString& hash);

//-------------------------------------------------------------------------
extern bool     queryBlock              (CBlock& block,       const SFString& num, bool needTrace);
extern bool     queryRawBlock           (SFString& block,     const SFString& num, bool needTrace, bool hashesOnly);
extern bool     queryRawTransaction     (const SFHash& txHash, SFString& results);
extern bool     queryRawReceipt         (const SFHash& txHash, SFString& results);
extern bool     queryRawLogs            (SFUint32 fromBlock, SFUint32 toBlock, const SFAddress& addr, SFString& results);

//-------------------------------------------------------------------------
extern void     etherlib_init           (const SFString& client);
extern void     etherlib_cleanup        (void);

//-------------------------------------------------------------------------
extern void     setStorageRoot          (const SFString& path);
extern SFString getJsonFilename1        (SFUint32 num);
extern SFString getBinaryFilename1      (SFUint32 num);
extern SFString getJsonPath1            (SFUint32 num);
extern SFString getBinaryPath1          (SFUint32 num);

//-------------------------------------------------------------------------
typedef bool (*BLOCKVISITFUNC)(CBlock& block, void *data);
typedef bool (*TRANSVISITFUNC)(CTransaction& trans, void *data);

//-------------------------------------------------------------------------
typedef bool (*MINIBLOCKVISITFUNC)(CMiniBlock& block, const CMiniTrans *trans, void *data);
typedef bool (*MINITRANSVISITFUNC)(CMiniTrans& trans, void *data);

//-------------------------------------------------------------------------
extern bool forEveryBlockOnDisc          (BLOCKVISITFUNC func, void *data, SFUint32 start=0, SFUint32 count=(SFUint32)-1);
extern bool forEveryEmptyBlockOnDisc     (BLOCKVISITFUNC func, void *data, SFUint32 start=0, SFUint32 count=(SFUint32)-1);
extern bool forEveryNonEmptyBlockOnDisc  (BLOCKVISITFUNC func, void *data, SFUint32 start=0, SFUint32 count=(SFUint32)-1);
extern bool forEveryFullBlockIndex       (BLOCKVISITFUNC func, void *data, SFUint32 start=0, SFUint32 count=(SFUint32)-1);

//-------------------------------------------------------------------------
extern bool forEveryTransaction          (TRANSVISITFUNC func, void *data, SFUint32 start=0, SFUint32 count=(SFUint32)-1);
extern bool forEveryTransactionTo        (TRANSVISITFUNC func, void *data, SFUint32 start=0, SFUint32 count=(SFUint32)-1);
extern bool forEveryTransactionFrom      (TRANSVISITFUNC func, void *data, SFUint32 start=0, SFUint32 count=(SFUint32)-1);

//-------------------------------------------------------------------------
extern bool forEveryFullBlockInMemory    (BLOCKVISITFUNC     func, void *data, SFUint32 start=0, SFUint32 count=(SFUint32)-1);
extern bool forEveryMiniBlockInMemory    (MINIBLOCKVISITFUNC func, void *data, SFUint32 start=0, SFUint32 count=(SFUint32)-1);
extern void clearInMemoryCache           (void);

//-------------------------------------------------------------------------
extern bool visitBlock                   (CBlock& block, void *data);
extern bool visitMini                    (CMiniBlock& block, const CMiniTrans *trans, void *data);

//-------------------------------------------------------------------------
class CBlockVisitor
{
public:
	CBlockVisitor(SFUint32 fb, SFUint32 c) : m_firstBlock(fb), m_cnt(c) { }
	SFUint32 firstBlock() const { return m_firstBlock; }
	SFUint32 getCount() const { return m_cnt; }
	void setFirst(SFUint32 n) { m_firstBlock = n; }
	void setCount(SFUint32 n) { m_cnt = n; }

protected:
	SFUint32 m_firstBlock;
	SFUint32 m_cnt;

private:
	CBlockVisitor(void) : m_firstBlock(0), m_cnt(0) { }
};

// Syntactic Sugar (TODO: These should be removed from the library)
//-------------------------------------------------------------------------
inline void   forEveryBlockFromClient    (BLOCKVISITFUNC     func, CBlockVisitor *bv) { forEveryBlockOnDisc        (func, bv, bv->firstBlock(), bv->getCount()); }
inline void   forEveryBlockOnDisc        (BLOCKVISITFUNC     func, CBlockVisitor *bv) { forEveryBlockOnDisc        (func, bv, bv->firstBlock(), bv->getCount()); }
inline void   forEveryNonEmptyBlockOnDisc(BLOCKVISITFUNC     func, CBlockVisitor *bv) { forEveryNonEmptyBlockOnDisc(func, bv, bv->firstBlock(), bv->getCount()); }
inline void   forEveryFullBlockInMemory  (BLOCKVISITFUNC     func, CBlockVisitor *bv) { forEveryFullBlockInMemory  (func, bv, bv->firstBlock(), bv->getCount()); }
inline void   forEveryMiniBlockInMemory  (MINIBLOCKVISITFUNC func, CBlockVisitor *bv) { forEveryMiniBlockInMemory  (func, bv, bv->firstBlock(), bv->getCount()); }

//-------------------------------------------------------------------------
// Helps debug a bloom filter
#define dbgBloom(a) fromBloom(a).Substitute("0"," ")
// \
//	.Substitute("0",SFString(iBlack)+"."+cOff) \
//	.Substitute("1",SFString(cGreen)+"1"+cOff) \
//	.Substitute("2",SFString(cRed)+"2"+cOff) \
//	.Substitute("4",SFString(cBlue)+"4"+cOff) \
//	.Substitute("8",SFString(cMagenta)+"8"+cOff) \

//-------------------------------------------------------------------------
inline SFUintBN makeBloom(const SFString& hexIn)
{
	SFString sha = getSha3(hexIn);
	SFUintBN bloom;
	for (uint32_t i=0;i<3;i++)
		bloom |= (SFUintBN(1) << (strtoul((const char*)"0x"+sha.substr(2+(i*4),4),NULL,16))%2048);
	return bloom;
}

//-------------------------------------------------------------------------
inline bool isBloomHit(const SFUintBN& test, const SFUintBN filter)
{
	return ((test & filter) == test);
}

//-------------------------------------------------------------------------
inline bool isBloomHit(const SFString& hexIn, const SFUintBN filter)
{
	return isBloomHit(makeBloom(hexIn),filter);
}
