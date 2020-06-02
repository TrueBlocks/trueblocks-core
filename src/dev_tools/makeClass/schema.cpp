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
#include <algorithm>
#include "schema.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CSchema, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextSchemaChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextSchemaChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CSchema::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["schema_fmt"] : fmtIn);
    if (fmt.empty()) {
        toJson(ctx);
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextSchemaChunk, this);
}

//---------------------------------------------------------------------------
string_q nextSchemaChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CSchema*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CSchema::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextSchemaChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "align") {
                return align;
            }
            break;
        case 'c':
            if (fieldName % "cn") {
                return cn;
            }
            if (fieldName % "chart") {
                return chart;
            }
            break;
        case 'd':
            if (fieldName % "decimals") {
                return uint_2_Str(decimals);
            }
            if (fieldName % "download") {
                return bool_2_Str(download);
            }
            if (fieldName % "detail") {
                return bool_2_Str(detail);
            }
            break;
        case 'e':
            if (fieldName % "editable") {
                return bool_2_Str(editable);
            }
            break;
        case 'f':
            if (fieldName % "function") {
                return function;
            }
            break;
        case 'h':
            if (fieldName % "hidden") {
                return bool_2_Str(hidden);
            }
            if (fieldName % "hide_empty") {
                return bool_2_Str(hide_empty);
            }
            break;
        case 'i':
            if (fieldName % "id") {
                return uint_2_Str(id);
            }
            if (fieldName % "isPill") {
                return bool_2_Str(isPill);
            }
            break;
        case 'n':
            if (fieldName % "name") {
                return name;
            }
            break;
        case 'o':
            if (fieldName % "onDisplay") {
                return onDisplay;
            }
            if (fieldName % "onAccept") {
                return onAccept;
            }
            if (fieldName % "onValidate") {
                return onValidate;
            }
            break;
        case 's':
            if (fieldName % "selector") {
                return selector;
            }
            if (fieldName % "searchable") {
                return bool_2_Str(searchable);
            }
            if (fieldName % "sortable") {
                return bool_2_Str(sortable);
            }
            break;
        case 't':
            if (fieldName % "type") {
                return type;
            }
            break;
        case 'u':
            if (fieldName % "underField") {
                return underField;
            }
            break;
        case 'w':
            if (fieldName % "width") {
                return uint_2_Str(width);
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
bool CSchema::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "align") {
                align = fieldValue;
                return true;
            }
            break;
        case 'c':
            if (fieldName % "cn") {
                cn = fieldValue;
                return true;
            }
            if (fieldName % "chart") {
                chart = fieldValue;
                return true;
            }
            break;
        case 'd':
            if (fieldName % "decimals") {
                decimals = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "download") {
                download = str_2_Bool(fieldValue);
                return true;
            }
            if (fieldName % "detail") {
                detail = str_2_Bool(fieldValue);
                return true;
            }
            break;
        case 'e':
            if (fieldName % "editable") {
                editable = str_2_Bool(fieldValue);
                return true;
            }
            break;
        case 'f':
            if (fieldName % "function") {
                function = fieldValue;
                return true;
            }
            break;
        case 'h':
            if (fieldName % "hidden") {
                hidden = str_2_Bool(fieldValue);
                return true;
            }
            if (fieldName % "hide_empty") {
                hide_empty = str_2_Bool(fieldValue);
                return true;
            }
            break;
        case 'i':
            if (fieldName % "id") {
                id = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "isPill") {
                isPill = str_2_Bool(fieldValue);
                return true;
            }
            break;
        case 'n':
            if (fieldName % "name") {
                name = fieldValue;
                return true;
            }
            break;
        case 'o':
            if (fieldName % "onDisplay") {
                onDisplay = fieldValue;
                return true;
            }
            if (fieldName % "onAccept") {
                onAccept = fieldValue;
                return true;
            }
            if (fieldName % "onValidate") {
                onValidate = fieldValue;
                return true;
            }
            break;
        case 's':
            if (fieldName % "selector") {
                selector = fieldValue;
                return true;
            }
            if (fieldName % "searchable") {
                searchable = str_2_Bool(fieldValue);
                return true;
            }
            if (fieldName % "sortable") {
                sortable = str_2_Bool(fieldValue);
                return true;
            }
            break;
        case 't':
            if (fieldName % "type") {
                type = fieldValue;
                return true;
            }
            break;
        case 'u':
            if (fieldName % "underField") {
                underField = fieldValue;
                return true;
            }
            break;
        case 'w':
            if (fieldName % "width") {
                width = str_2_Uint(fieldValue);
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CSchema::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CSchema::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> name;
    archive >> selector;
    archive >> type;
    archive >> hidden;
    archive >> width;
    archive >> function;
    archive >> editable;
    archive >> id;
    archive >> decimals;
    archive >> isPill;
    archive >> align;
    archive >> cn;
    archive >> download;
    archive >> chart;
    archive >> detail;
    archive >> hide_empty;
    archive >> searchable;
    archive >> sortable;
    archive >> underField;
    archive >> onDisplay;
    archive >> onAccept;
    archive >> onValidate;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CSchema::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << name;
    archive << selector;
    archive << type;
    archive << hidden;
    archive << width;
    archive << function;
    archive << editable;
    archive << id;
    archive << decimals;
    archive << isPill;
    archive << align;
    archive << cn;
    archive << download;
    archive << chart;
    archive << detail;
    archive << hide_empty;
    archive << searchable;
    archive << sortable;
    archive << underField;
    archive << onDisplay;
    archive << onAccept;
    archive << onValidate;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CSchemaArray& array) {
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
CArchive& operator<<(CArchive& archive, const CSchemaArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CSchema::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CSchema, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CSchema, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CSchema, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CSchema, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CSchema, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CSchema, "name", T_TEXT, ++fieldNum);
    ADD_FIELD(CSchema, "selector", T_TEXT, ++fieldNum);
    ADD_FIELD(CSchema, "type", T_TEXT, ++fieldNum);
    ADD_FIELD(CSchema, "hidden", T_BOOL, ++fieldNum);
    ADD_FIELD(CSchema, "width", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CSchema, "function", T_TEXT, ++fieldNum);
    ADD_FIELD(CSchema, "editable", T_BOOL, ++fieldNum);
    ADD_FIELD(CSchema, "id", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CSchema, "decimals", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CSchema, "isPill", T_BOOL, ++fieldNum);
    ADD_FIELD(CSchema, "align", T_TEXT, ++fieldNum);
    ADD_FIELD(CSchema, "cn", T_TEXT, ++fieldNum);
    ADD_FIELD(CSchema, "download", T_BOOL, ++fieldNum);
    ADD_FIELD(CSchema, "chart", T_TEXT, ++fieldNum);
    ADD_FIELD(CSchema, "detail", T_BOOL, ++fieldNum);
    ADD_FIELD(CSchema, "hide_empty", T_BOOL, ++fieldNum);
    ADD_FIELD(CSchema, "searchable", T_BOOL, ++fieldNum);
    ADD_FIELD(CSchema, "sortable", T_BOOL, ++fieldNum);
    ADD_FIELD(CSchema, "underField", T_TEXT, ++fieldNum);
    ADD_FIELD(CSchema, "onDisplay", T_TEXT, ++fieldNum);
    ADD_FIELD(CSchema, "onAccept", T_TEXT, ++fieldNum);
    ADD_FIELD(CSchema, "onValidate", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CSchema, "schema");
    HIDE_FIELD(CSchema, "deleted");
    HIDE_FIELD(CSchema, "showing");
    HIDE_FIELD(CSchema, "cname");

    builtIns.push_back(_biCSchema);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextSchemaChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CSchema* sch = reinterpret_cast<const CSchema*>(dataPtr);
    if (sch) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, sch);
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
bool CSchema::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CSchema& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_SCHEMA = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
bool CSchema::showEmptyField(const string_q& fn) const {
    if (fn == "name")
        return true;
    string_q val = getValueByName(fn);
    return val != "false" && val != "0" && val != "null" && !val.empty();
}
// EXISTING_CODE
}  // namespace qblocks
