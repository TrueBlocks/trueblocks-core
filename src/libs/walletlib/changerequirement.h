#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
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
#include "transaction.h"

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QChangeRequirement : public CTransaction {
public:
    SFUintBN _newRequired;

public:
    QChangeRequirement(void);
    QChangeRequirement(const QChangeRequirement& ch);
    virtual ~QChangeRequirement(void);
    QChangeRequirement& operator=(const QChangeRequirement& ch);

    DECLARE_NODE(QChangeRequirement);

    // EXISTING_CODE
    // EXISTING_CODE
    friend bool operator<(const QChangeRequirement& v1, const QChangeRequirement& v2);
    friend ostream& operator<<(ostream& os, const QChangeRequirement& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const QChangeRequirement& ch);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QChangeRequirement::QChangeRequirement(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QChangeRequirement::QChangeRequirement(const QChangeRequirement& ch) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ch);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QChangeRequirement::~QChangeRequirement(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QChangeRequirement::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QChangeRequirement::initialize(void) {
    CTransaction::initialize();

    _newRequired = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QChangeRequirement::duplicate(const QChangeRequirement& ch) {
    clear();
    CTransaction::duplicate(ch);

    _newRequired = ch._newRequired;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QChangeRequirement& QChangeRequirement::operator=(const QChangeRequirement& ch) {
    duplicate(ch);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool operator<(const QChangeRequirement& v1, const QChangeRequirement& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted
    return true;
}

//---------------------------------------------------------------------------
typedef vector<QChangeRequirement> QChangeRequirementArray;
extern SFArchive& operator>>(SFArchive& archive, QChangeRequirementArray& array);
extern SFArchive& operator<<(SFArchive& archive, const QChangeRequirementArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

