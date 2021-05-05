#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
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
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 *  the code inside of 'EXISTING_CODE' tags.
 */
#include "etherlib.h"
#include "transferfrom.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QFromTransferFrom : public QTransferFrom {
  public:
    address_t whop;
    address_t werp;

  public:
    QFromTransferFrom(void);
    QFromTransferFrom(const QFromTransferFrom& fr);
    virtual ~QFromTransferFrom(void);
    QFromTransferFrom& operator=(const QFromTransferFrom& fr);

    DECLARE_NODE(QFromTransferFrom);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const QFromTransferFrom& it) const;
    bool operator!=(const QFromTransferFrom& it) const {
        return !operator==(it);
    }
    friend bool operator<(const QFromTransferFrom& v1, const QFromTransferFrom& v2);
    friend ostream& operator<<(ostream& os, const QFromTransferFrom& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const QFromTransferFrom& fr);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QFromTransferFrom::QFromTransferFrom(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QFromTransferFrom::QFromTransferFrom(const QFromTransferFrom& fr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(fr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QFromTransferFrom::~QFromTransferFrom(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QFromTransferFrom::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QFromTransferFrom::initialize(void) {
    QTransferFrom::initialize();

    whop = "";
    werp = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QFromTransferFrom::duplicate(const QFromTransferFrom& fr) {
    clear();
    QTransferFrom::duplicate(fr);

    whop = fr.whop;
    werp = fr.werp;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QFromTransferFrom& QFromTransferFrom::operator=(const QFromTransferFrom& fr) {
    duplicate(fr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool QFromTransferFrom::operator==(const QFromTransferFrom& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const QFromTransferFrom& v1, const QFromTransferFrom& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<QFromTransferFrom> QFromTransferFromArray;
extern CArchive& operator>>(CArchive& archive, QFromTransferFromArray& array);
extern CArchive& operator<<(CArchive& archive, const QFromTransferFromArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_FROMTRANSFERFROM;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
