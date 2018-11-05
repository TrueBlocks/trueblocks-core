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
class CChangeAllowedRecipients : public CTransaction {
public:
    address_t _recipient;
    bool _allowed;

public:
    CChangeAllowedRecipients(void);
    CChangeAllowedRecipients(const CChangeAllowedRecipients& ch);
    virtual ~CChangeAllowedRecipients(void);
    CChangeAllowedRecipients& operator=(const CChangeAllowedRecipients& ch);

    DECLARE_NODE(CChangeAllowedRecipients);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CChangeAllowedRecipients& item) const;
    bool operator!=(const CChangeAllowedRecipients& item) const { return !operator==(item); }
    friend bool operator<(const CChangeAllowedRecipients& v1, const CChangeAllowedRecipients& v2);
    friend ostream& operator<<(ostream& os, const CChangeAllowedRecipients& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CChangeAllowedRecipients& ch);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CChangeAllowedRecipients::CChangeAllowedRecipients(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CChangeAllowedRecipients::CChangeAllowedRecipients(const CChangeAllowedRecipients& ch) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ch);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CChangeAllowedRecipients::~CChangeAllowedRecipients(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeAllowedRecipients::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeAllowedRecipients::initialize(void) {
    CTransaction::initialize();

    _recipient = "";
    _allowed = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CChangeAllowedRecipients::duplicate(const CChangeAllowedRecipients& ch) {
    clear();
    CTransaction::duplicate(ch);

    _recipient = ch._recipient;
    _allowed = ch._allowed;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CChangeAllowedRecipients& CChangeAllowedRecipients::operator=(const CChangeAllowedRecipients& ch) {
    duplicate(ch);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CChangeAllowedRecipients::operator==(const CChangeAllowedRecipients& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CChangeAllowedRecipients& v1, const CChangeAllowedRecipients& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CChangeAllowedRecipients> CChangeAllowedRecipientsArray;
extern CArchive& operator>>(CArchive& archive, CChangeAllowedRecipientsArray& array);
extern CArchive& operator<<(CArchive& archive, const CChangeAllowedRecipientsArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

