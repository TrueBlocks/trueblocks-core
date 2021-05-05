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
#include "classdefinition.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CClassDefinition, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextClassdefinitionChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextClassdefinitionChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CClassDefinition::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["classdefinition_fmt"] : fmtIn);
    if (fmt.empty()) {
        if (expContext().exportFmt == YAML1) {
            toYaml(ctx);
        } else {
            toJson(ctx);
        }
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextClassdefinitionChunk, this);
}

//---------------------------------------------------------------------------
string_q nextClassdefinitionChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CClassDefinition*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CClassDefinition::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextClassdefinitionChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'b':
            if (fieldName % "base_class") {
                return base_class;
            }
            if (fieldName % "base_name") {
                return base_name;
            }
            if (fieldName % "base_proper") {
                return base_proper;
            }
            if (fieldName % "base_lower") {
                return base_lower;
            }
            if (fieldName % "base_upper") {
                return base_upper;
            }
            if (fieldName % "base_base") {
                return base_base;
            }
            break;
        case 'c':
            if (fieldName % "class_name") {
                return class_name;
            }
            if (fieldName % "class_base") {
                return class_base;
            }
            if (fieldName % "class_upper") {
                return class_upper;
            }
            break;
        case 'd':
            if (fieldName % "display_str") {
                return display_str;
            }
            break;
        case 'e':
            if (fieldName % "eq_str") {
                return eq_str;
            }
            break;
        case 'f':
            if (fieldName % "field_str") {
                return field_str;
            }
            if (fieldName % "fieldArray" || fieldName % "fieldArrayCnt") {
                size_t cnt = fieldArray.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
                if (!cnt)
                    return "";
                string_q retS;
                for (size_t i = 0; i < cnt; i++) {
                    retS += fieldArray[i].Format();
                    retS += ((i < cnt - 1) ? ",\n" : "\n");
                }
                return retS;
            }
            break;
        case 'h':
            if (fieldName % "head_includes") {
                return head_includes;
            }
            break;
        case 'i':
            if (fieldName % "input_path") {
                return input_path;
            }
            break;
        case 's':
            if (fieldName % "short_fn") {
                return short_fn;
            }
            if (fieldName % "src_includes") {
                return src_includes;
            }
            if (fieldName % "sort_str") {
                return sort_str;
            }
            if (fieldName % "scope_str") {
                return scope_str;
            }
            if (fieldName % "serializable") {
                return bool_2_Str_t(serializable);
            }
            break;
        default:
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//---------------------------------------------------------------------------------------------------
bool CClassDefinition::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'b':
            if (fieldName % "base_class") {
                base_class = fieldValue;
                return true;
            }
            if (fieldName % "base_name") {
                base_name = fieldValue;
                return true;
            }
            if (fieldName % "base_proper") {
                base_proper = fieldValue;
                return true;
            }
            if (fieldName % "base_lower") {
                base_lower = fieldValue;
                return true;
            }
            if (fieldName % "base_upper") {
                base_upper = fieldValue;
                return true;
            }
            if (fieldName % "base_base") {
                base_base = fieldValue;
                return true;
            }
            break;
        case 'c':
            if (fieldName % "class_name") {
                class_name = fieldValue;
                return true;
            }
            if (fieldName % "class_base") {
                class_base = fieldValue;
                return true;
            }
            if (fieldName % "class_upper") {
                class_upper = fieldValue;
                return true;
            }
            break;
        case 'd':
            if (fieldName % "display_str") {
                display_str = fieldValue;
                return true;
            }
            break;
        case 'e':
            if (fieldName % "eq_str") {
                eq_str = fieldValue;
                return true;
            }
            break;
        case 'f':
            if (fieldName % "field_str") {
                field_str = fieldValue;
                return true;
            }
            if (fieldName % "fieldArray") {
                CParameter obj;
                string_q str = fieldValue;
                while (obj.parseJson3(str)) {
                    fieldArray.push_back(obj);
                    obj = CParameter();  // reset
                }
                return true;
            }
            break;
        case 'h':
            if (fieldName % "head_includes") {
                head_includes = fieldValue;
                return true;
            }
            break;
        case 'i':
            if (fieldName % "input_path") {
                input_path = fieldValue;
                return true;
            }
            break;
        case 's':
            if (fieldName % "short_fn") {
                short_fn = fieldValue;
                return true;
            }
            if (fieldName % "src_includes") {
                src_includes = fieldValue;
                return true;
            }
            if (fieldName % "sort_str") {
                sort_str = fieldValue;
                return true;
            }
            if (fieldName % "scope_str") {
                scope_str = fieldValue;
                return true;
            }
            if (fieldName % "serializable") {
                serializable = str_2_Bool(fieldValue);
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CClassDefinition::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CClassDefinition::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> short_fn;
    archive >> input_path;
    archive >> class_name;
    archive >> class_base;
    archive >> class_upper;
    archive >> base_class;
    archive >> base_name;
    archive >> base_proper;
    archive >> base_lower;
    archive >> base_upper;
    archive >> base_base;
    archive >> head_includes;
    archive >> src_includes;
    archive >> field_str;
    archive >> display_str;
    archive >> sort_str;
    archive >> eq_str;
    archive >> scope_str;
    archive >> serializable;
    // archive >> fieldArray;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CClassDefinition::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << short_fn;
    archive << input_path;
    archive << class_name;
    archive << class_base;
    archive << class_upper;
    archive << base_class;
    archive << base_name;
    archive << base_proper;
    archive << base_lower;
    archive << base_upper;
    archive << base_base;
    archive << head_includes;
    archive << src_includes;
    archive << field_str;
    archive << display_str;
    archive << sort_str;
    archive << eq_str;
    archive << scope_str;
    archive << serializable;
    // archive << fieldArray;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CClassDefinitionArray& array) {
    uint64_t count;
    archive >> count;
    array.resize(count);
    for (size_t i = 0; i < count; i++) {
        ASSERT(i < array.capacity());
        array.at(i).Serialize(archive);
    }
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CClassDefinitionArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CClassDefinition::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CClassDefinition, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CClassDefinition, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CClassDefinition, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CClassDefinition, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CClassDefinition, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CClassDefinition, "short_fn", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CClassDefinition, "input_path", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CClassDefinition, "class_name", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CClassDefinition, "class_base", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CClassDefinition, "class_upper", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CClassDefinition, "base_class", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CClassDefinition, "base_name", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CClassDefinition, "base_proper", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CClassDefinition, "base_lower", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CClassDefinition, "base_upper", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CClassDefinition, "base_base", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CClassDefinition, "head_includes", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CClassDefinition, "src_includes", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CClassDefinition, "field_str", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CClassDefinition, "display_str", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CClassDefinition, "sort_str", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CClassDefinition, "eq_str", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CClassDefinition, "scope_str", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CClassDefinition, "serializable", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CClassDefinition, "fieldArray", T_OBJECT | TS_ARRAY | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CClassDefinition, "fieldArray");

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CClassDefinition, "schema");
    HIDE_FIELD(CClassDefinition, "deleted");
    HIDE_FIELD(CClassDefinition, "showing");
    HIDE_FIELD(CClassDefinition, "cname");

    builtIns.push_back(_biCClassDefinition);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextClassdefinitionChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CClassDefinition* cla = reinterpret_cast<const CClassDefinition*>(dataPtr);
    if (cla) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, cla);
                // EXISTING_CODE
                // EXISTING_CODE
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool CClassDefinition::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CClassDefinition& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode* CClassDefinition::getObjectAt(const string_q& fieldName, size_t index) const {
    if (fieldName % "fieldArray") {
        if (index == NOPOS) {
            CParameter empty;
            ((CClassDefinition*)this)->fieldArray.push_back(empty);  // NOLINT
            index = fieldArray.size() - 1;
        }
        if (index < fieldArray.size())
            return &fieldArray[index];
    }

    return NULL;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_CLASSDEFINITION = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
CClassDefinition::CClassDefinition(const CToml& toml) {
    //------------------------------------------------------------------------------------------------
    class_name = toml.getConfigStr("settings", "class", "");
    base_class = toml.getConfigStr("settings", "base_class", "CBaseNode");
    field_str = toml.getConfigStr("settings", "fields", "");
    head_includes = toml.getConfigStr("settings", "includes", "");
    src_includes = toml.getConfigStr("settings", "cpp_includes", "");
    display_str = toml.getConfigStr("settings", "display_str", "");
    sort_str = toml.getConfigStr("settings", "sort", "");
    eq_str = toml.getConfigStr("settings", "equals", "");
    scope_str = toml.getConfigStr("settings", "scope", "static");  // TODO(tjayrush): global data
    serializable = toml.getConfigBool("settings", "serializable", false);

    //------------------------------------------------------------------------------------------------
    class_base = toProper(extract(class_name, 1));

    //------------------------------------------------------------------------------------------------
    base_name = extract(class_name, 1);
    base_proper = toProper(base_name);
    base_lower = toLower(base_name);
    base_upper = toUpper(base_name);
    base_base = toProper(extract(base_class, 1));

    explode(fieldArray, field_str, '|');
}
// EXISTING_CODE
}  // namespace qblocks
