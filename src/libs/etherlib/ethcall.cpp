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
#include "ethcall.h"
#include "etherlib.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CEthCall, CEthState);

//---------------------------------------------------------------------------
static string_q nextEthcallChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextEthcallChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CEthCall::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["ethcall_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextEthcallChunk, this);
}

//---------------------------------------------------------------------------
string_q nextEthcallChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CEthCall*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CEthCall::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextEthcallChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "abi_spec") {
                if (abi_spec == CAbi())
                    return "{}";
                return abi_spec.Format();
            }
            break;
        case 'b':
            if (fieldName % "blockNumber") {
                return uint_2_Str(blockNumber);
            }
            if (fieldName % "bytes") {
                return bytes;
            }
            break;
        case 'e':
            if (fieldName % "encoding") {
                return encoding;
            }
            break;
        case 'r':
            if (fieldName % "result") {
                if (result == CFunction())
                    return "{}";
                return result.Format();
            }
            break;
        default:
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    string_q s;
    s = toUpper(string_q("abi_spec")) + "::";
    if (contains(fieldName, s)) {
        string_q f = fieldName;
        replaceAll(f, s, "");
        f = abi_spec.getValueByName(f);
        return f;
    }

    s = toUpper(string_q("result")) + "::";
    if (contains(fieldName, s)) {
        string_q f = fieldName;
        replaceAll(f, s, "");
        f = result.getValueByName(f);
        return f;
    }

    // Finally, give the parent class a chance
    return CEthState::getValueByName(fieldName);
}

//---------------------------------------------------------------------------------------------------
bool CEthCall::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    if (CEthState::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "abi_spec") {
                return abi_spec.parseJson3(fieldValue);
            }
            break;
        case 'b':
            if (fieldName % "blockNumber") {
                blockNumber = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "bytes") {
                bytes = fieldValue;
                return true;
            }
            break;
        case 'e':
            if (fieldName % "encoding") {
                encoding = fieldValue;
                return true;
            }
            break;
        case 'r':
            if (fieldName % "result") {
                return result.parseJson3(fieldValue);
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CEthCall::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CEthCall::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CEthState::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> blockNumber;
    archive >> encoding;
    archive >> bytes;
    archive >> abi_spec;
    archive >> result;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CEthCall::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CEthState::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << blockNumber;
    archive << encoding;
    archive << bytes;
    archive << abi_spec;
    archive << result;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CEthCallArray& array) {
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
CArchive& operator<<(CArchive& archive, const CEthCallArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CEthCall::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CEthCall, "schema"))
        return;

    CEthState::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(CEthCall, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CEthCall, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CEthCall, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CEthCall, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CEthCall, "blockNumber", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CEthCall, "encoding", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CEthCall, "bytes", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_OBJECT(CEthCall, "abi_spec", T_OBJECT | TS_OMITEMPTY, ++fieldNum, GETRUNTIME_CLASS(CAbi));
    ADD_OBJECT(CEthCall, "result", T_OBJECT | TS_OMITEMPTY, ++fieldNum, GETRUNTIME_CLASS(CFunction));

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CEthCall, "schema");
    HIDE_FIELD(CEthCall, "deleted");
    HIDE_FIELD(CEthCall, "showing");
    HIDE_FIELD(CEthCall, "cname");

    builtIns.push_back(_biCEthCall);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextEthcallChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CEthCall* eth = reinterpret_cast<const CEthCall*>(dataPtr);
    if (eth) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, eth);
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
bool CEthCall::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CEthCall& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode* CEthCall::getObjectAt(const string_q& fieldName, size_t index) const {
    if (fieldName % "abi_spec")
        return &abi_spec;

    if (fieldName % "result")
        return &result;

    return NULL;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_ETHCALL =
    "[{BLOCKNUMBER}]\t"
    "[{ADDRESS}]\t"
    "[{BALANCE}]\t"
    "[{NONCE}]\t"
    "[{CODE}]\t"
    "[{STORAGE}]\t"
    "[{DEPLOYED}]\t"
    "[{ACCTTYPE}]";

//---------------------------------------------------------------------------
// EXISTING_CODE
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
bool doEthCall(const address_t& to, const string_q& encoding, const string_q& bytes, blknum_t blockNum, const CAbi& abi,
               CFunction& result) {
    ostringstream cmd;
    cmd << "[{";
    cmd << "\"to\": \"" << to << "\", ";
    cmd << "\"data\": \"" << encoding << substitute(bytes, "0x", "") << "\"";
    cmd << "}, \"" << uint_2_Hex(blockNum) << "\"]";

    string_q rpcRet = callRPC("eth_call", cmd.str(), false);
    if (startsWith(rpcRet, "0x"))
        abi.articulateOutputs(encoding, rpcRet, result);
    return result.outputs.size();
}

bool doEthCall(CEthCall& call) {
    return doEthCall(call.address, call.encoding, call.bytes, call.blockNumber, call.abi_spec, call.result);
}
// EXISTING_CODE
}  // namespace qblocks
