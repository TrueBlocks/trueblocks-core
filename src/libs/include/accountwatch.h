#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
/*
 * This file was generated with makeClass. Edit only those parts of the code inside
 * of 'EXISTING_CODE' tags.
 */
#include "ethtypes.h"
#include "abilib.h"
#include "transaction.h"
#include "incomestatement.h"

namespace qblocks {

//--------------------------------------------------------------------------
class CAccountWatch;
typedef SFArrayBase<CAccountWatch>         CAccountWatchArray;
typedef SFList<CAccountWatch*>             CAccountWatchList;
typedef SFUniqueList<CAccountWatch*>       CAccountWatchListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CAccountWatch : public CBaseNode {
public:
    SFUint32 index;
    SFAddress address;
    SFString name;
    SFString color;
    SFUint32 firstBlock;
    SFUint32 lastBlock;
    SFString status;
    bool deepScan;
    CIncomeStatement qbis;
    SFUintBN nodeBal;

public:
    CAccountWatch(void);
    CAccountWatch(const CAccountWatch& ac);
    ~CAccountWatch(void);
    CAccountWatch& operator=(const CAccountWatch& ac);

    DECLARE_NODE(CAccountWatch);

    // EXISTING_CODE
    CAccountWatch(uint32_t _id, const SFString& _addr, const SFString& _name, blknum_t fB, blknum_t lB, const SFString& _color)
    : index(_id), address(toLower(_addr)), name(_name), color(_color), firstBlock(fB), lastBlock(lB), status("") { }
    bool getWatch(const CToml& toml, uint32_t n);
    SFString displayName(bool terse, int w1=20, int w2=8) const;
    bool isTransactionOfInterest(CTransaction *trans, uint64_t nSigs, SFString sigs[]) const;
    // EXISTING_CODE

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CAccountWatch& ac);
    bool readBackLevel(SFArchive& archive);

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAccountWatch::CAccountWatch(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAccountWatch::CAccountWatch(const CAccountWatch& ac) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(ac);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAccountWatch::~CAccountWatch(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccountWatch::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccountWatch::Init(void) {
    CBaseNode::Init();

    index = 0;
//    address = EMPTY;
//    name = EMPTY;
//    color = EMPTY;
    firstBlock = 0;
    lastBlock = 0;
//    status = EMPTY;
    deepScan = 0;
//    qbis = ??; /* unknown type: CIncomeStatement */
    nodeBal = 0;

    // EXISTING_CODE
    lastBlock = UINT_MAX;
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccountWatch::Copy(const CAccountWatch& ac) {
    Clear();
    CBaseNode::Copy(ac);

    index = ac.index;
    address = ac.address;
    name = ac.name;
    color = ac.color;
    firstBlock = ac.firstBlock;
    lastBlock = ac.lastBlock;
    status = ac.status;
    deepScan = ac.deepScan;
    qbis = ac.qbis;
    nodeBal = ac.nodeBal;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CAccountWatch& CAccountWatch::operator=(const CAccountWatch& ac) {
    Copy(ac);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
inline SFString CAccountWatch::getValueByName(const SFString& fieldName) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return Format("[{"+toUpper(fieldName)+"}]");
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CAccountWatchArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CAccountWatchArray);
IMPLEMENT_ARCHIVE_LIST(CAccountWatchList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

