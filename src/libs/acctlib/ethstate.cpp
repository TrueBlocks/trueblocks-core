/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
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
#include "ethstate.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CEthState, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextEthstateChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextEthstateChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CEthState::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
    if (!m_showing)
        return;

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["ethstate_fmt"] : fmtIn);
    if (fmt.empty()) {
        ctx << toJson();
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextEthstateChunk, this);
}

//---------------------------------------------------------------------------
string_q nextEthstateChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CEthState *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CEthState::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    if ( fieldName % "ether" ) {
        balance = str_2_Wei(fieldValue)* weiPerEther();
        return true;
    }
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "address" ) { address = str_2_Addr(fieldValue); return true; }
            if ( fieldName % "accttype" ) { accttype = fieldValue; return true; }
            break;
        case 'b':
            if ( fieldName % "blockNumber" ) { blockNumber = str_2_Uint(fieldValue); return true; }
            if ( fieldName % "balance" ) { balance = str_2_Wei(fieldValue); return true; }
            break;
        case 'c':
            if ( fieldName % "code" ) { code = fieldValue; return true; }
            break;
        case 'd':
            if ( fieldName % "deployed" ) { deployed = str_2_Uint(fieldValue); return true; }
            break;
        case 'n':
            if ( fieldName % "nonce" ) { nonce = str_2_Uint(fieldValue); return true; }
            break;
        case 's':
            if ( fieldName % "storage" ) { storage = fieldValue; return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CEthState::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CEthState::Serialize(CArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> blockNumber;
    archive >> balance;
//    archive >> nonce;
//    archive >> code;
//    archive >> storage;
//    archive >> address;
//    archive >> deployed;
//    archive >> accttype;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CEthState::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << blockNumber;
    archive << balance;
//    archive << nonce;
//    archive << code;
//    archive << storage;
//    archive << address;
//    archive << deployed;
//    archive << accttype;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CEthStateArray& array) {
    uint64_t count;
    archive >> count;
    array.resize(count);
    for (size_t i = 0 ; i < count ; i++) {
        ASSERT(i < array.capacity());
        array.at(i).Serialize(archive);
    }
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CEthStateArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CEthState::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CEthState, "schema")) return;

    size_t fieldNum = 1000;
    ADD_FIELD(CEthState, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CEthState, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CEthState, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CEthState, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(CEthState, "blockNumber", T_NUMBER, ++fieldNum);
    ADD_FIELD(CEthState, "balance", T_NUMBER, ++fieldNum);
    ADD_FIELD(CEthState, "nonce", T_NUMBER, ++fieldNum);
    HIDE_FIELD(CEthState, "nonce");
    ADD_FIELD(CEthState, "code", T_TEXT, ++fieldNum);
    HIDE_FIELD(CEthState, "code");
    ADD_FIELD(CEthState, "storage", T_TEXT, ++fieldNum);
    HIDE_FIELD(CEthState, "storage");
    ADD_FIELD(CEthState, "address", T_ADDRESS, ++fieldNum);
    HIDE_FIELD(CEthState, "address");
    ADD_FIELD(CEthState, "deployed", T_NUMBER, ++fieldNum);
    HIDE_FIELD(CEthState, "deployed");
    ADD_FIELD(CEthState, "accttype", T_TEXT, ++fieldNum);
    HIDE_FIELD(CEthState, "accttype");

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CEthState, "schema");
    HIDE_FIELD(CEthState, "deleted");
    HIDE_FIELD(CEthState, "showing");
    HIDE_FIELD(CEthState, "cname");

    builtIns.push_back(_biCEthState);

    // EXISTING_CODE
    ADD_FIELD(CEthState, "ether", T_NUMBER, ++fieldNum);
    HIDE_FIELD(CEthState, "ether");
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextEthstateChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CEthState *eth = reinterpret_cast<const CEthState *>(dataPtr);
    if (eth) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'd':
                if ( fieldIn % "dollars" )
                    return getDispBal(eth->blockNumber, eth->balance);
                break;
            case 'e':
                if ( fieldIn % "ether" )
                    return wei_2_Ether(bnu_2_Str(eth->balance));
                break;
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
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
bool CEthState::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
string_q CEthState::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextEthstateChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "address" ) return addr_2_Str(address);
            if ( fieldName % "accttype" ) return accttype;
            break;
        case 'b':
            if ( fieldName % "blockNumber" ) return uint_2_Str(blockNumber);
            if ( fieldName % "balance" ) return bnu_2_Str(balance);
            break;
        case 'c':
            if ( fieldName % "code" ) return code;
            break;
        case 'd':
            if ( fieldName % "deployed" ) return uint_2_Str(deployed);
            break;
        case 'n':
            if ( fieldName % "nonce" ) return uint_2_Str(nonce);
            break;
        case 's':
            if ( fieldName % "storage" ) return storage;
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CEthState& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_ETHSTATE = 
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
wei_t getBalanceAt(const string_q& addr, blknum_t num) {
    if (num == NOPOS)
        num = getLastBlock_client();
    string_q params = "[\"[{ADDRESS}]\",\"[{NUM}]\"]";
    replace(params, "[{ADDRESS}]", str_2_Addr(addr));
    replace(params, "[{NUM}]",  uint_2_Hex(num));
    return str_2_Wei(callRPC("eth_getBalance", params, false));
}

