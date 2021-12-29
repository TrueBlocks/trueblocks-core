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
#include "apikey.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CApiKey, CBaseNode);

//---------------------------------------------------------------------------
extern string_q nextApikeyChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextApikeyChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CApiKey::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["apikey_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextApikeyChunk, this);
}

//---------------------------------------------------------------------------
string_q nextApikeyChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CApiKey*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CApiKey::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextApikeyChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'k':
            if (fieldName % "key") {
                return key;
            }
            break;
        case 's':
            if (fieldName % "secret") {
                return secret;
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
bool CApiKey::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'k':
            if (fieldName % "key") {
                key = fieldValue;
                return true;
            }
            break;
        case 's':
            if (fieldName % "secret") {
                secret = fieldValue;
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CApiKey::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CApiKey::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> key;
    archive >> secret;
    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CApiKey::SerializeC(CArchive& archive) const {
    // Writing always writes the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << key;
    archive << secret;
    // EXISTING_CODE
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CApiKey::Migrate(CArchive& archiveIn, CArchive& archiveOut) const {
    ASSERT(archiveIn.isReading());
    ASSERT(archiveOut.isWriting());
    CApiKey copy;
    // EXISTING_CODE
    // EXISTING_CODE
    copy.Serialize(archiveIn);
    copy.SerializeC(archiveOut);
    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CApiKeyArray& array) {
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
CArchive& operator<<(CArchive& archive, const CApiKeyArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CApiKey::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CApiKey, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CApiKey, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CApiKey, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CApiKey, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CApiKey, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CApiKey, "key", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CApiKey, "secret", T_TEXT | TS_OMITEMPTY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CApiKey, "schema");
    HIDE_FIELD(CApiKey, "deleted");
    HIDE_FIELD(CApiKey, "showing");
    HIDE_FIELD(CApiKey, "cname");

    builtIns.push_back(_biCApiKey);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextApikeyChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CApiKey* api = reinterpret_cast<const CApiKey*>(dataPtr);
    if (api) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, api);
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
bool CApiKey::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CApiKey& api) {
    api.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CApiKey& api) {
    api.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CApiKey& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_APIKEY =
    "[{KEY}]\t"
    "[{SECRET}]";

//---------------------------------------------------------------------------
// EXISTING_CODE
//--------------------------------------------------------------------------------
string_q getApiKey(const string_q& apiName, const string_q& signup) {
    string_q key = getGlobalConfig("")->getConfigStr("settings", toLower(apiName) + "_key", "<not_set>");
    if (!key.empty() && key != "<not_set>")
        return key;

    char buffer[256];
    bzero(buffer, sizeof(buffer));

    const char* STR_ERROR_NOKEY = "Articulation requires an api_key from `[APINAME]`. See `[SIGNUP]`.";
    errorMessage(substitute(substitute(STR_ERROR_NOKEY, "[APINAME]", apiName), "[SIGNUP]", signup));
    quickQuitHandler(0);

    return "";
}

//--------------------------------------------------------------------------------
bool getApiKey(CApiKey& lic) {
    lic.key = getGlobalConfig("blockScrape")->getConfigStr("settings", "pinata_api_key", "<not_set>");
    lic.secret = getGlobalConfig("blockScrape")->getConfigStr("settings", "pinata_secret_api_key", "<not_set>");
    if (isTestMode())
        lic.key = lic.secret = "--license_codes--";
    return (lic.key != "<not_set>" && lic.secret != "<not_set>");
}
// EXISTING_CODE
}  // namespace qblocks
