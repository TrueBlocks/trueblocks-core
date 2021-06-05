#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
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
 * the code inside of 'EXISTING_CODE' tags.
 */
#include "etherlib.h"
#include "transaction.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class QTransferFrom : public CTransaction {
  public:
    address_t _from;
    address_t _to;
    biguint_t _value;

  public:
    QTransferFrom(void);
    QTransferFrom(const QTransferFrom& tr);
    virtual ~QTransferFrom(void);
    QTransferFrom& operator=(const QTransferFrom& tr);

    DECLARE_NODE(QTransferFrom);

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const QTransferFrom& it) const;
    bool operator!=(const QTransferFrom& it) const {
        return !operator==(it);
    }
    friend bool operator<(const QTransferFrom& v1, const QTransferFrom& v2);
    friend ostream& operator<<(ostream& os, const QTransferFrom& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const QTransferFrom& tr);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline QTransferFrom::QTransferFrom(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QTransferFrom::QTransferFrom(const QTransferFrom& tr) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(tr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline QTransferFrom::~QTransferFrom(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QTransferFrom::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QTransferFrom::initialize(void) {
    CTransaction::initialize();

    _from = "";
    _to = "";
    _value = 0;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void QTransferFrom::duplicate(const QTransferFrom& tr) {
    clear();
    CTransaction::duplicate(tr);

    _from = tr._from;
    _to = tr._to;
    _value = tr._value;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline QTransferFrom& QTransferFrom::operator=(const QTransferFrom& tr) {
    duplicate(tr);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool QTransferFrom::operator==(const QTransferFrom& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const QTransferFrom& v1, const QTransferFrom& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<QTransferFrom> QTransferFromArray;
extern CArchive& operator>>(CArchive& archive, QTransferFromArray& array);
extern CArchive& operator<<(CArchive& archive, const QTransferFromArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_TRANSFERFROM;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
