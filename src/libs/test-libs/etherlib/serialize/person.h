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
#include "etherlib.h"

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CPerson : public CBaseNode {
public:
    string_q name;
    uint64_t age;
    CPerson *next;

public:
    CPerson(void);
    CPerson(const CPerson& pe);
    virtual ~CPerson(void);
    CPerson& operator=(const CPerson& pe);

    DECLARE_NODE(CPerson);

    // EXISTING_CODE
    CPerson(const string_q& n, uint64_t a) : name(n), age(a), next(NULL) { }
    // EXISTING_CODE
    bool operator==(const CPerson& item) const;
    bool operator!=(const CPerson& item) const { return !operator==(item); }
    friend bool operator<(const CPerson& v1, const CPerson& v2);
    friend ostream& operator<<(ostream& os, const CPerson& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CPerson& pe);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CPerson::CPerson(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPerson::CPerson(const CPerson& pe) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(pe);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CPerson::~CPerson(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPerson::clear(void) {
    if (next)
        delete next;
    next = NULL;
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPerson::initialize(void) {
    CBaseNode::initialize();

    name = "";
    age = 0;
    next = NULL;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPerson::duplicate(const CPerson& pe) {
    clear();
    CBaseNode::duplicate(pe);

    name = pe.name;
    age = pe.age;
    if (pe.next) {
        next = new CPerson;
        *next = *pe.next;
    }

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPerson& CPerson::operator=(const CPerson& pe) {
    duplicate(pe);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CPerson::operator==(const CPerson& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CPerson& v1, const CPerson& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CPerson> CPersonArray;
extern CArchive& operator>>(CArchive& archive, CPersonArray& array);
extern CArchive& operator<<(CArchive& archive, const CPersonArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_PERSON;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

