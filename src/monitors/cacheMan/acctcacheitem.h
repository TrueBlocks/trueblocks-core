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
#include "utillib.h"
#include "options.h"

//--------------------------------------------------------------------------
class CAcctCacheItem;
typedef SFArrayBase<CAcctCacheItem>         CAcctCacheItemArray;
typedef SFList<CAcctCacheItem*>             CAcctCacheItemList;
typedef SFUniqueList<CAcctCacheItem*>       CAcctCacheItemListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CAcctCacheItem : public CBaseNode {
public:
    SFUint32 blockNum;
    SFUint32 transIndex;
    int32_t which;

public:
    CAcctCacheItem(void);
    CAcctCacheItem(const CAcctCacheItem& ac);
    ~CAcctCacheItem(void);
    CAcctCacheItem& operator=(const CAcctCacheItem& ac);

    DECLARE_NODE(CAcctCacheItem);

    // EXISTING_CODE
    CAcctCacheItem(SFUint32 b, SFUint32 t, int32_t w) : blockNum(b), transIndex(t), which(w) {}
    bool operator==(const CAcctCacheItem& item) {
        return (blockNum == item.blockNum && transIndex == item.transIndex); // && which == item.which);
    }
    bool operator!=(const CAcctCacheItem& item) { return !operator==(item); }
    friend ostream& operator<<(ostream& os, const CAcctCacheItem& cacheItem);
    // EXISTING_CODE

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CAcctCacheItem& ac);
    bool readBackLevel(SFArchive& archive);

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAcctCacheItem::CAcctCacheItem(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAcctCacheItem::CAcctCacheItem(const CAcctCacheItem& ac) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(ac);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAcctCacheItem::~CAcctCacheItem(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAcctCacheItem::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAcctCacheItem::Init(void) {
    CBaseNode::Init();

    blockNum = 0;
    transIndex = 0;
    which = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAcctCacheItem::Copy(const CAcctCacheItem& ac) {
    Clear();
    CBaseNode::Copy(ac);

    blockNum = ac.blockNum;
    transIndex = ac.transIndex;
    which = ac.which;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CAcctCacheItem& CAcctCacheItem::operator=(const CAcctCacheItem& ac) {
    Copy(ac);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
inline SFString CAcctCacheItem::getValueByName(const SFString& fieldName) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return Format("[{"+toUpper(fieldName)+"}]");
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CAcctCacheItemArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CAcctCacheItemArray);
IMPLEMENT_ARCHIVE_LIST(CAcctCacheItemList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

