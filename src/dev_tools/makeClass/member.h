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
#include "utillib.h"

namespace qblocks {

// EXISTING_CODE
//--------------------------------------------------------------------------
// Signature parts
#define SIG_FTYPE (1 << 1)
#define SIG_FNAME (1 << 2)
#define SIG_FSPACE (1 << 3)
#define SIG_ITYPE (1 << 4)
#define SIG_INAME (1 << 5)
#define SIG_IINDEXED (1 << 6)
#define SIG_ENCODE (1 << 7)
#define SIG_CONST (1 << 8)
#define SIG_ANONYMOUS (1 << 9)
#define SIG_PAYABLE (1 << 10)
#define SIG_CANONICAL (SIG_FNAME | SIG_ITYPE)
#define SIG_DEFAULT (SIG_FTYPE | SIG_FNAME | SIG_FSPACE | SIG_ITYPE | SIG_INAME | SIG_IINDEXED)
#define SIG_DETAILS (SIG_DEFAULT | SIG_CONST | SIG_ANONYMOUS | SIG_PAYABLE | SIG_ENCODE)
// bitfield for 'is_flags'
#define IS_NOT (0)
#define IS_POINTER (1 << 1)
#define IS_ARRAY (1 << 2)
#define IS_OBJECT (1 << 3)
#define IS_BUILTIN (1 << 4)
#define IS_MINIMAL (1 << 5)
#define IS_ENABLED (1 << 6)
#define IS_NOWRITE (1 << 7)
#define IS_OMITEMPTY (1 << 8)
#define IS_EXTRA (1 << 9)
#define IS_NOADDFLD (1 << 10)
class CMember;
typedef vector<CMember> CMemberArray;
// EXISTING_CODE

//--------------------------------------------------------------------------
class CMember : public CBaseNode {
  public:
    string_q type;
    string_q name;
    string_q strDefault;
    string_q value;
    bool indexed;
    string_q internalType;
    CMemberArray components;
    bool unused;
    uint64_t is_flags;
    uint64_t precision;
    uint64_t maxWidth;
    uint64_t doc;
    uint64_t disp;
    string_q example;
    string_q description;

  public:
    CMember(void);
    CMember(const CMember& pa);
    virtual ~CMember(void);
    CMember& operator=(const CMember& pa);

    DECLARE_NODE(CMember);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    string_q resolveType(void) const;
    explicit CMember(const string_q& n, const string_q& type, const string_q& val = "");
    explicit CMember(const string_q& n, const string_q& type, uint64_t val);
    explicit CMember(const string_q& n, const string_q& type, int64_t val);
    explicit CMember(const string_q& n, const string_q& type, bool val);
    explicit CMember(const string_q& n, const string_q& type, biguint_t val);
    explicit CMember(const string_q& n, const string_q& type, const CStringArray& array);
    bool isValid(void) const;
    void postProcessType(void);
    // EXISTING_CODE
    bool operator==(const CMember& it) const;
    bool operator!=(const CMember& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CMember& v1, const CMember& v2);
    friend ostream& operator<<(ostream& os, const CMember& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CMember& pa);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CMember::CMember(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CMember::CMember(const CMember& pa) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(pa);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CMember::~CMember(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMember::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMember::initialize(void) {
    CBaseNode::initialize();

    type = "";
    name = "";
    strDefault = "";
    value = "";
    indexed = false;
    internalType = "";
    components.clear();
    unused = false;
    is_flags = IS_ENABLED;
    precision = 5;
    maxWidth = NOPOS;
    doc = 0;
    disp = 0;
    example = "";
    description = "";

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMember::duplicate(const CMember& pa) {
    clear();
    CBaseNode::duplicate(pa);

    type = pa.type;
    name = pa.name;
    strDefault = pa.strDefault;
    value = pa.value;
    indexed = pa.indexed;
    internalType = pa.internalType;
    components = pa.components;
    unused = pa.unused;
    is_flags = pa.is_flags;
    precision = pa.precision;
    maxWidth = pa.maxWidth;
    doc = pa.doc;
    disp = pa.disp;
    example = pa.example;
    description = pa.description;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CMember& CMember::operator=(const CMember& pa) {
    duplicate(pa);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CMember::operator==(const CMember& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CMember& v1, const CMember& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CMember> CMemberArray;
extern CArchive& operator>>(CArchive& archive, CMemberArray& array);
extern CArchive& operator<<(CArchive& archive, const CMemberArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CMember& par);
extern CArchive& operator>>(CArchive& archive, CMember& par);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_MEMBER;

//---------------------------------------------------------------------------
// EXISTING_CODE
inline string_q members_2_Str(CMemberArray& members) {
    string_q ret;
    for (auto member : members) {
        if (!ret.empty())
            ret += ", ";
        ret += member.value;
    }
    return trim(ret);
}
// EXISTING_CODE
}  // namespace qblocks
