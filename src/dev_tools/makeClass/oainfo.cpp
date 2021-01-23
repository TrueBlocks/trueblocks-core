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
#include "oainfo.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(COAInfo, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextOainfoChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextOainfoChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void COAInfo::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["oainfo_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextOainfoChunk, this);
}

//---------------------------------------------------------------------------
string_q nextOainfoChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const COAInfo*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q COAInfo::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextOainfoChunk_custom(fieldName, this);
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
        case 't':
            if (fieldName % "title") {
                return title;
            }
            break;
        case 'v':
            if (fieldName % "version") {
                return version;
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
bool COAInfo::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
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
        case 't':
            if (fieldName % "title") {
                title = fieldValue;
                return true;
            }
            break;
        case 'v':
            if (fieldName % "version") {
                version = fieldValue;
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void COAInfo::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool COAInfo::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> description;
    archive >> title;
    archive >> version;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool COAInfo::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << description;
    archive << title;
    archive << version;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, COAInfoArray& array) {
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
CArchive& operator<<(CArchive& archive, const COAInfoArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void COAInfo::registerClass(void) {
    // only do this once
    if (HAS_FIELD(COAInfo, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(COAInfo, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(COAInfo, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(COAInfo, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(COAInfo, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(COAInfo, "description", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(COAInfo, "title", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(COAInfo, "version", T_TEXT | TS_OMITEMPTY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(COAInfo, "schema");
    HIDE_FIELD(COAInfo, "deleted");
    HIDE_FIELD(COAInfo, "showing");
    HIDE_FIELD(COAInfo, "cname");

    builtIns.push_back(_biCOAInfo);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextOainfoChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const COAInfo* oai = reinterpret_cast<const COAInfo*>(dataPtr);
    if (oai) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, oai);
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
bool COAInfo::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const COAInfo& oai) {
    oai.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, COAInfo& oai) {
    oai.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const COAInfo& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_OAINFO = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
