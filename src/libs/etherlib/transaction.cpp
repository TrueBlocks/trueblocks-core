/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
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
#include "transaction.h"
#include "etherlib.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CTransaction, CBaseNode);

//---------------------------------------------------------------------------
extern string_q nextTransactionChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextTransactionChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CTransaction::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
    if (!m_showing)
        return;

    if (fmtIn.empty()) {
        ctx << toJson();
        return;
    }

    string_q fmt = fmtIn;
    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextTransactionChunk, this);
}

//---------------------------------------------------------------------------
string_q nextTransactionChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const CTransaction *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CTransaction::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    if (fieldName == "to" && fieldValue == "null")
        *((string_q*)&fieldValue) = "0x";  // NOLINT

    if ( fieldName % "input" ) {
        input = fieldValue;
        function = Format("[{FUNCTION}]");
        return true;

    } else if ( fieldName % "value" ) {
        value = canonicalWei(fieldValue);
        ether = str2Double(Format("[{ETHER}]"));
        return true;

    } else if ( fieldName % "contractAddress" ) {
        receipt.contractAddress = toAddress(fieldValue);
        return true;

    } else if ( fieldName % "gasUsed" ) {
        receipt.gasUsed = toUnsigned(fieldValue);
        return true;
    } else if ( fieldName % "receipt" ) {
        char *p = (char *)fieldValue.c_str();  // NOLINT
        size_t nFields = 0;
        receipt.parseJson(p, nFields);
        return true;
    }
    if (pBlock)
        if (((CBlock*)pBlock)->setValueByName(fieldName, fieldValue))  // NOLINT
            return true;
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'b':
            if ( fieldName % "blockHash" ) { blockHash = toHash(fieldValue); return true; }
            if ( fieldName % "blockNumber" ) { blockNumber = toUnsigned(fieldValue); return true; }
            break;
        case 'f':
            if ( fieldName % "from" ) { from = toAddress(fieldValue); return true; }
            break;
        case 'g':
            if ( fieldName % "gas" ) { gas = toGas(fieldValue); return true; }
            if ( fieldName % "gasPrice" ) { gasPrice = toGas(fieldValue); return true; }
            break;
        case 'h':
            if ( fieldName % "hash" ) { hash = toHash(fieldValue); return true; }
            break;
        case 'i':
            if ( fieldName % "input" ) { input = fieldValue; return true; }
            if ( fieldName % "isError" ) { isError = toUnsigned(fieldValue); return true; }
            if ( fieldName % "isInternal" ) { isInternal = toUnsigned(fieldValue); return true; }
            break;
        case 'n':
            if ( fieldName % "nonce" ) { nonce = toUnsigned(fieldValue); return true; }
            break;
        case 'r':
            if ( fieldName % "receipt" ) { /* receipt = fieldValue; */ return false; }
            break;
        case 't':
            if ( fieldName % "transactionIndex" ) { transactionIndex = toUnsigned(fieldValue); return true; }
            if ( fieldName % "timestamp" ) { timestamp = toTimestamp(fieldValue); return true; }
            if ( fieldName % "to" ) { to = toAddress(fieldValue); return true; }
            break;
        case 'v':
            if ( fieldName % "value" ) { value = toWei(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CTransaction::finishParse() {
    // EXISTING_CODE
    function = Format("[{FUNCTION}]");
    ether = str2Double(Format("[{ETHER}]"));
    receipt.pTrans = this;
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CTransaction::Serialize(SFArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // If we're reading a back level, read the whole thing and we're done.
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> hash;
    archive >> blockHash;
    archive >> blockNumber;
    archive >> transactionIndex;
    archive >> nonce;
    archive >> timestamp;
    archive >> from;
    archive >> to;
    archive >> value;
    archive >> gas;
    archive >> gasPrice;
    archive >> input;
    archive >> isError;
    archive >> isInternal;
    archive >> receipt;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CTransaction::SerializeC(SFArchive& archive) const {

    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << hash;
    archive << blockHash;
    archive << blockNumber;
    archive << transactionIndex;
    archive << nonce;
    archive << timestamp;
    archive << from;
    archive << to;
    archive << value;
    archive << gas;
    archive << gasPrice;
    archive << input;
    archive << isError;
    archive << isInternal;
    archive << receipt;

    return true;
}

//---------------------------------------------------------------------------
SFArchive& operator>>(SFArchive& archive, CTransactionArray& array) {
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
SFArchive& operator<<(SFArchive& archive, const CTransactionArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CTransaction::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    size_t fieldNum = 1000;
    ADD_FIELD(CTransaction, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CTransaction, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CTransaction, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CTransaction, "hash", T_HASH, ++fieldNum);
    ADD_FIELD(CTransaction, "blockHash", T_HASH, ++fieldNum);
    ADD_FIELD(CTransaction, "blockNumber", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTransaction, "transactionIndex", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTransaction, "nonce", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTransaction, "timestamp", T_TIMESTAMP, ++fieldNum);
    ADD_FIELD(CTransaction, "from", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CTransaction, "to", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CTransaction, "value", T_WEI, ++fieldNum);
    ADD_FIELD(CTransaction, "gas", T_GAS, ++fieldNum);
    ADD_FIELD(CTransaction, "gasPrice", T_GAS, ++fieldNum);
    ADD_FIELD(CTransaction, "input", T_TEXT, ++fieldNum);
    ADD_FIELD(CTransaction, "isError", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTransaction, "isInternal", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTransaction, "receipt", T_OBJECT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CTransaction, "schema");
    HIDE_FIELD(CTransaction, "deleted");
    HIDE_FIELD(CTransaction, "showing");

    builtIns.push_back(_biCTransaction);

    // EXISTING_CODE
    // These are 'phony' fields needed because etherscan.io sends them
    // as part of a transaction from its API; however, these fields
    // don't actually exist in a transaction. We add them (so Format
    // works, but hide them since they are never used.
    ADD_FIELD(CTransaction, "confirmations", T_NUMBER, ++fieldNum);
    HIDE_FIELD(CTransaction, "confirmations");

    // Add custom fields
    ADD_FIELD(CTransaction, "gasCost", T_WEI, ++fieldNum);
    ADD_FIELD(CTransaction, "function", T_TEXT, ++fieldNum);
    ADD_FIELD(CTransaction, "articulated", T_TEXT, ++fieldNum);
    ADD_FIELD(CTransaction, "events", T_TEXT, ++fieldNum);
    ADD_FIELD(CTransaction, "price", T_TEXT, ++fieldNum);
    ADD_FIELD(CTransaction, "gasUsed", T_GAS, ++fieldNum);
    ADD_FIELD(CTransaction, "date", T_DATE, ++fieldNum);
    ADD_FIELD(CTransaction, "datesh", T_DATE, ++fieldNum);
    ADD_FIELD(CTransaction, "time", T_DATE, ++fieldNum);
    ADD_FIELD(CTransaction, "ether", T_ETHER, ++fieldNum);
    ADD_FIELD(CTransaction, "encoding", T_TEXT, ++fieldNum);

    // Hide fields we don't want to show by default
    HIDE_FIELD(CTransaction, "function");
    HIDE_FIELD(CTransaction, "articulated");
    HIDE_FIELD(CTransaction, "events");
    HIDE_FIELD(CTransaction, "price");
    HIDE_FIELD(CTransaction, "encoding");
    HIDE_FIELD(CTransaction, "gasCost");
    HIDE_FIELD(CTransaction, "isError");
    HIDE_FIELD(CTransaction, "isInternal");
    HIDE_FIELD(CTransaction, "date");
    HIDE_FIELD(CTransaction, "datesh");
    HIDE_FIELD(CTransaction, "time");
    HIDE_FIELD(CTransaction, "ether");
    if (isTestMode()) {
        UNHIDE_FIELD(CTransaction, "isError");
    }
    //    HIDE_FIELD(CTransaction, "receipt");
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextTransactionChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CTransaction *tra = (const CTransaction *)dataPtr;  // NOLINT
    if (tra) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'a':
                if ( fieldIn % "articulated" ) {
                    if (tra->function.empty() || tra->function == " ")
                        return "[ \"" + tra->input + "\" ]";
                    return tra->function;
                }
                break;
            case 'c':
                if ( fieldIn % "contractAddress" ) return fromAddress(tra->receipt.contractAddress);
                break;
            case 'd':
                if (fieldIn % "date" || fieldIn % "datesh") {
                    timestamp_t ts = (tra->pBlock ? tra->pBlock->timestamp : tra->timestamp);
                    string_q ret = dateFromTimeStamp(ts).Format(FMT_JSON);
                    if (fieldIn % "datesh")  // short date
                        return extract(ret, 0, 10);
                    return ret;
                }
                break;
            case 'e':
                if ( fieldIn % "ether" ) return wei2Ether(asStringBN(tra->value));
                if ( fieldIn % "encoding" ) {
                    return extract(tra->input, 0, 10);
                }
                if ( fieldIn % "events" ) {
                    if (tra->receipt.logs.size())
                        return "++EVENT_LIST++";
                    return "";
                }
                break;
            case 'f':
                if ( fieldIn % "function" )
                    return tra->inputToFunction();
                break;
            case 'g':
                if ( fieldIn % "gasUsed" ) return asStringU(tra->receipt.gasUsed);
                if ( fieldIn % "gasCost" ) {
                    SFUintBN used = tra->receipt.gasUsed;
                    SFUintBN price = tra->gasPrice;
                    return to_string(used * price).c_str();
                }
                break;
            case 't':
                if ( fieldIn % "timestamp" && tra->pBlock) return asString(tra->pBlock->timestamp);
                if ( fieldIn % "time" ) {
                    timestamp_t ts = (tra->pBlock ? tra->pBlock->timestamp : tra->timestamp);
                    return extract(dateFromTimeStamp(ts).Format(FMT_JSON), 12);
                }
                break;
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, tra);
                // EXISTING_CODE
                if ( fieldIn % "price" )
                    return "++PRICE++";
                // EXISTING_CODE
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool CTransaction::readBackLevel(SFArchive& archive) {

    CBaseNode::readBackLevel(archive);
    bool done = false;
    // EXISTING_CODE
    if (m_schema <= getVersionNum(0, 4, 0)) {
        SFWei removed;  // used to be cumulativeGasUsed
        archive >> hash;
        archive >> blockHash;
        archive >> blockNumber;
        archive >> transactionIndex;
        archive >> nonce;
        archive >> timestamp;
        archive >> from;
        archive >> to;
        archive >> value;
        archive >> gas;
        archive >> gasPrice;
        archive >> removed;
        archive >> input;
        archive >> isError;
        archive >> isInternal;
        archive >> receipt;
        finishParse();
        done = true;
    }
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFArchive& operator<<(SFArchive& archive, const CTransaction& tra) {
    tra.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
SFArchive& operator>>(SFArchive& archive, CTransaction& tra) {
    tra.Serialize(archive);
    return archive;
}

//---------------------------------------------------------------------------
string_q CTransaction::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextTransactionChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'b':
            if ( fieldName % "blockHash" ) return fromHash(blockHash);
            if ( fieldName % "blockNumber" ) return asStringU(blockNumber);
            break;
        case 'f':
            if ( fieldName % "from" ) return fromAddress(from);
            break;
        case 'g':
            if ( fieldName % "gas" ) return fromGas(gas);
            if ( fieldName % "gasPrice" ) return fromGas(gasPrice);
            break;
        case 'h':
            if ( fieldName % "hash" ) return fromHash(hash);
            break;
        case 'i':
            if ( fieldName % "input" ) return input;
            if ( fieldName % "isError" ) return asStringU(isError);
            if ( fieldName % "isInternal" ) return asStringU(isInternal);
            break;
        case 'n':
            if ( fieldName % "nonce" ) return asStringU(nonce);
            break;
        case 'r':
            if ( fieldName % "receipt" ) { expContext().noFrst=true; return receipt.Format(); }
            break;
        case 't':
            if ( fieldName % "transactionIndex" ) return asStringU(transactionIndex);
            if ( fieldName % "timestamp" ) return fromTimestamp(timestamp);
            if ( fieldName % "to" ) return fromAddress(to);
            break;
        case 'v':
            if ( fieldName % "value" ) return fromWei(value);
            break;
    }

    // EXISTING_CODE
    // See if this field belongs to the item's container
    ret = nextBlockChunk(fieldName, pBlock);
    if (contains(ret, "Field not found"))
        ret = "";
    if (!ret.empty())
        return ret;
    // EXISTING_CODE

    string_q s;
    s = toUpper(string_q("receipt")) + "::";
    if (contains(fieldName, s)) {
        string_q f = fieldName;
        replaceAll(f, s, "");
        f = receipt.getValueByName(f);
        return f;
    }

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CTransaction& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode *CTransaction::getObjectAt(const string_q& fieldName, size_t index) const {
    if ( fieldName % "receipt" )
        return &receipt;
    return NULL;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
//--------------------------------------------------------------------
bool sortTransactionsForWrite(const CTransaction& t1, const CTransaction& t2) {
    if (t1.timestamp != t2.timestamp)
        return t1.timestamp < t2.timestamp;
    else if (t1.from.compare(t2.from))  // not equal
        return t1.from < t2.from;
    else if (t1.transactionIndex != t2.transactionIndex)
        return t1.transactionIndex < t2.transactionIndex;
    return t1.hash < t2.hash;
}

//--------------------------------------------------------------------
inline string_q asStringULL(uint64_t i) {
    ostringstream os;
    os << i;
    return os.str().c_str();
}

//----------------------------------------------------------------------------
inline unsigned char hex2Ascii(char *str) {
    unsigned char c;
    c =  (unsigned char)((str[0] >= 'A' ? ((str[0]&0xDF)-'A')+10 : (str[0]-'0')));
    c *= 16;
    c = (unsigned char)(c + (str[1] >= 'A' ? ((str[1]&0xDF)-'A')+10 : (str[1]-'0')));
    return c;
}

//----------------------------------------------------------------------------
inline string_q hex2String(const string_q& inHex) {
    string_q ret, in = startsWith(inHex, "0x") ? extract(inHex, 2) : inHex;
    while (!in.empty()) {
        string_q nibble = extract(in, 0, 2);
        in = extract(in, 2);
        ret += (char)hex2Ascii((char*)nibble.c_str());  // NOLINT
    }
    return ret;
}

//------------------------------------------------------------------------------
#define toBigNum2(a, b)      string_q(to_string(canonicalWei("0x" + grabPart(a, b))).c_str())
#define grabPart(a, b)       trimLeading(extract((a), 64*(b), 64), '0')
#define grabBigNum(a, b)     strtoull(grabPart(a, b).c_str(), NULL, 16)
#define toAddr(a, b)         "0x" + padLeft(grabPart(a, b), 40, '0')
#define toAddrOld(a, b)      "0x" + grabPart(a, b)
#define toAscString(a, b)    hex2String("0x" + grabPart(a, b))
#define toBigNum(a, b)       asStringULL(grabBigNum(a, b))
#define toBigNum3(a, b)      padNum3(grabBigNum(a, b))
#define theRest(a, b)        extract((a), 64*(b), (a).length())
#define toVote(a, b)         (grabBigNum(a, b) ? "Yea" : "Nay")
#define toBoolean(a, b)      (grabBigNum(a, b) ? "true" : "false")
#define toBytes(a, b)        extract((a), 64*(b), 64)
string_q parse(const string_q& params, size_t nItems, string_q *types) {

    string_q ret;
    for (size_t item = 0 ; item < (size_t)nItems ; item++) {
        string_q t = types[item];
        bool isDynamic = (t == "string" || t == "bytes" || contains(t, "[]"));
        string_q val;

             if ( t == "address"                    )   val =          toAddr      (params, item);  // NOLINT
        else if ( t == "bool"                       )   val =          toBoolean   (params, item);
        else if ( t == "vote"                       )   val =          toVote      (params, item);
        else if ( t == "uint3"                      )   val =          toBigNum3   (params, item);
        else if ( t == "bytes256"                   )   val =          toAscString (params, item);
        else if ( contains(t, "int") &&   !isDynamic)   val =          toBigNum2   (params, item);
        else if ( contains(t, "bytes") && !isDynamic)   val =          toBytes     (params, item);
        else if ( isDynamic                         )   val = "off:" + toBigNum2   (params, item);
        else                                            val = "unknown type: " + t;

        if (contains(val, "off:")) {
            size_t start = toLongU(substitute(val, "off:", "")) / (size_t)32;
            size_t len   = grabBigNum(params, start);
            if (len == NOPOS)
                len = params.length()-start;
            if (t == "string") {
                val = "\"";
                val += substitute(
                            substitute(
                                substitute(
                                    hex2String(extract(params, (start+1) * 64, len * 2)),
                                "\n", "\\n"),
                            "\r", ""),
                        "\"", "\\\"");
                val += "\"";
            } else {
                val = "0x" + extract(params, (start+1) * 64, len * 2);
            }
        }
        ret += ("|" + val);
    }

    return "\"" + trim(ret, '|') + "\"";
}

//---------------------------------------------------------------------------
string_q toFunction(const string_q& name, const string_q& input, size_t nItems, string_q *items) {
    return "[ \"" + name + "\", " + substitute(parse(extract(input, 10), nItems, items), "|", "\", \"") + " ]";
}

//---------------------------------------------------------------------------
string_q CTransaction::inputToFunction(void) const {
    if (input.length() < 10)
        return " ";

    if (funcPtr) {
        string_q items[256];
        size_t nItems = 0;
        for (size_t i = 0 ; i < funcPtr->inputs.size() ; i++)
            items[nItems++] = funcPtr->inputs[i].type;
        return toFunction(funcPtr->name, input, nItems, items);
    }

    return " ";
}
// EXISTING_CODE
}  // namespace qblocks

