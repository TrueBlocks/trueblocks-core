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
#include "configfile.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CConfigFile, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextConfigfileChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextConfigfileChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CConfigFile::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["configfile_fmt"] : fmtIn);
    if (fmt.empty()) {
        ctx << toJson();
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextConfigfileChunk, this);
}

//---------------------------------------------------------------------------
string_q nextConfigfileChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CConfigFile*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CConfigFile::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextConfigfileChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'g':
            if (fieldName % "groups" || fieldName % "groupsCnt") {
                size_t cnt = groups.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
                if (!cnt)
                    return "";
                string_q retS;
                for (size_t i = 0; i < cnt; i++) {
                    retS += groups[i].Format();
                    retS += ((i < cnt - 1) ? ",\n" : "\n");
                }
                return retS;
            }
            break;
        case 'n':
            if (fieldName % "name")
                return name;
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//---------------------------------------------------------------------------------------------------
bool CConfigFile::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'g':
            if (fieldName % "groups") {
                CConfigGroup item;
                string_q str = fieldValue;
                while (item.parseJson3(str)) {
                    groups.push_back(item);
                    item = CConfigGroup();  // reset
                }
                return true;
            }
            break;
        case 'n':
            if (fieldName % "name") {
                name = fieldValue;
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CConfigFile::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CConfigFile::Serialize(CArchive& archive) {
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
    archive >> groups;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CConfigFile::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << name;
    archive << groups;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CConfigFileArray& array) {
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
CArchive& operator<<(CArchive& archive, const CConfigFileArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CConfigFile::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CConfigFile, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CConfigFile, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CConfigFile, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CConfigFile, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CConfigFile, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CConfigFile, "name", T_TEXT, ++fieldNum);
    ADD_FIELD(CConfigFile, "groups", T_OBJECT | TS_ARRAY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CConfigFile, "schema");
    HIDE_FIELD(CConfigFile, "deleted");
    HIDE_FIELD(CConfigFile, "showing");
    HIDE_FIELD(CConfigFile, "cname");

    builtIns.push_back(_biCConfigFile);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextConfigfileChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CConfigFile* con = reinterpret_cast<const CConfigFile*>(dataPtr);
    if (con) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, con);
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
bool CConfigFile::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CConfigFile& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode* CConfigFile::getObjectAt(const string_q& fieldName, size_t index) const {
    if (fieldName % "groups" && index < groups.size())
        return &groups[index];
    return NULL;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_CONFIGFILE = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
