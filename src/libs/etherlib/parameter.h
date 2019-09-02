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
#include "utillib.h"

namespace qblocks {

// EXISTING_CODE
//--------------------------------------------------------------------------
#define SIG_FTYPE     (1<<1)
#define SIG_FNAME     (1<<2)
#define SIG_FSPACE    (1<<3)
#define SIG_ITYPE     (1<<4)
#define SIG_INAME     (1<<5)
#define SIG_IINDEXED  (1<<6)
#define SIG_ENCODE    (1<<7)
#define SIG_CONST     (1<<8)
#define SIG_ANONYMOUS (1<<9)
#define SIG_PAYABLE   (1<<10)
#define SIG_CANONICAL (SIG_FNAME|SIG_ITYPE)
#define SIG_DEFAULT   (SIG_FTYPE|SIG_FNAME|SIG_FSPACE|SIG_ITYPE|SIG_INAME|SIG_IINDEXED)
#define SIG_DETAILS   (SIG_DEFAULT|SIG_CONST|SIG_ANONYMOUS|SIG_PAYABLE|SIG_ENCODE)
// EXISTING_CODE

//--------------------------------------------------------------------------
class CParameter : public CBaseNode {
public:
    bool indexed;
    string_q name;
    string_q type;
    bool isPointer;
    bool isArray;
    bool isObject;
    string_q strDefault;
    string_q value;

public:
    CParameter(void);
    CParameter(const CParameter& pa);
    virtual ~CParameter(void);
    CParameter& operator=(const CParameter& pa);

    DECLARE_NODE(CParameter);

    // EXISTING_CODE
    explicit CParameter(string_q& txtIn);
    explicit CParameter(const string_q& n, const string_q& type, const string_q& val="");
    explicit CParameter(const string_q& n, const string_q& type, uint64_t val);
    explicit CParameter(const string_q& n, const string_q& type, int64_t val);
    explicit CParameter(const string_q& n, const string_q& type, bool val);
    explicit CParameter(const string_q& n, const string_q& type, biguint_t val);
    explicit CParameter(const string_q& n, const string_q& type, const CStringArray& array);
    string_q getFunctionAssign(uint64_t which) const;
    string_q getEventAssign(uint64_t which, uint64_t nIndexed = NOPOS) const;
    bool fromDefinition(const string_q& input);
    bool isValid(void) const;
    bool noWrite;
    // EXISTING_CODE
    bool operator==(const CParameter& item) const;
    bool operator!=(const CParameter& item) const { return !operator==(item); }
    friend bool operator<(const CParameter& v1, const CParameter& v2);
    friend ostream& operator<<(ostream& os, const CParameter& item);

protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CParameter& pa);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CParameter::CParameter(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CParameter::CParameter(const CParameter& pa) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(pa);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CParameter::~CParameter(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CParameter::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CParameter::initialize(void) {
    CBaseNode::initialize();

    indexed = 0;
    name = "";
    type = "";
    isPointer = 0;
    isArray = 0;
    isObject = 0;
    strDefault = "";
    value = "";

    // EXISTING_CODE
    noWrite = false;
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CParameter::duplicate(const CParameter& pa) {
    clear();
    CBaseNode::duplicate(pa);

    indexed = pa.indexed;
    name = pa.name;
    type = pa.type;
    isPointer = pa.isPointer;
    isArray = pa.isArray;
    isObject = pa.isObject;
    strDefault = pa.strDefault;
    value = pa.value;

    // EXISTING_CODE
    noWrite = pa.noWrite;
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CParameter& CParameter::operator=(const CParameter& pa) {
    duplicate(pa);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CParameter::operator==(const CParameter& item) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CParameter& v1, const CParameter& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CParameter> CParameterArray;
extern CArchive& operator>>(CArchive& archive, CParameterArray& array);
extern CArchive& operator<<(CArchive& archive, const CParameterArray& array);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_PARAMETER;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

