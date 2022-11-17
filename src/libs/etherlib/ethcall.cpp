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
#include "ethcall.h"
#include "etherlib.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CEthCall, CBaseNode);

//---------------------------------------------------------------------------
extern string_q nextEthcallChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextEthcallChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CEthCall::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["ethcall_fmt"] : fmtIn);
    if (fmt.empty()) {
        toJson(ctx);
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
            if (fieldName % "address") {
                return addr_2_Str(address);
            }
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
        case 'c':
            if (fieldName % "callResult") {
                if (callResult == CFunction())
                    return "{}";
                return callResult.Format();
            }
            if (fieldName % "compressedResult") {
                return compressedResult;
            }
            break;
        case 'd':
            if (fieldName % "deployed") {
                return uint_2_Str(deployed);
            }
            break;
        case 'e':
            if (fieldName % "encoding") {
                return encoding;
            }
            break;
        case 's':
            if (fieldName % "signature") {
                return signature;
            }
            break;
        default:
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // test for contained object field specifiers
    string_q objSpec;
    objSpec = toUpper("abi_spec") + "::";
    if (contains(fieldName, objSpec))
        return abi_spec.getValueByName(substitute(fieldName, objSpec, ""));

    objSpec = toUpper("callResult") + "::";
    if (contains(fieldName, objSpec))
        return callResult.getValueByName(substitute(fieldName, objSpec, ""));

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//---------------------------------------------------------------------------------------------------
bool CEthCall::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "address") {
                address = str_2_Addr(fieldValue);
                return true;
            }
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
        case 'c':
            if (fieldName % "callResult") {
                return callResult.parseJson3(fieldValue);
            }
            if (fieldName % "compressedResult") {
                compressedResult = fieldValue;
                return true;
            }
            break;
        case 'd':
            if (fieldName % "deployed") {
                deployed = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'e':
            if (fieldName % "encoding") {
                encoding = fieldValue;
                return true;
            }
            break;
        case 's':
            if (fieldName % "signature") {
                signature = fieldValue;
                return true;
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
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    // archive >> blockNumber;
    // archive >> address;
    // archive >> signature;
    archive >> encoding;
    archive >> bytes;
    archive >> abi_spec;
    // archive >> callResult;
    // archive >> compressedResult;
    // archive >> deployed;
    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CEthCall::SerializeC(CArchive& archive) const {
    // Writing always writes the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    // archive << blockNumber;
    // archive << address;
    // archive << signature;
    archive << encoding;
    archive << bytes;
    archive << abi_spec;
    // archive << callResult;
    // archive << compressedResult;
    // archive << deployed;
    // EXISTING_CODE
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CEthCall::Migrate(CArchive& archiveIn, CArchive& archiveOut) const {
    ASSERT(archiveIn.isReading());
    ASSERT(archiveOut.isWriting());
    CEthCall copy;
    // EXISTING_CODE
    // EXISTING_CODE
    copy.Serialize(archiveIn);
    copy.SerializeC(archiveOut);
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

    size_t fieldNum = 1000;
    ADD_FIELD(CEthCall, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CEthCall, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CEthCall, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CEthCall, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CEthCall, "blockNumber", T_BLOCKNUM, ++fieldNum);
    HIDE_FIELD(CEthCall, "blockNumber");
    ADD_FIELD(CEthCall, "address", T_ADDRESS | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CEthCall, "address");
    ADD_FIELD(CEthCall, "signature", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CEthCall, "signature");
    ADD_FIELD(CEthCall, "encoding", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CEthCall, "bytes", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_OBJECT(CEthCall, "abi_spec", T_OBJECT | TS_OMITEMPTY, ++fieldNum, GETRUNTIME_CLASS(CAbi));
    ADD_OBJECT(CEthCall, "callResult", T_OBJECT | TS_OMITEMPTY, ++fieldNum, GETRUNTIME_CLASS(CFunction));
    HIDE_FIELD(CEthCall, "callResult");
    ADD_FIELD(CEthCall, "compressedResult", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CEthCall, "compressedResult");
    ADD_FIELD(CEthCall, "deployed", T_BLOCKNUM, ++fieldNum);
    HIDE_FIELD(CEthCall, "deployed");

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
            case 'c':
                if (fieldIn % "compressedResult") {
                    ostringstream os;
                    bool first = true;
                    for (auto result : eth->callResult.outputs) {
                        if (!first)
                            os << "|";
                        os << result.value;
                        first = false;
                    }
                    return os.str();
                }
                break;
            case 's':
                if (fieldIn % "signature") {
                    return eth->callResult.signature;
                }
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

// EXISTING_CODE
// EXISTING_CODE

//---------------------------------------------------------------------------
bool CEthCall::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CEthCall& eth) {
    eth.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CEthCall& eth) {
    eth.Serialize(archive);
    return archive;
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
    // EXISTING_CODE
    // EXISTING_CODE
    if (fieldName % "abi_spec")
        return &abi_spec;
    if (fieldName % "callResult")
        return &callResult;
    // EXISTING_CODE
    // EXISTING_CODE

    return NULL;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_ETHCALL =
    "[{BLOCKNUMBER}]\t"
    "[{ADDRESS}]\t"
    "[{SIGNATURE}]\t"
    "[{ENCODING}]\t"
    "[{BYTES}]\t"
    "[{COMPRESSEDRESULT}]";

//---------------------------------------------------------------------------
// EXISTING_CODE
//-------------------------------------------------------------------------
string_q CEthCall::getCallResult(void) const {
    return callResult.outputs.size() ? callResult.outputs[0].value : "";
}

//-------------------------------------------------------------------------
bool CEthCall::getCallResult(string_q& out) const {
    if (callResult.outputs.size()) {
        out = callResult.outputs[0].value;
        return true;
    }
    return false;
}

//-------------------------------------------------------------------------
bool CEthCall::getCallResult(CStringArray& out) const {
    for (auto output : callResult.outputs)
        out.push_back(output.value);
    return out.size();
}

//-------------------------------------------------------------------------
bool doEthCall(CEthCall& theCall, bool checkProxy) {
    if (theCall.deployed != NOPOS && theCall.deployed > theCall.blockNumber) {
        if (isTestMode()) {
            LOG_INFO(theCall.Format(
                "Calling a contract ([{ADDRESS}]) at block [{BLOCKNUMBER}] prior to its deployment [{DEPLOYED}]"));
        }
        return false;
    }

    string_q orig = theCall.encoding;
    blknum_t o = theCall.blockNumber;
    if (isTestMode()) {
        theCall.blockNumber = min(blknum_t(15000000), theCall.blockNumber);
        LOG_INFO("Calling ", theCall.address, " at block ", theCall.blockNumber, "...: ", getEnvStr("SHIT"));
    }

    ostringstream cmd;
    cmd << "[";
    cmd << "{";
    cmd << "\"to\": \"" << theCall.address << "\", ";
    cmd << "\"data\": \"" << theCall.encoding << substitute(theCall.bytes, "0x", "") << "\"";
    cmd << "}, \"" << uint_2_Hex(theCall.blockNumber) << "\"";
    cmd << "]";

    string_q ret = callRPC("eth_call", cmd.str(), false);
    if (startsWith(ret, "0x")) {
        if (isTestMode()) {
            LOG_INFO("call to ", theCall.address, " at block ", theCall.blockNumber, " at four-byte ", theCall.encoding,
                     " returned ", ret);
        }
        theCall.encoding = orig;
        theCall.blockNumber = o;
        theCall.abi_spec.articulateOutputs(theCall.encoding.substr(0, 10), ret, theCall.callResult);
        return true;
    }

    if (checkProxy) {
        address_t proxy;
        if (getProxyContract(theCall.address, theCall.blockNumber, proxy)) {
            if (isTestMode()) {
                LOG_INFO("proxy for ", theCall.address, " found at ", proxy, ". Calling into proxy.");
                theCall.address = proxy;
                return doEthCall(theCall, checkProxy);
            }
        }
    }

    if (isTestMode()) {
        LOG_INFO("call to ", theCall.address, " at block ", theCall.blockNumber, " at four-byte ", theCall.encoding,
                 " returned ", ret);
    }

    theCall.encoding = orig;
    theCall.blockNumber = o;
    return false;
}

extern string_q getPC_internal(const address_t& contract, blknum_t blockNum);
//-------------------------------------------------------------------------------------
bool getProxyContract(const address_t& contract, blknum_t blockNum, address_t& proxy) {
    proxy = getPC_internal(contract, blockNum);
    return !proxy.empty();
}

//-----------------------------------------------------------------------
string_q getPC_internal(const address_t& contract, blknum_t blockNum) {
    // We try the following two not-so-useful methods of calling the implementation function directly
    CEthCall theCall;
    theCall.address = contract;
    theCall.encoding = "0x59679b0f";  // implementation()
    theCall.bytes = "";
    theCall.blockNumber = blockNum;
    if (doEthCall(theCall, false /* proxy */)) {
        string_q result = theCall.getCallResult();
        biguint_t test = str_2_Wei(result);
        address_t proxy;
        if (isPotentialAddr(test, proxy)) {
            if (proxy != contract && isAddress(proxy) && !isZeroAddr(proxy)) {
                if (isTestMode()) {
                    LOG_INFO("getPC_internal: query implemenation() on contract ", contract, " block ", blockNum,
                             " returned: ", proxy);
                }
                return proxy;
            }
        }
    }

    // We check a bunch of different locations for the proxy
    CStringArray locations = {
        "0x360894a13ba1a3210667c828492db98dca3e2076cc3735a920a3ca505d382bbc",  // EIP1967
        "0x7050c9e0f4ca769c69bd3a8ef740bc37934f8e2c036e5a723fd8ee048ed3f8c3",  // EIP1967ZOS
        "0xc5f16f0fcc639fa48a6947836d9850f504798523bf8c9a3a87d5876cf622bcf7",  // EIP1822
        "0x5f3b5dfeb7b28cdbd7faba78963ee202a494e2a2cc8c9978d5e30d2aebb8c197",  // EIP1822ZOS};
        "0x",
    };

    for (auto loc : locations) {
        string_q result = getStorageAt(contract, loc, blockNum);
        biguint_t test = str_2_Wei(result);
        address_t proxy;
        if (isPotentialAddr(test, proxy)) {
            if (proxy != contract && isAddress(proxy) && !isZeroAddr(proxy)) {
                if (isTestMode()) {
                    LOG_INFO("getPC_internal: query for slot ", loc, " contract ", contract, " block ", blockNum,
                             " returned: ", proxy);
                }
                return proxy;
            }
        }
    }

    if (isTestMode()) {
        LOG_INFO("getPC_internal: eth_call using four-byte ", theCall.encoding, " contract ", contract, " block ",
                 blockNum, " returned: false");
    }

    return "";
}
// EXISTING_CODE
}  // namespace qblocks
