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
    bool is_pointer;
    bool is_array;
    bool is_object;
    bool is_builtin;
    bool is_minimal;
    bool is_noaddfld;
    bool is_nowrite;
    bool is_omitempty;
    bool is_extra;

  public:
    CMember(void);
    CMember(const CMember& me);
    virtual ~CMember(void);
    CMember& operator=(const CMember& me);

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
    void duplicate(const CMember& me);
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
inline CMember::CMember(const CMember& me) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(me);
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
    is_pointer = false;
    is_array = false;
    is_object = false;
    is_builtin = false;
    is_minimal = false;
    is_noaddfld = false;
    is_nowrite = false;
    is_omitempty = false;
    is_extra = false;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CMember::duplicate(const CMember& me) {
    clear();
    CBaseNode::duplicate(me);

    type = me.type;
    name = me.name;
    strDefault = me.strDefault;
    value = me.value;
    indexed = me.indexed;
    internalType = me.internalType;
    components = me.components;
    unused = me.unused;
    is_flags = me.is_flags;
    precision = me.precision;
    maxWidth = me.maxWidth;
    doc = me.doc;
    disp = me.disp;
    example = me.example;
    description = me.description;
    is_pointer = me.is_pointer;
    is_array = me.is_array;
    is_object = me.is_object;
    is_builtin = me.is_builtin;
    is_minimal = me.is_minimal;
    is_noaddfld = me.is_noaddfld;
    is_nowrite = me.is_nowrite;
    is_omitempty = me.is_omitempty;
    is_extra = me.is_extra;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CMember& CMember::operator=(const CMember& me) {
    duplicate(me);
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
extern CArchive& operator<<(CArchive& archive, const CMember& mem);
extern CArchive& operator>>(CArchive& archive, CMember& mem);

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
