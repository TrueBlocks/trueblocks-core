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
/*
 * This file was generated with makeClass. Edit only those parts of the code inside
 * of 'EXISTING_CODE' tags.
 */
#include "etherlib.h"

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CBlockIndexItem : public CBaseNode {
public:
    uint32_t bn;
    uint32_t ts;
    uint32_t cnt;

public:
    CBlockIndexItem(void);
    CBlockIndexItem(const CBlockIndexItem& bl);
    virtual ~CBlockIndexItem(void);
    CBlockIndexItem& operator=(const CBlockIndexItem& bl);

    DECLARE_NODE(CBlockIndexItem);

    // EXISTING_CODE
    CBlockIndexItem(blknum_t b, timestamp_t t, size_t c) : bn((uint32_t)b), ts((uint32_t)t), cnt((uint32_t)c) {}
    static size_t sizeOnDisc(void) { return sizeof(uint32_t)*3; }
    // EXISTING_CODE
    bool operator==(const CBlockIndexItem& item) const;
    bool operator!=(const CBlockIndexItem& item) const { return !operator==(item); }
    friend bool operator<(const CBlockIndexItem& v1, const CBlockIndexItem& v2);
    friend ostream& operator<<(ostream& os, const CBlockIndexItem& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CBlockIndexItem& bl);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CBlockIndexItem::CBlockIndexItem(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CBlockIndexItem::CBlockIndexItem(const CBlockIndexItem& bl) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(bl);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CBlockIndexItem::~CBlockIndexItem(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBlockIndexItem::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBlockIndexItem::initialize(void) {
    CBaseNode::initialize();

    bn = 0;
    ts = 0;
    cnt = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBlockIndexItem::duplicate(const CBlockIndexItem& bl) {
    clear();
    CBaseNode::duplicate(bl);

    bn = bl.bn;
    ts = bl.ts;
    cnt = bl.cnt;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CBlockIndexItem& CBlockIndexItem::operator=(const CBlockIndexItem& bl) {
    duplicate(bl);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CBlockIndexItem::operator==(const CBlockIndexItem& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CBlockIndexItem& v1, const CBlockIndexItem& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CBlockIndexItem> CBlockIndexItemArray;
extern CArchive& operator>>(CArchive& archive, CBlockIndexItemArray& array);
extern CArchive& operator<<(CArchive& archive, const CBlockIndexItemArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

