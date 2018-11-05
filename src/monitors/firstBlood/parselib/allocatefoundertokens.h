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
#include <vector>
#include <map>
#include "transaction.h"

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CAllocateFounderTokens : public CTransaction {
public:
    CAllocateFounderTokens(void);
    CAllocateFounderTokens(const CAllocateFounderTokens& al);
    virtual ~CAllocateFounderTokens(void);
    CAllocateFounderTokens& operator=(const CAllocateFounderTokens& al);

    DECLARE_NODE(CAllocateFounderTokens);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CAllocateFounderTokens& item) const;
    bool operator!=(const CAllocateFounderTokens& item) const { return !operator==(item); }
    friend bool operator<(const CAllocateFounderTokens& v1, const CAllocateFounderTokens& v2);
    friend ostream& operator<<(ostream& os, const CAllocateFounderTokens& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CAllocateFounderTokens& al);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAllocateFounderTokens::CAllocateFounderTokens(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAllocateFounderTokens::CAllocateFounderTokens(const CAllocateFounderTokens& al) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(al);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAllocateFounderTokens::~CAllocateFounderTokens(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAllocateFounderTokens::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAllocateFounderTokens::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAllocateFounderTokens::duplicate(const CAllocateFounderTokens& al) {
    clear();
    CTransaction::duplicate(al);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CAllocateFounderTokens& CAllocateFounderTokens::operator=(const CAllocateFounderTokens& al) {
    duplicate(al);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CAllocateFounderTokens::operator==(const CAllocateFounderTokens& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CAllocateFounderTokens& v1, const CAllocateFounderTokens& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CAllocateFounderTokens> CAllocateFounderTokensArray;
extern CArchive& operator>>(CArchive& archive, CAllocateFounderTokensArray& array);
extern CArchive& operator<<(CArchive& archive, const CAllocateFounderTokensArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

