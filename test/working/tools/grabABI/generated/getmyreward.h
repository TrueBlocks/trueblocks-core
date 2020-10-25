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
 * This file was generated with makeClass. Edit only those parts of the code inside
 * of 'EXISTING_CODE' tags.
 */

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class  : public CBaseNode {
  public:
    (void);
    (const & );
    ~(void);
    & operator=(const & );

    DECLARE_NODE();

    // EXISTING_CODE
    // EXISTING_CODE
    bool operator==(const & it) const;
    bool operator!=(const & it) const {
        return !operator==(it);
    }
    friend bool operator<(const & v1, const & v2);
    friend ostream& operator<<(ostream& os, const & it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const & );
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline ::(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline ::(const & ) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate();
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline ::~(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void ::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void ::initialize(void) {
    CBaseNode::initialize();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void ::duplicate(const & ) {
    clear();
    CBaseNode::duplicate();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline & ::operator=(const & ) {
    duplicate();
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool ::operator==(const & it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const & v1, const & v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<> Array;
extern CArchive& operator>>(CArchive& archive, Array& array);
extern CArchive& operator<<(CArchive& archive, const Array& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
