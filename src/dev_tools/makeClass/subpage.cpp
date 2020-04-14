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
#include "subpage.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CSubpage, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextSubpageChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextSubpageChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CSubpage::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["subpage_fmt"] : fmtIn);
    if (fmt.empty()) {
        ctx << toJson();
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextSubpageChunk, this);
}

//---------------------------------------------------------------------------
string_q nextSubpageChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CSubpage*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CSubpage::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextSubpageChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'e':
            if (fieldName % "extract") {
                return extract;
            }
            break;
        case 'i':
            if (fieldName % "icon") {
                return icon;
            }
            break;
        case 'o':
            if (fieldName % "options") {
                return options;
            }
            break;
        case 'p':
            if (fieldName % "page") {
                return page;
            }
            break;
        case 'r':
            if (fieldName % "route") {
                return route;
            }
            break;
        case 's':
            if (fieldName % "subpage") {
                return subpage;
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
bool CSubpage::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    if (fieldName == "subpage" && fieldValue % "separator")
        isSeparator = true;
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'e':
            if (fieldName % "extract") {
                extract = fieldValue;
                return true;
            }
            break;
        case 'i':
            if (fieldName % "icon") {
                icon = fieldValue;
                return true;
            }
            break;
        case 'o':
            if (fieldName % "options") {
                options = fieldValue;
                return true;
            }
            break;
        case 'p':
            if (fieldName % "page") {
                page = fieldValue;
                return true;
            }
            break;
        case 'r':
            if (fieldName % "route") {
                route = fieldValue;
                return true;
            }
            break;
        case 's':
            if (fieldName % "subpage") {
                subpage = fieldValue;
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CSubpage::finishParse() {
    // EXISTING_CODE
    if (subpage == "dashboard") {
        route = subpage;
        options = page;
    }
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CSubpage::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> page;
    archive >> subpage;
    archive >> route;
    archive >> options;
    archive >> extract;
    archive >> icon;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CSubpage::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << page;
    archive << subpage;
    archive << route;
    archive << options;
    archive << extract;
    archive << icon;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CSubpageArray& array) {
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
CArchive& operator<<(CArchive& archive, const CSubpageArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CSubpage::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CSubpage, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CSubpage, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CSubpage, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CSubpage, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CSubpage, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CSubpage, "page", T_TEXT, ++fieldNum);
    ADD_FIELD(CSubpage, "subpage", T_TEXT, ++fieldNum);
    ADD_FIELD(CSubpage, "route", T_TEXT, ++fieldNum);
    ADD_FIELD(CSubpage, "options", T_TEXT, ++fieldNum);
    ADD_FIELD(CSubpage, "extract", T_TEXT, ++fieldNum);
    ADD_FIELD(CSubpage, "icon", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CSubpage, "schema");
    HIDE_FIELD(CSubpage, "deleted");
    HIDE_FIELD(CSubpage, "showing");
    HIDE_FIELD(CSubpage, "cname");

    builtIns.push_back(_biCSubpage);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextSubpageChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CSubpage* sub = reinterpret_cast<const CSubpage*>(dataPtr);
    if (sub) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, sub);
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
bool CSubpage::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CSubpage& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_SUBPAGE = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
