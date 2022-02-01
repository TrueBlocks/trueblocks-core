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
#include "configenv.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CConfigEnv, CBaseNode);

//---------------------------------------------------------------------------
extern string_q nextConfigenvChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextConfigenvChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CConfigEnv::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["configenv_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextConfigenvChunk, this);
}

//---------------------------------------------------------------------------
string_q nextConfigenvChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CConfigEnv*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CConfigEnv::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextConfigenvChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'c':
            if (fieldName % "chain") {
                return chain;
            }
            if (fieldName % "configPath") {
                return configPath;
            }
            if (fieldName % "chainConfigPath") {
                return chainConfigPath;
            }
            if (fieldName % "cachePath") {
                return cachePath;
            }
            break;
        case 'd':
            if (fieldName % "defChain") {
                return defChain;
            }
            break;
        case 'i':
            if (fieldName % "indexPath") {
                return indexPath;
            }
            break;
        case 'r':
            if (fieldName % "rpcProvider") {
                return rpcProvider;
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
bool CConfigEnv::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'c':
            if (fieldName % "chain") {
                chain = fieldValue;
                return true;
            }
            if (fieldName % "configPath") {
                configPath = fieldValue;
                return true;
            }
            if (fieldName % "chainConfigPath") {
                chainConfigPath = fieldValue;
                return true;
            }
            if (fieldName % "cachePath") {
                cachePath = fieldValue;
                return true;
            }
            break;
        case 'd':
            if (fieldName % "defChain") {
                defChain = fieldValue;
                return true;
            }
            break;
        case 'i':
            if (fieldName % "indexPath") {
                indexPath = fieldValue;
                return true;
            }
            break;
        case 'r':
            if (fieldName % "rpcProvider") {
                rpcProvider = fieldValue;
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CConfigEnv::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CConfigEnv::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> chain;
    archive >> configPath;
    archive >> chainConfigPath;
    archive >> cachePath;
    archive >> indexPath;
    archive >> defChain;
    archive >> rpcProvider;
    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CConfigEnv::SerializeC(CArchive& archive) const {
    // Writing always writes the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << chain;
    archive << configPath;
    archive << chainConfigPath;
    archive << cachePath;
    archive << indexPath;
    archive << defChain;
    archive << rpcProvider;
    // EXISTING_CODE
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CConfigEnv::Migrate(CArchive& archiveIn, CArchive& archiveOut) const {
    ASSERT(archiveIn.isReading());
    ASSERT(archiveOut.isWriting());
    CConfigEnv copy;
    // EXISTING_CODE
    // EXISTING_CODE
    copy.Serialize(archiveIn);
    copy.SerializeC(archiveOut);
    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CConfigEnvArray& array) {
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
CArchive& operator<<(CArchive& archive, const CConfigEnvArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CConfigEnv::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CConfigEnv, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CConfigEnv, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CConfigEnv, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CConfigEnv, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CConfigEnv, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CConfigEnv, "chain", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CConfigEnv, "configPath", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CConfigEnv, "chainConfigPath", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CConfigEnv, "cachePath", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CConfigEnv, "indexPath", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CConfigEnv, "defChain", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CConfigEnv, "rpcProvider", T_TEXT | TS_OMITEMPTY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CConfigEnv, "schema");
    HIDE_FIELD(CConfigEnv, "deleted");
    HIDE_FIELD(CConfigEnv, "showing");
    HIDE_FIELD(CConfigEnv, "cname");

    builtIns.push_back(_biCConfigEnv);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextConfigenvChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CConfigEnv* con = reinterpret_cast<const CConfigEnv*>(dataPtr);
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

// EXISTING_CODE
// EXISTING_CODE

//---------------------------------------------------------------------------
bool CConfigEnv::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CConfigEnv& con) {
    con.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CConfigEnv& con) {
    con.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CConfigEnv& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_CONFIGENV =
    "[{CHAIN}]\t"
    "[{CONFIGPATH}]\t"
    "[{CHAINCONFIGPATH}]\t"
    "[{CACHEPATH}]\t"
    "[{INDEXPATH}]\t"
    "[{DEFCHAIN}]\t"
    "[{RPCPROVIDER}]";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
