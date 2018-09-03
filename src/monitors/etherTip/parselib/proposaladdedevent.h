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
#include "logentry.h"

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CProposalAddedEvent : public CLogEntry {
public:
    biguint_t proposalID;
    address_t recipient;
    biguint_t amount;
    string_q description;

public:
    CProposalAddedEvent(void);
    CProposalAddedEvent(const CProposalAddedEvent& pr);
    virtual ~CProposalAddedEvent(void);
    CProposalAddedEvent& operator=(const CProposalAddedEvent& pr);

    DECLARE_NODE(CProposalAddedEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const CProposalAddedEvent& item) const;
    bool operator!=(const CProposalAddedEvent& item) const { return !operator==(item); }
    friend bool operator<(const CProposalAddedEvent& v1, const CProposalAddedEvent& v2);
    friend ostream& operator<<(ostream& os, const CProposalAddedEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CProposalAddedEvent& pr);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CProposalAddedEvent::CProposalAddedEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CProposalAddedEvent::CProposalAddedEvent(const CProposalAddedEvent& pr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(pr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CProposalAddedEvent::~CProposalAddedEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CProposalAddedEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CProposalAddedEvent::initialize(void) {
    CLogEntry::initialize();

    proposalID = 0;
    recipient = "";
    amount = 0;
    description = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CProposalAddedEvent::duplicate(const CProposalAddedEvent& pr) {
    clear();
    CLogEntry::duplicate(pr);

    proposalID = pr.proposalID;
    recipient = pr.recipient;
    amount = pr.amount;
    description = pr.description;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CProposalAddedEvent& CProposalAddedEvent::operator=(const CProposalAddedEvent& pr) {
    duplicate(pr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CProposalAddedEvent::operator==(const CProposalAddedEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CProposalAddedEvent& v1, const CProposalAddedEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CProposalAddedEvent> CProposalAddedEventArray;
extern CArchive& operator>>(CArchive& archive, CProposalAddedEventArray& array);
extern CArchive& operator<<(CArchive& archive, const CProposalAddedEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

