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
// bitfield for 'memberFlags'
#define IS_POINTER (1 << 1)
#define IS_ARRAY (1 << 2)
#define IS_OBJECT (1 << 3)
#define IS_BUILTIN (1 << 4)
#define IS_MINIMAL (1 << 5)
#define IS_ENABLED (1 << 6)
#define IS_NOWRITE (1 << 7)
#define IS_OMITEMPTY (1 << 8)
#define IS_NOADDFLD (1 << 9)
#define IS_RAWONLY (1 << 10)
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
    uint64_t memberFlags;
    uint64_t precision;
    uint64_t maxWidth;
    uint64_t doc;
    uint64_t disp;
    string_q example;
    string_q description;

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
    memberFlags = IS_ENABLED;
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
    memberFlags = me.memberFlags;
    precision = me.precision;
    maxWidth = me.maxWidth;
    doc = me.doc;
    disp = me.disp;
    example = me.example;
    description = me.description;

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
    // Equality operator as defined in class definition
    return name == it.name;
}

//-------------------------------------------------------------------------
inline bool operator<(const CMember& v1, const CMember& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // Default sort as defined in class definition
    return v1.name < v2.name;
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
