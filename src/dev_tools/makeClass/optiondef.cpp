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
#include <algorithm>
#include "optiondef.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(COptionDef, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextOptiondefChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextOptiondefChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void COptionDef::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["optiondef_fmt"] : fmtIn);
    if (fmt.empty()) {
        ctx << toJson();
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextOptiondefChunk, this);
}

//---------------------------------------------------------------------------
string_q nextOptiondefChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const COptionDef *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool COptionDef::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "api_route" ) { api_route = fieldValue; return true; }
            break;
        case 'c':
            if ( fieldName % "command" ) { command = fieldValue; return true; }
            if ( fieldName % "core_visible" ) { core_visible = fieldValue; return true; }
            break;
        case 'd':
            if ( fieldName % "deft_val" ) { def_val = fieldValue; return true; }
            if ( fieldName % "docs_visible" ) { docs_visible = fieldValue; return true; }
            if ( fieldName % "data_type" ) { data_type = fieldValue; return true; }
            if ( fieldName % "description" ) { description = fieldValue; return true; }
            break;
        case 'g':
            if ( fieldName % "group" ) { group = fieldValue; return true; }
            if ( fieldName % "generate" ) { generate = fieldValue; return true; }
            break;
        case 'h':
            if ( fieldName % "hotkey" ) { hotkey = fieldValue; return true; }
            break;
        case 'i':
            if ( fieldName % "is_required" ) { is_required = fieldValue; return true; }
            break;
        case 'n':
            if ( fieldName % "num" ) { num = fieldValue; return true; }
            break;
        case 'o':
            if ( fieldName % "option_kind" ) { option_kind = fieldValue; return true; }
            break;
        case 't':
            if ( fieldName % "tool" ) { tool = fieldValue; return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void COptionDef::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool COptionDef::Serialize(CArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> num;
    archive >> group;
    archive >> api_route;
    archive >> tool;
    archive >> command;
    archive >> hotkey;
    archive >> def_val;
    archive >> is_required;
    archive >> core_visible;
    archive >> docs_visible;
    archive >> generate;
    archive >> option_kind;
    archive >> data_type;
    archive >> description;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool COptionDef::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << num;
    archive << group;
    archive << api_route;
    archive << tool;
    archive << command;
    archive << hotkey;
    archive << def_val;
    archive << is_required;
    archive << core_visible;
    archive << docs_visible;
    archive << generate;
    archive << option_kind;
    archive << data_type;
    archive << description;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, COptionDefArray& array) {
    uint64_t count;
    archive >> count;
    array.resize(count);
    for (size_t i = 0 ; i < count ; i++) {
        ASSERT(i < array.capacity());
        array.at(i).Serialize(archive);
    }
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const COptionDefArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void COptionDef::registerClass(void) {
    // only do this once
    if (HAS_FIELD(COptionDef, "schema")) return;

    size_t fieldNum = 1000;
    ADD_FIELD(COptionDef, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(COptionDef, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(COptionDef, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(COptionDef, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(COptionDef, "num", T_TEXT, ++fieldNum);
    ADD_FIELD(COptionDef, "group", T_TEXT, ++fieldNum);
    ADD_FIELD(COptionDef, "api_route", T_TEXT, ++fieldNum);
    ADD_FIELD(COptionDef, "tool", T_TEXT, ++fieldNum);
    ADD_FIELD(COptionDef, "command", T_TEXT, ++fieldNum);
    ADD_FIELD(COptionDef, "hotkey", T_TEXT, ++fieldNum);
    ADD_FIELD(COptionDef, "def_val", T_TEXT, ++fieldNum);
    ADD_FIELD(COptionDef, "is_required", T_TEXT, ++fieldNum);
    ADD_FIELD(COptionDef, "core_visible", T_TEXT, ++fieldNum);
    ADD_FIELD(COptionDef, "docs_visible", T_TEXT, ++fieldNum);
    ADD_FIELD(COptionDef, "generate", T_TEXT, ++fieldNum);
    ADD_FIELD(COptionDef, "option_kind", T_TEXT, ++fieldNum);
    ADD_FIELD(COptionDef, "data_type", T_TEXT, ++fieldNum);
    ADD_FIELD(COptionDef, "description", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(COptionDef, "schema");
    HIDE_FIELD(COptionDef, "deleted");
    HIDE_FIELD(COptionDef, "showing");
    HIDE_FIELD(COptionDef, "cname");

    builtIns.push_back(_biCOptionDef);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextOptiondefChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const COptionDef *opt = reinterpret_cast<const COptionDef *>(dataPtr);
    if (opt) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'd':
                if ( fieldIn % "dataType" )
                    return (opt->option_kind == "switch" ? "" : opt->data_type);
                break;
            case 'o':
                if ( fieldIn % "opts" ) {
                    string_q ret;
                    if (opt->is_required == "TRUE")
                        ret += ("|OPT_REQUIRED");

                    if (opt->core_visible != "TRUE")
                        ret += ("|OPT_HIDDEN");

                    if (opt->option_kind == "switch")
                        ret += ("|OPT_SWITCH");
                    else if (opt->option_kind == "toggle")
                        ret += ("|OPT_TOGGLE");
                    else if (opt->option_kind == "flag")
                        ret += ("|OPT_FLAG");
                    else if (opt->option_kind == "positional")
                        ret += ("|OPT_POSITIONAL");
                    else
                        ret += ("|OPT_DESCRIPTION");
                    return substitute(trim(ret,'|'), "|", " | ");
                }
                break;
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, opt);
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
bool COptionDef::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
string_q COptionDef::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextOptiondefChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "api_route" ) return api_route;
            break;
        case 'c':
            if ( fieldName % "command" ) return command;
            if ( fieldName % "core_visible" ) return core_visible;
            break;
        case 'd':
            if ( fieldName % "def_val" ) return def_val;
            if ( fieldName % "docs_visible" ) return docs_visible;
            if ( fieldName % "data_type" ) return data_type;
            if ( fieldName % "description" ) return description;
            break;
        case 'g':
            if ( fieldName % "group" ) return group;
            if ( fieldName % "generate" ) return generate;
            break;
        case 'h':
            if ( fieldName % "hotkey" ) return hotkey;
            break;
        case 'i':
            if ( fieldName % "is_required" ) return is_required;
            break;
        case 'n':
            if ( fieldName % "num" ) return num;
            break;
        case 'o':
            if ( fieldName % "option_kind" ) return option_kind;
            break;
        case 't':
            if ( fieldName % "tool" ) return tool;
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const COptionDef& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_OPTIONDEF = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
//---------------------------------------------------------------------------------------------------
COptionDef::COptionDef(const string_q& line) {
    CStringArray parts;
    explode(parts, line, ',');
    if (parts.size() > 0)  num = parts[0];
    if (parts.size() > 1)  group = parts[1];
    if (parts.size() > 2)  api_route = parts[2];
    if (parts.size() > 3)  tool = parts[3];
    if (parts.size() > 4)  command = parts[4];
    if (parts.size() > 5)  hotkey = parts[5];
    if (parts.size() > 6)  def_val = parts[6];
    if (parts.size() > 7)  is_required = parts[7];
    if (parts.size() > 8)  core_visible = parts[8];
    if (parts.size() > 9)  docs_visible = parts[9];
    if (parts.size() > 10) generate = parts[10];
    if (parts.size() > 11) option_kind = parts[11];
    if (parts.size() > 12) data_type = parts[12];
    if (parts.size() > 13) description = substitute(parts[13], "&#44;", ",");
}
// EXISTING_CODE
}  // namespace qblocks

