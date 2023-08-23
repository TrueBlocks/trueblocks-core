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
#include "member.h"

namespace qblocks {

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CClassDefinition : public CBaseNode {
  public:
    string_q short_fn;
    string_q input_path;
    string_q class_name;
    string_q class_base;
    string_q class_upper;
    string_q base_class;
    string_q base_name;
    string_q base_proper;
    string_q base_lower;
    string_q base_upper;
    string_q base_base;
    string_q go_model;
    string_q head_includes;
    string_q src_includes;
    string_q display_str;
    CMemberArray fieldArray;
    CMemberArray extraArray;
    string_q contained_by;
    string_q doc_group;
    string_q doc_order;
    string_q doc_descr;
    string_q doc_route;
    string_q doc_alias;
    string_q doc_producer;
    string_q go_output;
    string_q cache_as;
    string_q cache_by;
    string_q cache_type;
    bool disabled;

  public:
    CClassDefinition(void);
    CClassDefinition(const CClassDefinition& cl);
    virtual ~CClassDefinition(void);
    CClassDefinition& operator=(const CClassDefinition& cl);

    DECLARE_NODE(CClassDefinition);

    const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;

    // EXISTING_CODE
    void ReadSettings(const CToml& toml);
    // EXISTING_CODE
    bool operator==(const CClassDefinition& it) const;
    bool operator!=(const CClassDefinition& it) const {
        return !operator==(it);
    }
    friend bool operator<(const CClassDefinition& v1, const CClassDefinition& v2);
    friend ostream& operator<<(ostream& os, const CClassDefinition& it);

  protected:
    void clear(void);
    void initialize(void);
    void duplicate(const CClassDefinition& cl);
    bool readBackLevel(CArchive& archive) override;

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CClassDefinition::CClassDefinition(void) {
    initialize();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CClassDefinition::CClassDefinition(const CClassDefinition& cl) {
    // EXISTING_CODE
    // EXISTING_CODE
    duplicate(cl);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CClassDefinition::~CClassDefinition(void) {
    clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CClassDefinition::clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CClassDefinition::initialize(void) {
    CBaseNode::initialize();

    short_fn = "";
    input_path = "";
    class_name = "";
    class_base = "";
    class_upper = "";
    base_class = "";
    base_name = "";
    base_proper = "";
    base_lower = "";
    base_upper = "";
    base_base = "";
    go_model = "";
    head_includes = "";
    src_includes = "";
    display_str = "";
    fieldArray.clear();
    extraArray.clear();
    contained_by = "";
    doc_group = "";
    doc_order = "";
    doc_descr = "";
    doc_route = "";
    doc_alias = "";
    doc_producer = "";
    go_output = "";
    cache_as = "";
    cache_by = "";
    cache_type = "";
    disabled = false;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CClassDefinition::duplicate(const CClassDefinition& cl) {
    clear();
    CBaseNode::duplicate(cl);

    short_fn = cl.short_fn;
    input_path = cl.input_path;
    class_name = cl.class_name;
    class_base = cl.class_base;
    class_upper = cl.class_upper;
    base_class = cl.base_class;
    base_name = cl.base_name;
    base_proper = cl.base_proper;
    base_lower = cl.base_lower;
    base_upper = cl.base_upper;
    base_base = cl.base_base;
    go_model = cl.go_model;
    head_includes = cl.head_includes;
    src_includes = cl.src_includes;
    display_str = cl.display_str;
    fieldArray = cl.fieldArray;
    extraArray = cl.extraArray;
    contained_by = cl.contained_by;
    doc_group = cl.doc_group;
    doc_order = cl.doc_order;
    doc_descr = cl.doc_descr;
    doc_route = cl.doc_route;
    doc_alias = cl.doc_alias;
    doc_producer = cl.doc_producer;
    go_output = cl.go_output;
    cache_as = cl.cache_as;
    cache_by = cl.cache_by;
    cache_type = cl.cache_type;
    disabled = cl.disabled;

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CClassDefinition& CClassDefinition::operator=(const CClassDefinition& cl) {
    duplicate(cl);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//-------------------------------------------------------------------------
inline bool CClassDefinition::operator==(const CClassDefinition& it) const {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default equal operator in class definition, assume none are equal (so find fails)
    return false;
}

//-------------------------------------------------------------------------
inline bool operator<(const CClassDefinition& v1, const CClassDefinition& v2) {
    // EXISTING_CODE
    // EXISTING_CODE
    // No default sort defined in class definition, assume already sorted, preserve ordering
    return true;
}

//---------------------------------------------------------------------------
typedef vector<CClassDefinition> CClassDefinitionArray;
extern CArchive& operator>>(CArchive& archive, CClassDefinitionArray& array);
extern CArchive& operator<<(CArchive& archive, const CClassDefinitionArray& array);

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CClassDefinition& cla);
extern CArchive& operator>>(CArchive& archive, CClassDefinition& cla);

//---------------------------------------------------------------------------
extern const char* STR_DISPLAY_CLASSDEFINITION;

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
