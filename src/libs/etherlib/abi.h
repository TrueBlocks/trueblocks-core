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
#include "function.h"
#include "parameter.h"

namespace qblocks {

// EXISTING_CODE
class CTransaction;
class CLogEntry;
class CTrace;
// EXISTING_CODE

//--------------------------------------------------------------------------
class CAbi : public CBaseNode {
public:
    address_t address;
    CFunctionArray interfaces;

public:
    CAbi(void);
    CAbi(const CAbi& ab);
    virtual ~CAbi(void);
    CAbi& operator=(const CAbi& ab);

    DECLARE_NODE(CAbi);

    const CBaseNode *getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    bool addIfUnique(const string_q& addr, CFunction& func, bool decorateNames);
    bool loadAbiKnown(const string_q& which);
    bool loadCachedAbis(const string_q& which);
    bool loadAbiByAddress(address_t addr);
    bool loadAbiFromFile(const string_q& fileName, bool builtIn);
    bool loadAbiFromString(const string_q& str, bool builtIn);
    bool articulateTransaction(CTransaction *p) const;
    bool articulateLog(CLogEntry *l) const;
    bool articulateTrace(CTrace *t) const;
    bool articulateOutputs(const string_q& encoding, const string_q& value, CFunction& ret) const;
    friend class CAccountWatch;
    // EXISTING_CODE
    bool operator==(const CAbi& item) const;
    bool operator!=(const CAbi& item) const { return !operator==(item); }
    friend bool operator<(const CAbi& v1, const CAbi& v2);
    friend ostream& operator<<(ostream& os, const CAbi& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CAbi& ab);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAbi::CAbi(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAbi::CAbi(const CAbi& ab) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ab);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAbi::~CAbi(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAbi::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAbi::initialize(void) {
    CBaseNode::initialize();

    address = "";
    interfaces.clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAbi::duplicate(const CAbi& ab) {
    clear();
    CBaseNode::duplicate(ab);

    address = ab.address;
    interfaces = ab.interfaces;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAbi& CAbi::operator=(const CAbi& ab) {
    duplicate(ab);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CAbi::operator==(const CAbi& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CAbi& v1, const CAbi& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CAbi> CAbiArray;
extern CArchive& operator>>(CArchive& archive, CAbiArray& array);
extern CArchive& operator<<(CArchive& archive, const CAbiArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CAbi& abi);
extern CArchive& operator>>(CArchive& archive, CAbi& abi);

//---------------------------------------------------------------------------
// EXISTING_CODE
extern bool decodeRLP(CParameterArray& interfaces, const string_q& desc, const string_q& input);
// EXISTING_CODE
}  // namespace qblocks

