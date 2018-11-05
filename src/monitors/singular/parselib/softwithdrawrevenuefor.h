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
class CSoftWithdrawRevenueFor : public CTransaction {
public:
    address_t forAddress;

public:
    CSoftWithdrawRevenueFor(void);
    CSoftWithdrawRevenueFor(const CSoftWithdrawRevenueFor& so);
    virtual ~CSoftWithdrawRevenueFor(void);
    CSoftWithdrawRevenueFor& operator=(const CSoftWithdrawRevenueFor& so);

    DECLARE_NODE(CSoftWithdrawRevenueFor);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CSoftWithdrawRevenueFor& item) const;
    bool operator!=(const CSoftWithdrawRevenueFor& item) const { return !operator==(item); }
    friend bool operator<(const CSoftWithdrawRevenueFor& v1, const CSoftWithdrawRevenueFor& v2);
    friend ostream& operator<<(ostream& os, const CSoftWithdrawRevenueFor& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CSoftWithdrawRevenueFor& so);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CSoftWithdrawRevenueFor::CSoftWithdrawRevenueFor(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CSoftWithdrawRevenueFor::CSoftWithdrawRevenueFor(const CSoftWithdrawRevenueFor& so) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(so);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CSoftWithdrawRevenueFor::~CSoftWithdrawRevenueFor(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSoftWithdrawRevenueFor::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSoftWithdrawRevenueFor::initialize(void) {
    CTransaction::initialize();

    forAddress = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CSoftWithdrawRevenueFor::duplicate(const CSoftWithdrawRevenueFor& so) {
    clear();
    CTransaction::duplicate(so);

    forAddress = so.forAddress;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CSoftWithdrawRevenueFor& CSoftWithdrawRevenueFor::operator=(const CSoftWithdrawRevenueFor& so) {
    duplicate(so);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CSoftWithdrawRevenueFor::operator==(const CSoftWithdrawRevenueFor& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CSoftWithdrawRevenueFor& v1, const CSoftWithdrawRevenueFor& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CSoftWithdrawRevenueFor> CSoftWithdrawRevenueForArray;
extern CArchive& operator>>(CArchive& archive, CSoftWithdrawRevenueForArray& array);
extern CArchive& operator<<(CArchive& archive, const CSoftWithdrawRevenueForArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

