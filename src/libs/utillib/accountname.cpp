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
#include "accountname.h"
#include "options_base.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CAccountName, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextAccountnameChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextAccountnameChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CAccountName::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["accountname_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextAccountnameChunk, this);
}

//---------------------------------------------------------------------------
string_q nextAccountnameChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CAccountName*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CAccountName::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextAccountnameChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    if (fieldName % "decimals" && decimals == 0)
        return "";
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "address") {
                return addr_2_Str(address);
            }
            break;
        case 'd':
            if (fieldName % "decimals") {
                return decimals == 0 ? "" : uint_2_Str(decimals);
            }
            if (fieldName % "description") {
                return description;
            }
            break;
        case 'f':
            if (fieldName % "firstAppearance") {
                return uint_2_Str(firstAppearance);
            }
            break;
        case 'i':
            if (fieldName % "is_custom") {
                return bool_2_Str(is_custom);
            }
            if (fieldName % "is_prefund") {
                return bool_2_Str(is_prefund);
            }
            if (fieldName % "is_contract") {
                return bool_2_Str(is_contract);
            }
            if (fieldName % "is_erc20") {
                return bool_2_Str(is_erc20);
            }
            if (fieldName % "is_erc721") {
                return bool_2_Str(is_erc721);
            }
            break;
        case 'l':
            if (fieldName % "lastExport") {
                return uint_2_Str(lastExport);
            }
            if (fieldName % "latestAppearance") {
                return uint_2_Str(latestAppearance);
            }
            break;
        case 'n':
            if (fieldName % "name") {
                return name;
            }
            if (fieldName % "nAppearances") {
                return uint_2_Str(nAppearances);
            }
            break;
        case 'p':
            if (fieldName % "path") {
                return path;
            }
            break;
        case 's':
            if (fieldName % "symbol") {
                return symbol;
            }
            if (fieldName % "source") {
                return source;
            }
            if (fieldName % "sizeInBytes") {
                return sizeInBytes == 0 ? "" : uint_2_Str(sizeInBytes);
            }
            break;
        case 't':
            if (fieldName % "tags") {
                return tags;
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
bool CAccountName::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    if (fieldName % "address")
        fieldValue = toLower(fieldValue);
    if (CBaseNode::setValueByName(fieldNameIn, fieldValueIn))
        return true;
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "address") {
                address = str_2_Addr(fieldValue);
                return true;
            }
            break;
        case 'd':
            if (fieldName % "decimals") {
                decimals = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "description") {
                description = fieldValue;
                return true;
            }
            break;
        case 'f':
            if (fieldName % "firstAppearance") {
                firstAppearance = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'i':
            if (fieldName % "is_custom") {
                is_custom = str_2_Bool(fieldValue);
                return true;
            }
            if (fieldName % "is_prefund") {
                is_prefund = str_2_Bool(fieldValue);
                return true;
            }
            if (fieldName % "is_contract") {
                is_contract = str_2_Bool(fieldValue);
                return true;
            }
            if (fieldName % "is_erc20") {
                is_erc20 = str_2_Bool(fieldValue);
                return true;
            }
            if (fieldName % "is_erc721") {
                is_erc721 = str_2_Bool(fieldValue);
                return true;
            }
            break;
        case 'l':
            if (fieldName % "lastExport") {
                lastExport = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "latestAppearance") {
                latestAppearance = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'n':
            if (fieldName % "name") {
                name = fieldValue;
                return true;
            }
            if (fieldName % "nAppearances") {
                nAppearances = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'p':
            if (fieldName % "path") {
                path = fieldValue;
                return true;
            }
            break;
        case 's':
            if (fieldName % "symbol") {
                symbol = fieldValue;
                return true;
            }
            if (fieldName % "source") {
                source = fieldValue;
                return true;
            }
            if (fieldName % "sizeInBytes") {
                sizeInBytes = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 't':
            if (fieldName % "tags") {
                tags = fieldValue;
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CAccountName::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CAccountName::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> tags;
    archive >> address;
    archive >> name;
    archive >> symbol;
    archive >> source;
    archive >> decimals;
    archive >> description;
    archive >> is_custom;
    archive >> is_prefund;
    archive >> is_contract;
    archive >> is_erc20;
    archive >> is_erc721;
    // archive >> nAppearances;
    // archive >> lastExport;
    // archive >> firstAppearance;
    // archive >> latestAppearance;
    // archive >> path;
    // archive >> sizeInBytes;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CAccountName::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << tags;
    archive << address;
    archive << name;
    archive << symbol;
    archive << source;
    archive << decimals;
    archive << description;
    archive << is_custom;
    archive << is_prefund;
    archive << is_contract;
    archive << is_erc20;
    archive << is_erc721;
    // archive << nAppearances;
    // archive << lastExport;
    // archive << firstAppearance;
    // archive << latestAppearance;
    // archive << path;
    // archive << sizeInBytes;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CAccountNameArray& array) {
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
CArchive& operator<<(CArchive& archive, const CAccountNameArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CAccountName::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CAccountName, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CAccountName, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CAccountName, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CAccountName, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CAccountName, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CAccountName, "tags", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CAccountName, "address", T_ADDRESS | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CAccountName, "name", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CAccountName, "symbol", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CAccountName, "source", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CAccountName, "decimals", T_UNUMBER | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CAccountName, "description", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CAccountName, "is_custom", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CAccountName, "is_prefund", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CAccountName, "is_contract", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CAccountName, "is_erc20", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CAccountName, "is_erc721", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CAccountName, "nAppearances", T_BLOCKNUM, ++fieldNum);
    HIDE_FIELD(CAccountName, "nAppearances");
    ADD_FIELD(CAccountName, "lastExport", T_BLOCKNUM, ++fieldNum);
    HIDE_FIELD(CAccountName, "lastExport");
    ADD_FIELD(CAccountName, "firstAppearance", T_BLOCKNUM, ++fieldNum);
    HIDE_FIELD(CAccountName, "firstAppearance");
    ADD_FIELD(CAccountName, "latestAppearance", T_BLOCKNUM, ++fieldNum);
    HIDE_FIELD(CAccountName, "latestAppearance");
    ADD_FIELD(CAccountName, "path", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CAccountName, "path");
    ADD_FIELD(CAccountName, "sizeInBytes", T_UNUMBER | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CAccountName, "sizeInBytes");

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CAccountName, "schema");
    HIDE_FIELD(CAccountName, "deleted");
    HIDE_FIELD(CAccountName, "showing");
    HIDE_FIELD(CAccountName, "cname");

    builtIns.push_back(_biCAccountName);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextAccountnameChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CAccountName* acc = reinterpret_cast<const CAccountName*>(dataPtr);
    if (acc) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'n':
                if (fieldIn % "name") {
                    string_q ret = substitute(acc->name, "\"", "");
                    if (isTestMode() && (acc->is_custom || contains(acc->tags, "Friends"))) {
                        ret = "Name " + acc->address.substr(0, 10);
                    }
                    return ret;
                }
                break;
            case 's':
                if (fieldIn % "source")
                    return substitute(acc->source, "\"", "");
                break;
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, acc);
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
bool CAccountName::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    if (m_schema <= getVersionNum(0, 6, 5)) {
        string_q subtags, unused5;
        bool unused2, unused3, unused4;
        archive >> tags;
        archive >> subtags;  // subtags has been removed and added to tags with ':' separator
        archive >> name;
        archive >> address;
        archive >> symbol;
        archive >> description;
        archive >> source;
        archive >> unused5;  // used to be logo
        // archive >> path;
        // archive >> color;
        archive >> unused2;  // used to be is_contract;
        archive >> unused3;  // used to be is_private;
        archive >> unused4;  // used to be is_shared;
        // archive >> firstAppearance;
        // archive >> latestAppearance;
        // archive >> lastExport;
        // archive >> nRecords;
        // archive >> sizeInBytes;
        if (!subtags.empty())
            tags += (":" + subtags);
        finishParse();
        done = true;
    }
    // EXISTING_CODE
    return done;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CAccountName& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_ACCOUNTNAME =
    "[{TAGS}]\t"
    "[{ADDRESS}]\t"
    "[{NAME}]\t"
    "[{SYMBOL}]\t"
    "[{SOURCE}]\t"
    "[{DECIMALS}]\t"
    "[{DESCRIPTION}]\t"
    "[{DELETED}]\t"
    "[{IS_CUSTOM}]\t"
    "[{IS_PREFUND}]\t"
    "[{IS_CONTRACT}]\t"
    "[{IS_ERC20}]\t"
    "[{IS_ERC721}]";

//---------------------------------------------------------------------------
// EXISTING_CODE
//---------------------------------------------------------------------------
extern CArchive& operator>>(CArchive& archive, CAccountNameMap& nameMap) {
    uint64_t count;
    archive >> count;
    for (size_t i = 0; i < count; i++) {
        ASSERT(i < array.capacity());
        CAccountName item;
        item.Serialize(archive);
        nameMap[item.address] = item;
    }
    return archive;
}

//---------------------------------------------------------------------------
extern CArchive& operator<<(CArchive& archive, const CAccountNameMap& nameMap) {
    uint64_t count = nameMap.size();
    archive << count;
    for (auto item : nameMap) {
        item.second.SerializeC(archive);
    }
    return archive;
}
// EXISTING_CODE
}  // namespace qblocks
