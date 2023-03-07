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
class CParameter;
typedef vector<CParameter> CParameterArray;
// EXISTING_CODE

//--------------------------------------------------------------------------
class CParameter : public CBaseNode {
  public:
    string_q type;
    string_q name;
    string_q strDefault;
    string_q value;
    bool indexed;
    string_q internalType;
    CParameterArray components;

  public:
    CParameter(void);
    CParameter(const CParameter& pa);
    virtual ~CParameter(void);
    CParameter& operator=(const CParameter& pa);

    DECLARE_NODE(CParameter);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    string_q resolveType(void) const;
    explicit CParameter(const string_q& n, const string_q& type, const string_q& val = "");
    explicit CParameter(const string_q& n, const string_q& type, uint64_t val);
    explicit CParameter(const string_q& n, const string_q& type, int64_t val);
    explicit CParameter(const string_q& n, const string_q& type, bool val);
    explicit CParameter(const string_q& n, const string_q& type, biguint_t val);
    explicit CParameter(const string_q& n, const string_q& type, const CStringArray& array);
    bool isValid(void) const;
    // EXISTING_CODE
    bool operator==(const CParameter& it) const;
    bool operator!=(const CParameter& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CParameter& v1, const CParameter& v2);
    friend ostream& operator<<(ostream& os, const CParameter& it);

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

    type = "";
    name = "";
    strDefault = "";
    value = "";
    indexed = false;
    internalType = "";
    components.clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CParameter::duplicate(const CParameter& pa) {
    clear();
    CBaseNode::duplicate(pa);

    type = pa.type;
    name = pa.name;
    strDefault = pa.strDefault;
    value = pa.value;
    indexed = pa.indexed;
    internalType = pa.internalType;
    components = pa.components;

    // EXISTING_CODE
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
inline bool CParameter::operator==(const CParameter& it) const {
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
extern CArchive& operator<<(CArchive& archive, const CParameter& par);
extern CArchive& operator>>(CArchive& archive, CParameter& par);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_PARAMETER;

//---------------------------------------------------------------------------
// EXISTING_CODE
inline string_q params_2_Str(CParameterArray& params) {
    string_q ret;
    for (auto param : params) {
        if (!ret.empty())
            ret += ", ";
        ret += param.value;
    }
    return trim(ret);
}
// EXISTING_CODE
}  // namespace qblocks