//-------------------------------------------------------------------------
bool nodeHasBalances(void) {
    // Account 0xa1e4380a3b1f749673e270229993ee55f35663b4 owned 2000000000000000000000 (2000 ether)
    // at block zero. If the node is holding balances (i.e. its an archive node), then it will
    // return that value for block 1 as well. Otherwise, it will return a zero balance.
    // NOTE: Unimportantly, account 0xa1e4380a3b1f749673e270229993ee55f35663b4 transacted in the first ever transaction.
    uint64_t save = verbose; // silence the warning from the node since we know this may fail
    verbose = 0;
    bool ret = getBalanceAt("0xa1e4380a3b1f749673e270229993ee55f35663b4", 1) == str_2_Wei("2000000000000000000000");
    verbose = save;
    return ret;
}

//-------------------------------------------------------------------------
string_q getCodeAt(const string_q& addr, blknum_t num) {
    if (num == NOPOS)
        num = getLastBlock_client();
    string_q params = "[\"[{ADDRESS}]\",\"[{NUM}]\"]";
    replace(params, "[{ADDRESS}]", str_2_Addr(addr));
    replace(params, "[{NUM}]",  uint_2_Hex(num));
    return callRPC("eth_getCode", params, false);
}

//-------------------------------------------------------------------------
bool isContractAt(const address_t& addr, blknum_t num) {
    return !substitute(getCodeAt(addr, num), "0x", "").empty();
}

//-------------------------------------------------------------------------
uint64_t getNonceAt(const address_t& addr, blknum_t num) {
    if (num == NOPOS)
        num = getLastBlock_client();
    string_q params = "[\"[{ADDRESS}]\",\"[{NUM}]\"]";
    replace(params, "[{ADDRESS}]", str_2_Addr(addr));
    replace(params, "[{NUM}]",  uint_2_Hex(num));
    return str_2_Uint(callRPC("eth_getTransactionCount", params, false));
}

//-------------------------------------------------------------------------
string_q getStorageAt(const string_q& addr, uint64_t pos, blknum_t num) {
    if (num == NOPOS)
        num = getLastBlock_client();
    string_q params = "[\"[{ADDRESS}]\",\"[{POS}]\",\"[{NUM}]\"]";
    replace(params, "[{ADDRESS}]", str_2_Addr(addr));
    replace(params, "[{POS}]",  uint_2_Hex(pos));
    replace(params, "[{NUM}]",  uint_2_Hex(num));
    return callRPC("eth_getStorageAt", params, false);

    // Calculating the correct position depends on the storage to retrieve. Consider the following
    // contract deployed at 0x295a70b2de5e3953354a6a8344e616ed314d7251 by address
    // 0x391694e7e0b0cce554cb130d723a9d27458f9298.
    //
    // contract Storage {
    //     uint pos0;
    //     mapping(address => uint) pos1;
    //
    //     function Storage() {
    //         pos0 = 1234;
    //         pos1[msg.sender] = 5678;
    //     }
    // }
    //
    // Retrieving the value of pos0 is straight forward:
    //
    // curl -X POST --data '{"jsonrpc":"2.0", "method": "eth_get StorageAt", "params": ["0x295a70b2de5e3953354a6a8344e616ed314d7251", "0x0", "latest"],
    //      "id": 1}' localhost:8545
    //
    // returns {"jsonrpc":"2.0","id":1,"result":"0x00000000000000000000000000000000000000000000000000000000000004d2"}
    //
    // Retrieving an element of the map is harder. The position of an element in the map is calculated with:
    //
    //      keccack(LeftPad32(key, 0), LeftPad32(map_position, 0))
    //
    // This means to retrieve the storage on pos1["0x391694e7e0b0cce554cb130d723a9d27458f9298"] we need to calculate
    // the position with:
    //
    //      keccak(decodeHex("000000000000000000000000391694e7e0b0cce554cb130d723a9d27458f9298" +
    //                       "0000000000000000000000000000000000000000000000000000000000000001"))
    //
    // The geth console which comes with the web3 library can be used to make the calculation:
    //
    //      > var key = "000000000000000000000000391694e7e0b0cce554cb130d723a9d27458f9298" + "0000000000000000000000000000000000000000000000000000000000000001"
    //      undefined
    //      > web3.sha3(key, {"encoding": "hex"})
    //      "0x6661e9d6d8b923d5bbaab1b96e1dd51ff6ea2a93520fdc9eb75d059238b8c5e9"
    //
    // Now to fetch the storage:
    //
    //      curl -X POST --data '{"jsonrpc":"2.0", "method": "eth_get StorageAt", "params": ["0x295a70b2de5e3953354a6a8344e616ed314d7251",
    //                  "0x6661e9d6d8b923d5bbaab1b96e1dd51ff6ea2a93520fdc9eb75d059238b8c5e9", "latest"], "id": 1}' localhost:8545
    //
    // returns: {"jsonrpc":"2.0","id":1,"result":"0x000000000000000000000000000000000000000000000000000000000000162e"}
    //
}

//--------------------------------------------------------------
static blknum_t findCodeAt_binarySearch(const address_t& addr, blknum_t first, blknum_t last) {
    if (last > first) {
        size_t mid = first + ((last - first) / 2);
        bool atMid  = isContractAt(addr, mid);
        bool atMid1 = isContractAt(addr, mid + 1);
        if (!atMid && atMid1)
            return mid;  // found it
        if (atMid) {
            // we're too high, so search below
            return findCodeAt_binarySearch(addr, first, mid-1);
        }
        // we're too low, so search above
        return findCodeAt_binarySearch(addr, mid+1, last);
    }
    return first;
}

//-------------------------------------------------------------------------------------
blknum_t getDeployBlock(const address_t& addr) {
    if (!isContractAt(addr))
        return NOPOS;
    blknum_t num = findCodeAt_binarySearch(addr, 0, getLastBlock_client());
    return (num ? num+1 : NOPOS);
}
// EXISTING_CODE
}  // namespace qblocks

