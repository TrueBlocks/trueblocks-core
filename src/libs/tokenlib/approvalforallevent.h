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
#include "logentry_ex.h"

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QApprovalForAllEvent : public CLogEntry_Ex {
public:
    address_t _owner;
    address_t _operator;
    bool _approved;

public:
    QApprovalForAllEvent(void);
    QApprovalForAllEvent(const QApprovalForAllEvent& ap);
    virtual ~QApprovalForAllEvent(void);
    QApprovalForAllEvent& operator=(const QApprovalForAllEvent& ap);

    DECLARE_NODE(QApprovalForAllEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const QApprovalForAllEvent& item) const;
    bool operator!=(const QApprovalForAllEvent& item) const { return !operator==(item); }
    friend bool operator<(const QApprovalForAllEvent& v1, const QApprovalForAllEvent& v2);
    friend ostream& operator<<(ostream& os, const QApprovalForAllEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const QApprovalForAllEvent& ap);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QApprovalForAllEvent::QApprovalForAllEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QApprovalForAllEvent::QApprovalForAllEvent(const QApprovalForAllEvent& ap) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ap);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QApprovalForAllEvent::~QApprovalForAllEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QApprovalForAllEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QApprovalForAllEvent::initialize(void) {
    CLogEntry_Ex::initialize();

    _owner = "";
    _operator = "";
    _approved = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QApprovalForAllEvent::duplicate(const QApprovalForAllEvent& ap) {
    clear();
    CLogEntry_Ex::duplicate(ap);

    _owner = ap._owner;
    _operator = ap._operator;
    _approved = ap._approved;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QApprovalForAllEvent& QApprovalForAllEvent::operator=(const QApprovalForAllEvent& ap) {
    duplicate(ap);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool QApprovalForAllEvent::operator==(const QApprovalForAllEvent& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const QApprovalForAllEvent& v1, const QApprovalForAllEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<QApprovalForAllEvent> QApprovalForAllEventArray;
extern CArchive& operator>>(CArchive& archive, QApprovalForAllEventArray& array);
extern CArchive& operator<<(CArchive& archive, const QApprovalForAllEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

