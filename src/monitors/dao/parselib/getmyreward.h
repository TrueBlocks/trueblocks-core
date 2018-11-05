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
class CGetMyReward : public CTransaction {
public:
    CGetMyReward(void);
    CGetMyReward(const CGetMyReward& ge);
    virtual ~CGetMyReward(void);
    CGetMyReward& operator=(const CGetMyReward& ge);

    DECLARE_NODE(CGetMyReward);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CGetMyReward& item) const;
    bool operator!=(const CGetMyReward& item) const { return !operator==(item); }
    friend bool operator<(const CGetMyReward& v1, const CGetMyReward& v2);
    friend ostream& operator<<(ostream& os, const CGetMyReward& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CGetMyReward& ge);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CGetMyReward::CGetMyReward(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CGetMyReward::CGetMyReward(const CGetMyReward& ge) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ge);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CGetMyReward::~CGetMyReward(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CGetMyReward::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CGetMyReward::initialize(void) {
    CTransaction::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CGetMyReward::duplicate(const CGetMyReward& ge) {
    clear();
    CTransaction::duplicate(ge);

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CGetMyReward& CGetMyReward::operator=(const CGetMyReward& ge) {
    duplicate(ge);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CGetMyReward::operator==(const CGetMyReward& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CGetMyReward& v1, const CGetMyReward& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CGetMyReward> CGetMyRewardArray;
extern CArchive& operator>>(CArchive& archive, CGetMyRewardArray& array);
extern CArchive& operator<<(CArchive& archive, const CGetMyRewardArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

