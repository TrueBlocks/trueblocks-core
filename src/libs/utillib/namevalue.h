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
/*
 * This file was generated with makeClass. Edit only those parts of the code inside
 * of 'EXISTING_CODE' tags.
 */
#include "basetypes.h"
#include "basenode.h"
#include "conversions.h"
#include "sfarchive.h"

namespace qblocks {

//--------------------------------------------------------------------------
class CNameValue;
typedef SFArrayBase<CNameValue>         CNameValueArray;
typedef SFList<CNameValue*>             CNameValueList;
typedef SFUniqueList<CNameValue*>       CNameValueListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CNameValue : public CBaseNode {
public:
    SFString name;
    SFString value;

public:
    CNameValue(void);
    CNameValue(const CNameValue& na);
    virtual ~CNameValue(void);
    CNameValue& operator=(const CNameValue& na);

    DECLARE_NODE(CNameValue);

    // EXISTING_CODE
    SFString getValue  (void) const { return            value;  }
    uint64_t getValueU (void) const { return toUnsigned(value); }
    int64_t  getValueI (void) const { return toLong    (value); }
    SFUintBN getValueBN(void) const { return toWei     (value); }
    SFString getName   (void) const { return            name;   }
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const CNameValue& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CNameValue& na);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CNameValue::CNameValue(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CNameValue::CNameValue(const CNameValue& na) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(na);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CNameValue::~CNameValue(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNameValue::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNameValue::Init(void) {
    CBaseNode::Init();

    name = "";
    value = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CNameValue::Copy(const CNameValue& na) {
    Clear();
    CBaseNode::Copy(na);

    name = na.name;
    value = na.value;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CNameValue& CNameValue::operator=(const CNameValue& na) {
    Copy(na);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CNameValueArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CNameValueArray);
IMPLEMENT_ARCHIVE_LIST(CNameValueList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

