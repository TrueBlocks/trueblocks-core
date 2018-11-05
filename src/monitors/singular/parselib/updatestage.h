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
class CUpdateStage : public CTransaction {
public:
    CUpdateStage(void);
    CUpdateStage(const CUpdateStage& up);
    virtual ~CUpdateStage(void);
    CUpdateStage& operator=(const CUpdateStage& up);

    DECLARE_NODE(CUpdateStage);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CUpdateStage& item) const;
    bool operator!=(const CUpdateStage& item) const { return !operator==(item); }
    friend bool operator<(const CUpdateStage& v1, const CUpdateStage& v2);
    friend ostream& operator<<(ostream& os, const CUpdateStage& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CUpdateStage& up);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CUpdateStage::CUpdateStage(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CUpdateStage::CUpdateStage(const CUpdateStage& up) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(up);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CUpdateStage::~CUpdateStage(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CUpdateStage::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CUpdateStage::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CUpdateStage::duplicate(const CUpdateStage& up) {
    clear();
    CTransaction::duplicate(up);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CUpdateStage& CUpdateStage::operator=(const CUpdateStage& up) {
    duplicate(up);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CUpdateStage::operator==(const CUpdateStage& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CUpdateStage& v1, const CUpdateStage& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CUpdateStage> CUpdateStageArray;
extern CArchive& operator>>(CArchive& archive, CUpdateStageArray& array);
extern CArchive& operator<<(CArchive& archive, const CUpdateStageArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

