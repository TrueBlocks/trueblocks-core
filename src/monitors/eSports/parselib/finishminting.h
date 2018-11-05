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
class CFinishMinting : public CTransaction {
public:
    CFinishMinting(void);
    CFinishMinting(const CFinishMinting& fi);
    virtual ~CFinishMinting(void);
    CFinishMinting& operator=(const CFinishMinting& fi);

    DECLARE_NODE(CFinishMinting);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CFinishMinting& item) const;
    bool operator!=(const CFinishMinting& item) const { return !operator==(item); }
    friend bool operator<(const CFinishMinting& v1, const CFinishMinting& v2);
    friend ostream& operator<<(ostream& os, const CFinishMinting& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CFinishMinting& fi);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CFinishMinting::CFinishMinting(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CFinishMinting::CFinishMinting(const CFinishMinting& fi) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(fi);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CFinishMinting::~CFinishMinting(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CFinishMinting::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CFinishMinting::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CFinishMinting::duplicate(const CFinishMinting& fi) {
    clear();
    CTransaction::duplicate(fi);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CFinishMinting& CFinishMinting::operator=(const CFinishMinting& fi) {
    duplicate(fi);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CFinishMinting::operator==(const CFinishMinting& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CFinishMinting& v1, const CFinishMinting& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CFinishMinting> CFinishMintingArray;
extern CArchive& operator>>(CArchive& archive, CFinishMintingArray& array);
extern CArchive& operator<<(CArchive& archive, const CFinishMintingArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

