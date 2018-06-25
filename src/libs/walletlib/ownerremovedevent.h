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
#include "logentry.h"

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QOwnerRemovedEvent : public CLogEntry {
public:
    SFAddress oldOwner;

public:
    QOwnerRemovedEvent(void);
    QOwnerRemovedEvent(const QOwnerRemovedEvent& ow);
    virtual ~QOwnerRemovedEvent(void);
    QOwnerRemovedEvent& operator=(const QOwnerRemovedEvent& ow);

    DECLARE_NODE(QOwnerRemovedEvent);

    // EXISTING_CODE
    // EXISTING_CODE
    friend bool operator<(const QOwnerRemovedEvent& v1, const QOwnerRemovedEvent& v2);
    friend ostream& operator<<(ostream& os, const QOwnerRemovedEvent& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const QOwnerRemovedEvent& ow);
    bool readBackLevel(SFArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QOwnerRemovedEvent::QOwnerRemovedEvent(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QOwnerRemovedEvent::QOwnerRemovedEvent(const QOwnerRemovedEvent& ow) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(ow);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QOwnerRemovedEvent::~QOwnerRemovedEvent(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QOwnerRemovedEvent::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QOwnerRemovedEvent::initialize(void) {
    CLogEntry::initialize();

    oldOwner = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QOwnerRemovedEvent::duplicate(const QOwnerRemovedEvent& ow) {
    clear();
    CLogEntry::duplicate(ow);

    oldOwner = ow.oldOwner;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline QOwnerRemovedEvent& QOwnerRemovedEvent::operator=(const QOwnerRemovedEvent& ow) {
    duplicate(ow);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool operator<(const QOwnerRemovedEvent& v1, const QOwnerRemovedEvent& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted
    return true;
}

//---------------------------------------------------------------------------
typedef vector<QOwnerRemovedEvent> QOwnerRemovedEventArray;
extern SFArchive& operator>>(SFArchive& archive, QOwnerRemovedEventArray& array);
extern SFArchive& operator<<(SFArchive& archive, const QOwnerRemovedEventArray& array);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

