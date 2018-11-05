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
class CRetrieveDAOReward : public CTransaction {
public:
    bool _toMembers;

public:
    CRetrieveDAOReward(void);
    CRetrieveDAOReward(const CRetrieveDAOReward& re);
    virtual ~CRetrieveDAOReward(void);
    CRetrieveDAOReward& operator=(const CRetrieveDAOReward& re);

    DECLARE_NODE(CRetrieveDAOReward);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CRetrieveDAOReward& item) const;
    bool operator!=(const CRetrieveDAOReward& item) const { return !operator==(item); }
    friend bool operator<(const CRetrieveDAOReward& v1, const CRetrieveDAOReward& v2);
    friend ostream& operator<<(ostream& os, const CRetrieveDAOReward& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CRetrieveDAOReward& re);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CRetrieveDAOReward::CRetrieveDAOReward(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CRetrieveDAOReward::CRetrieveDAOReward(const CRetrieveDAOReward& re) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CRetrieveDAOReward::~CRetrieveDAOReward(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRetrieveDAOReward::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRetrieveDAOReward::initialize(void) {
    CTransaction::initialize();

    _toMembers = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRetrieveDAOReward::duplicate(const CRetrieveDAOReward& re) {
    clear();
    CTransaction::duplicate(re);

    _toMembers = re._toMembers;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CRetrieveDAOReward& CRetrieveDAOReward::operator=(const CRetrieveDAOReward& re) {
    duplicate(re);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CRetrieveDAOReward::operator==(const CRetrieveDAOReward& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CRetrieveDAOReward& v1, const CRetrieveDAOReward& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CRetrieveDAOReward> CRetrieveDAORewardArray;
extern CArchive& operator>>(CArchive& archive, CRetrieveDAORewardArray& array);
extern CArchive& operator<<(CArchive& archive, const CRetrieveDAORewardArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

