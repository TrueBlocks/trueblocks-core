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
class CStopToken : public CTransaction {
public:
    CStopToken(void);
    CStopToken(const CStopToken& st);
    virtual ~CStopToken(void);
    CStopToken& operator=(const CStopToken& st);

    DECLARE_NODE(CStopToken);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CStopToken& item) const;
    bool operator!=(const CStopToken& item) const { return !operator==(item); }
    friend bool operator<(const CStopToken& v1, const CStopToken& v2);
    friend ostream& operator<<(ostream& os, const CStopToken& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CStopToken& st);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CStopToken::CStopToken(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CStopToken::CStopToken(const CStopToken& st) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(st);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CStopToken::~CStopToken(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CStopToken::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CStopToken::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CStopToken::duplicate(const CStopToken& st) {
    clear();
    CTransaction::duplicate(st);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CStopToken& CStopToken::operator=(const CStopToken& st) {
    duplicate(st);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CStopToken::operator==(const CStopToken& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CStopToken& v1, const CStopToken& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CStopToken> CStopTokenArray;
extern CArchive& operator>>(CArchive& archive, CStopTokenArray& array);
extern CArchive& operator<<(CArchive& archive, const CStopTokenArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

