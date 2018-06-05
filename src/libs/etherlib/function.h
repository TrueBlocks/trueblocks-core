#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
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
#include "utillib.h"
#include "parameter.h"

namespace qblocks {

//--------------------------------------------------------------------------
class CFunction;
typedef SFArrayBase<CFunction>         CFunctionArray;
typedef SFList<CFunction*>             CFunctionList;
typedef SFUniqueList<CFunction*>       CFunctionListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CFunction : public CBaseNode {
public:
    SFString name;
    SFString type;
    bool anonymous;
    bool constant;
    bool payable;
    SFString signature;
    SFString encoding;
    CParameterArray inputs;
    CParameterArray outputs;

public:
    CFunction(void);
    CFunction(const CFunction& fu);
    virtual ~CFunction(void);
    CFunction& operator=(const CFunction& fu);

    DECLARE_NODE(CFunction);

    const CBaseNode *getObjectAt(const SFString& fieldName, uint32_t index) const override;

    // EXISTING_CODE
    bool hasAddrs;
    SFString getSignature(uint64_t parts) const;
    SFString encodeItem(void) const;
    bool isBuiltin;
    SFString origName;
    // EXISTING_CODE
    friend ostream& operator<<(ostream& os, const CFunction& item);

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CFunction& fu);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CFunction::CFunction(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CFunction::CFunction(const CFunction& fu) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(fu);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CFunction::~CFunction(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CFunction::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CFunction::Init(void) {
    CBaseNode::Init();

    name = "";
    type = "";
    anonymous = 0;
    constant = 0;
    payable = 0;
    signature = "";
    encoding = "";
    inputs.Clear();
    outputs.Clear();

    // EXISTING_CODE
    hasAddrs = false;
    isBuiltin = false;
    origName = "";
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CFunction::Copy(const CFunction& fu) {
    Clear();
    CBaseNode::Copy(fu);

    name = fu.name;
    type = fu.type;
    anonymous = fu.anonymous;
    constant = fu.constant;
    payable = fu.payable;
    signature = fu.signature;
    encoding = fu.encoding;
    inputs = fu.inputs;
    outputs = fu.outputs;

    // EXISTING_CODE
    hasAddrs = fu.hasAddrs;
    isBuiltin = fu.isBuiltin;
    origName = fu.origName;
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CFunction& CFunction::operator=(const CFunction& fu) {
    Copy(fu);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CFunctionArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CFunctionArray);
IMPLEMENT_ARCHIVE_LIST(CFunctionList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

