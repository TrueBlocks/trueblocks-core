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
 *  the code inside of 'EXISTING_CODE' tags.
 */
#include "configuration.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CConfiguration, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextConfigurationChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextConfigurationChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CConfiguration::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["configuration_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextConfigurationChunk, this);
}

//---------------------------------------------------------------------------
string_q nextConfigurationChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CConfiguration*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CConfiguration::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextConfigurationChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'f':
            if (fieldName % "files" || fieldName % "filesCnt") {
                size_t cnt = files.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
                if (!cnt)
                    return "";
                string_q retS;
                for (size_t i = 0; i < cnt; i++) {
                    retS += files[i].Format();
                    retS += ((i < cnt - 1) ? ",\n" : "\n");
                }
                return retS;
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
bool CConfiguration::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'f':
            if (fieldName % "files") {
                CConfigFile obj;
                string_q str = fieldValue;
                while (obj.parseJson3(str)) {
                    files.push_back(obj);
                    obj = CConfigFile();  // reset
                }
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CConfiguration::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CConfiguration::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> files;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CConfiguration::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << files;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CConfigurationArray& array) {
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
CArchive& operator<<(CArchive& archive, const CConfigurationArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CConfiguration::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CConfiguration, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CConfiguration, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CConfiguration, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CConfiguration, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CConfiguration, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CConfiguration, "files", T_OBJECT | TS_ARRAY | TS_OMITEMPTY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CConfiguration, "schema");
    HIDE_FIELD(CConfiguration, "deleted");
    HIDE_FIELD(CConfiguration, "showing");
    HIDE_FIELD(CConfiguration, "cname");

    builtIns.push_back(_biCConfiguration);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextConfigurationChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CConfiguration* con = reinterpret_cast<const CConfiguration*>(dataPtr);
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
bool CConfiguration::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CConfiguration& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode* CConfiguration::getObjectAt(const string_q& fieldName, size_t index) const {
    if (fieldName % "files") {
        if (index == NOPOS) {
            CConfigFile empty;
            ((CConfiguration*)this)->files.push_back(empty);  // NOLINT
            index = files.size() - 1;
        }
        if (index < files.size())
            return &files[index];
    }

    return NULL;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_CONFIGURATION = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
