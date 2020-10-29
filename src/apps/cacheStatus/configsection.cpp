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
#include "configsection.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CConfigSection, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextConfigsectionChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextConfigsectionChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CConfigSection::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["configsection_fmt"] : fmtIn);
    if (fmt.empty()) {
        toJson(ctx);
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextConfigsectionChunk, this);
}

//---------------------------------------------------------------------------
string_q nextConfigsectionChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CConfigSection*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CConfigSection::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextConfigsectionChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'k':
            if (fieldName % "keys" || fieldName % "keysCnt") {
                size_t cnt = keys.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
                if (!cnt)
                    return "";
                string_q retS;
                for (size_t i = 0; i < cnt; i++) {
                    retS += keys[i].Format();
                    retS += ((i < cnt - 1) ? ",\n" : "\n");
                }
                return retS;
            }
            break;
        case 'n':
            if (fieldName % "name") {
                return name;
            }
            break;
        case 's':
            if (fieldName % "section") {
                return section;
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
bool CConfigSection::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'k':
            if (fieldName % "keys") {
                CConfigItem obj;
                string_q str = fieldValue;
                while (obj.parseJson3(str)) {
                    keys.push_back(obj);
                    obj = CConfigItem();  // reset
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
        case 's':
            if (fieldName % "section") {
                section = fieldValue;
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CConfigSection::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CConfigSection::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> section;
    archive >> name;
    archive >> keys;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CConfigSection::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << section;
    archive << name;
    archive << keys;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CConfigSectionArray& array) {
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
CArchive& operator<<(CArchive& archive, const CConfigSectionArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CConfigSection::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CConfigSection, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CConfigSection, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CConfigSection, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CConfigSection, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CConfigSection, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CConfigSection, "section", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CConfigSection, "name", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CConfigSection, "keys", T_OBJECT | TS_ARRAY | TS_OMITEMPTY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CConfigSection, "schema");
    HIDE_FIELD(CConfigSection, "deleted");
    HIDE_FIELD(CConfigSection, "showing");
    HIDE_FIELD(CConfigSection, "cname");

    builtIns.push_back(_biCConfigSection);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextConfigsectionChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CConfigSection* con = reinterpret_cast<const CConfigSection*>(dataPtr);
    if (con) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'k':
                if (fieldIn % "keys") {
                    for (auto key : con->keys) {
                        if (key.named.size() > 0) {
                            manageFields("CConfigItem:value", false);
                            manageFields("CConfigItem:named", true);
                        } else {
                            manageFields("CConfigItem:value", true);
                            manageFields("CConfigItem:named", false);
                        }
                    }
                }
                break;
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
bool CConfigSection::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CConfigSection& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode* CConfigSection::getObjectAt(const string_q& fieldName, size_t index) const {
    if (fieldName % "keys" && index < keys.size())
        return &keys[index];
    return NULL;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_CONFIGSECTION = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
