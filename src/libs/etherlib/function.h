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
#include "utillib.h"
#include "parameter.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CFunction : public CBaseNode {
  public:
    string_q name;
    string_q type;
    bool anonymous;
    bool constant;
    string_q stateMutability;
    string_q signature;
    string_q encoding;
    string_q message;
    CParameterArray inputs;
    CParameterArray outputs;

  public:
    CFunction(void);
    CFunction(const CFunction& fu);
    virtual ~CFunction(void);
    CFunction& operator=(const CFunction& fu);

    DECLARE_NODE(CFunction);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    bool checkTypes(void) const;
    explicit CFunction(const string_q& n) : name(n), anonymous(false), constant(false) {
    }
    string_q getSignature(uint64_t parts) const;
    string_q encodeItem(void) const;
    friend class CTransaction;
    friend class CLogEntry;
    friend class CTrace;
    bool fromDefinition(const string_q& lineIn);
    string_q compressed(void) const;
    bool isDefault(const CBaseNode* obj) const override;
    // EXISTING_CODE
    bool operator==(const CFunction& it) const;
    bool operator!=(const CFunction& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CFunction& v1, const CFunction& v2);
    friend ostream& operator<<(ostream& os, const CFunction& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CFunction& fu);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CFunction::CFunction(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CFunction::CFunction(const CFunction& fu) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(fu);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CFunction::~CFunction(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CFunction::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CFunction::initialize(void) {
    CBaseNode::initialize();

    name = "";
    type = "";
    anonymous = false;
    constant = false;
    stateMutability = "";
    signature = "";
    encoding = "";
    message = "";
    inputs.clear();
    outputs.clear();

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CFunction::duplicate(const CFunction& fu) {
    clear();
    CBaseNode::duplicate(fu);

    name = fu.name;
    type = fu.type;
    anonymous = fu.anonymous;
    constant = fu.constant;
    stateMutability = fu.stateMutability;
    signature = fu.signature;
    encoding = fu.encoding;
    message = fu.message;
    inputs = fu.inputs;
    outputs = fu.outputs;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CFunction& CFunction::operator=(const CFunction& fu) {
    duplicate(fu);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CFunction::operator==(const CFunction& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // Equality operator as defined in class definition
    return encoding % it.encoding && message % it.message;
}

//-------------------------------------------------------------------------
inline bool operator<(const CFunction& v1, const CFunction& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // Default sort as defined in class definition
    return v1.encoding < v2.encoding;
}

//---------------------------------------------------------------------------
typedef vector<CFunction> CFunctionArray;
extern CArchive& operator>>(CArchive& archive, CFunctionArray& array);
extern CArchive& operator<<(CArchive& archive, const CFunctionArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CFunction& fun);
extern CArchive& operator>>(CArchive& archive, CFunction& fun);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_FUNCTION;

//---------------------------------------------------------------------------
// EXISTING_CODE
//---------------------------------------------------------------------------
inline bool sortByFunctionName(const CFunction& f1, const CFunction& f2) {
    return f1.name < f2.name;
}
// EXISTING_CODE
}  // namespace qblocks
