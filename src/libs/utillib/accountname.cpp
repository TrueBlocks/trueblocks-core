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
#include "accountname.h"
#include "options_base.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CAccountName, CBaseNode);

//---------------------------------------------------------------------------
extern string_q nextAccountnameChunk(const string_q& fieldIn, const void* dataPtr);
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
                return addr_2_Str(address.substr(0, 42));
            }
            break;
        case 'd':
            if (fieldName % "decimals") {
                return decimals == 0 ? "" : uint_2_Str(decimals);
            }
            if (fieldName % "description") {
                return description.substr(0, 255);
            }
            break;
        case 'i':
            if (fieldName % "isCustom") {
                return bool_2_Str(isCustom);
            }
            if (fieldName % "isPrefund") {
                return bool_2_Str(isPrefund);
            }
            if (fieldName % "isContract") {
                return bool_2_Str(isContract);
            }
            if (fieldName % "isErc20") {
                return bool_2_Str(isErc20);
            }
            if (fieldName % "isErc721") {
                return bool_2_Str(isErc721);
            }
            break;
        case 'n':
            if (fieldName % "name") {
                return name.substr(0, 120);
            }
            break;
        case 's':
            if (fieldName % "symbol") {
                return symbol.substr(0, 30);
            }
            if (fieldName % "source") {
                return source.substr(0, 180);
            }
            break;
        case 't':
            if (fieldName % "tags") {
                return tags.substr(0, 30);
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
    if (CBaseNode::setValueByName(fieldNameIn, fieldValueIn))
        return true;
    if (fieldName % "address")
        fieldValue = toLower(fieldValue);
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "address") {
                address = str_2_Addr(fieldValue.substr(0, 42));
                return true;
            }
            break;
        case 'd':
            if (fieldName % "decimals") {
                decimals = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "description") {
                description = fieldValue.substr(0, 255);
                return true;
            }
            break;
        case 'i':
            if (fieldName % "isCustom") {
                isCustom = str_2_Bool(fieldValue);
                return true;
            }
            if (fieldName % "isPrefund") {
                isPrefund = str_2_Bool(fieldValue);
                return true;
            }
            if (fieldName % "isContract") {
                isContract = str_2_Bool(fieldValue);
                return true;
            }
            if (fieldName % "isErc20") {
                isErc20 = str_2_Bool(fieldValue);
                return true;
            }
            if (fieldName % "isErc721") {
                isErc721 = str_2_Bool(fieldValue);
                return true;
            }
            break;
        case 'n':
            if (fieldName % "name") {
                name = fieldValue.substr(0, 120);
                return true;
            }
            break;
        case 's':
            if (fieldName % "symbol") {
                symbol = fieldValue.substr(0, 30);
                return true;
            }
            if (fieldName % "source") {
                source = fieldValue.substr(0, 180);
                return true;
            }
            break;
        case 't':
            if (fieldName % "tags") {
                tags = fieldValue.substr(0, 30);
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
    archive >> isCustom;
    archive >> isPrefund;
    archive >> isContract;
    archive >> isErc20;
    archive >> isErc721;
    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CAccountName::SerializeC(CArchive& archive) const {
    // Writing always writes the latest version of the data
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
    archive << isCustom;
    archive << isPrefund;
    archive << isContract;
    archive << isErc20;
    archive << isErc721;
    // EXISTING_CODE
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CAccountName::Migrate(CArchive& archiveIn, CArchive& archiveOut) const {
    ASSERT(archiveIn.isReading());
    ASSERT(archiveOut.isWriting());
    CAccountName copy;
    // EXISTING_CODE
    // EXISTING_CODE
    copy.Serialize(archiveIn);
    copy.SerializeC(archiveOut);
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
    ADD_FIELD(CAccountName, "isCustom", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CAccountName, "isPrefund", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CAccountName, "isContract", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CAccountName, "isErc20", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CAccountName, "isErc721", T_BOOL | TS_OMITEMPTY, ++fieldNum);

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
                    if (isTestMode() && (acc->isCustom || contains(acc->tags, "Individuals"))) {
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

// EXISTING_CODE
// EXISTING_CODE

//---------------------------------------------------------------------------
bool CAccountName::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    if (m_schema < getVersionNum(0, 6, 6)) {
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
        archive >> unused2;  // used to be isContract;
        archive >> unused3;  // used to be isPrivate;
        archive >> unused4;  // used to be isShared;
        // archive >> first Appearance;
        // archive >> latest Appearance;
        // archive >> last Export;
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

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CAccountName& acc) {
    acc.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CAccountName& acc) {
    acc.Serialize(archive);
    return archive;
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
    "[{ISCUSTOM}]\t"
    "[{ISPREFUND}]\t"
    "[{ISCONTRACT}]\t"
    "[{ISERC20}]\t"
    "[{ISERC721}]";

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
