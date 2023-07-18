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
#include "option.h"
#include "markdown.h"
#include "exportcontext.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(COption, CBaseNode);

//---------------------------------------------------------------------------
extern string_q nextOptionChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextOptionChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void COption::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["option_fmt"] : fmtIn);
    if (fmt.empty()) {
        toJson(ctx);
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextOptionChunk, this);
}

//---------------------------------------------------------------------------
string_q nextOptionChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const COption*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q COption::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextOptionChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'd':
            if (fieldName % "description") {
                return description;
            }
            break;
        case 'h':
            if (fieldName % "hotKey") {
                return hotKey;
            }
            break;
        case 'i':
            if (fieldName % "is_visible") {
                return bool_2_Str(is_visible);
            }
            if (fieldName % "is_positional") {
                return bool_2_Str(is_positional);
            }
            if (fieldName % "is_required") {
                return bool_2_Str(is_required);
            }
            if (fieldName % "is_deprecated") {
                return bool_2_Str(is_deprecated);
            }
            break;
        case 'l':
            if (fieldName % "longName") {
                return longName;
            }
            break;
        case 'o':
            if (fieldName % "option_type") {
                return option_type;
            }
            break;
        case 'p':
            if (fieldName % "permitted") {
                return permitted;
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
bool COption::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'd':
            if (fieldName % "description") {
                description = fieldValue;
                return true;
            }
            break;
        case 'h':
            if (fieldName % "hotKey") {
                hotKey = fieldValue;
                return true;
            }
            break;
        case 'i':
            if (fieldName % "is_visible") {
                is_visible = str_2_Bool(fieldValue);
                return true;
            }
            if (fieldName % "is_positional") {
                is_positional = str_2_Bool(fieldValue);
                return true;
            }
            if (fieldName % "is_required") {
                is_required = str_2_Bool(fieldValue);
                return true;
            }
            if (fieldName % "is_deprecated") {
                is_deprecated = str_2_Bool(fieldValue);
                return true;
            }
            break;
        case 'l':
            if (fieldName % "longName") {
                longName = fieldValue;
                return true;
            }
            break;
        case 'o':
            if (fieldName % "option_type") {
                option_type = fieldValue;
                return true;
            }
            break;
        case 'p':
            if (fieldName % "permitted") {
                permitted = fieldValue;
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void COption::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool COption::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> hotKey;
    archive >> longName;
    archive >> description;
    archive >> permitted;
    archive >> option_type;
    archive >> is_visible;
    archive >> is_positional;
    archive >> is_required;
    archive >> is_deprecated;
    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool COption::SerializeC(CArchive& archive) const {
    // Writing always writes the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << hotKey;
    archive << longName;
    archive << description;
    archive << permitted;
    archive << option_type;
    archive << is_visible;
    archive << is_positional;
    archive << is_required;
    archive << is_deprecated;
    // EXISTING_CODE
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------------------------------
bool COption::Migrate(CArchive& archiveIn, CArchive& archiveOut) const {
    ASSERT(archiveIn.isReading());
    ASSERT(archiveOut.isWriting());
    COption copy;
    // EXISTING_CODE
    // EXISTING_CODE
    copy.Serialize(archiveIn);
    copy.SerializeC(archiveOut);
    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, COptionArray& array) {
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
CArchive& operator<<(CArchive& archive, const COptionArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void COption::registerClass(void) {
    // only do this once
    if (HAS_FIELD(COption, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(COption, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(COption, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(COption, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(COption, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(COption, "hotKey", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(COption, "longName", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(COption, "description", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(COption, "permitted", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(COption, "option_type", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(COption, "is_visible", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(COption, "is_positional", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(COption, "is_required", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(COption, "is_deprecated", T_BOOL | TS_OMITEMPTY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(COption, "schema");
    HIDE_FIELD(COption, "deleted");
    HIDE_FIELD(COption, "showing");
    HIDE_FIELD(COption, "cname");

    builtIns.push_back(_biCOption);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextOptionChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const COption* opt = reinterpret_cast<const COption*>(dataPtr);
    if (opt) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
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

// EXISTING_CODE
// EXISTING_CODE

//---------------------------------------------------------------------------
bool COption::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const COption& opt) {
    opt.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, COption& opt) {
    opt.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const COption& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_OPTION = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
//---------------------------------------------------------------------------
COption::COption(const string_q& ln, const string_q& sn, const string_q& t, size_t opts, const string_q& d) {
    description = substitute(d, "&#44;", ",");

    is_positional = (opts & OPT_POSITIONAL);
    is_visible = !(opts & OPT_HIDDEN);
    is_required = (opts & OPT_REQUIRED);
    is_deprecated = (opts == OPT_DEPRECATED);

    option_type = t;
    permitted = t;
    permitted = substitute(permitted, "<uint32>", "<num>");
    permitted = substitute(permitted, "<uint64>", "<num>");
    permitted = substitute(permitted, "<blknum>", "<num>");
    permitted = substitute(permitted, "<string>", "<str>");
    permitted = substitute(permitted, "list<topic>", "<hash>");
    permitted = substitute(permitted, "list<addr>", "<addr>");
    if (contains(option_type, "enum")) {
        description +=
            ", one [X] of " + substitute(substitute(substitute(option_type, "list<", ""), ">", ""), "enum", "");
        replace(description, " [X]", (contains(option_type, "list") ? " or more" : ""));
        permitted = "<val>";
    }
    if (permitted == "<str>" || permitted == "<hash>" || permitted == "<addr>")
        permitted = "string";

    hotKey = (sn.empty() ? "" : "-" + sn);
    if (ln.empty())
        return;

    if (!(opts & OPT_HELP || opts & OPT_VERBOSE)) {
        longName = "--" + ln + (permitted.empty() ? "" : " " + permitted);
        if (is_positional)
            longName = ln;
    } else {
        longName = "--" + ln;
    }
}

// EXISTING_CODE
}  // namespace qblocks
