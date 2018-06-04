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
#include "sfarchive.h"

namespace qblocks {

//--------------------------------------------------------------------------
class CAccountName;
typedef SFArrayBase<CAccountName>         CAccountNameArray;
typedef SFList<CAccountName*>             CAccountNameList;
typedef SFUniqueList<CAccountName*>       CAccountNameListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CAccountName : public CBaseNode {
public:
    SFString symbol;
    SFString name;
    SFString addr;
    SFString source;
    SFString description;

public:
    CAccountName(void);
    CAccountName(const CAccountName& ac);
    virtual ~CAccountName(void);
    CAccountName& operator=(const CAccountName& ac);

    DECLARE_NODE(CAccountName);

    // EXISTING_CODE
    explicit CAccountName(SFString& nameIn);
    bool Match(const SFString& s1, const SFString& s2, const SFString& s3, bool matchCase, bool all) const;
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const CAccountName& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CAccountName& ac);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAccountName::CAccountName(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAccountName::CAccountName(const CAccountName& ac) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(ac);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAccountName::~CAccountName(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccountName::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccountName::Init(void) {
    CBaseNode::Init();

    symbol = "";
    name = "";
    addr = "";
    source = "";
    description = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccountName::Copy(const CAccountName& ac) {
    Clear();
    CBaseNode::Copy(ac);

    symbol = ac.symbol;
    name = ac.name;
    addr = ac.addr;
    source = ac.source;
    description = ac.description;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CAccountName& CAccountName::operator=(const CAccountName& ac) {
    Copy(ac);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CAccountNameArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CAccountNameArray);
IMPLEMENT_ARCHIVE_LIST(CAccountNameList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

