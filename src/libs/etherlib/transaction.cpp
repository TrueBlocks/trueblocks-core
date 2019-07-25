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

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["transaction_fmt"] : fmtIn);
    if (fmt.empty()) {
        ctx << toJson();
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextTransactionChunk, this);
}

//---------------------------------------------------------------------------
string_q nextTransactionChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CTransaction *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CTransaction::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    //LOG4("CTransaction::setValueByName --> " + fieldName + "=" + fieldValue.substr(0,50));
    if (fieldName == "to" && fieldValue == "null")
        fieldValue = "0x";  // NOLINT

    if ( fieldName % "input" ) {
        input = fieldValue;
        return true;

    } else if ( fieldName % "value" ) {
        value = str_2_Wei(fieldValue);
        return true;

    } else if ( fieldName % "contractAddress" ) {
        receipt.contractAddress = str_2_Addr(fieldValue);
        return true;

    } else if ( fieldName % "gasUsed" ) {
        receipt.gasUsed = str_2_Uint(fieldValue);
        return true;

    } else if ( fieldName % "receipt" ) {
        string_q str = fieldValue;
        return receipt.parseJson3(str);

    }

    if (pBlock) {
        if (fieldName == "hash")
            fieldName = "tx_hash";  // otherwise, the block uses it and returns empty
        bool ret = ((CBlock*)pBlock)->setValueByName(fieldName, fieldValue);  // NOLINT
        if (fieldName == "tx_hash")
            fieldName = "hash";  // otherwise, the block uses it and returns empty
        if (ret) {
            bool done = (fieldName != "blockHash" && fieldName != "blockNumber" && fieldName != "gasUsed");
            //LOG4(fieldName, done);
            if (done) {
                //LOG4("set in block");
                return true;
            } else {
                //LOG4("set in block and transaction");
            }
        } else {
            //LOG4("not set in block");
        }
    }

    // Order matters
    if (fieldName == "transactionHash")
        fieldName = "hash";  // NOLINT
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "articulatedTx" ) { return articulatedTx.parseJson3(fieldValue); }
            break;
        case 'b':
            if ( fieldName % "blockHash" ) { blockHash = str_2_Hash(fieldValue); return true; }
            if ( fieldName % "blockNumber" ) { blockNumber = str_2_Uint(fieldValue); return true; }
            break;
        case 'c':
            if ( fieldName % "compressedTx" ) { compressedTx = fieldValue; return true; }
            break;
        case 'f':
            if ( fieldName % "from" ) { from = str_2_Addr(fieldValue); return true; }
            if ( fieldName % "finalized" ) { finalized = str_2_Bool(fieldValue); return true; }
            break;
        case 'g':
            if ( fieldName % "gas" ) { gas = str_2_Gas(fieldValue); return true; }
            if ( fieldName % "gasPrice" ) { gasPrice = str_2_Gas(fieldValue); return true; }
            break;
        case 'h':
            if ( fieldName % "hash" ) { hash = str_2_Hash(fieldValue); return true; }
            break;
        case 'i':
            if ( fieldName % "input" ) { input = fieldValue; return true; }
            if ( fieldName % "isError" ) { isError = str_2_Uint(fieldValue); return true; }
            if ( fieldName % "isInternal" ) { isInternal = str_2_Uint(fieldValue); return true; }
            break;
        case 'n':
            if ( fieldName % "nonce" ) { nonce = str_2_Uint(fieldValue); return true; }
            break;
        case 'r':
            if ( fieldName % "receipt" ) { return receipt.parseJson3(fieldValue); }
            break;
        case 't':
            if ( fieldName % "transactionIndex" ) { transactionIndex = str_2_Uint(fieldValue); return true; }
            if ( fieldName % "timestamp" ) { timestamp = str_2_Ts(fieldValue); return true; }
            if ( fieldName % "to" ) { to = str_2_Addr(fieldValue); return true; }
            if ( fieldName % "traces" ) {
                CTrace item;
                string_q str = fieldValue;
                while (item.parseJson3(str)) {
                    traces.push_back(item);
                    item = CTrace();  // reset
                }
                return true;
            }
            break;
        case 'v':
            if ( fieldName % "value" ) { value = str_2_Wei(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CTransaction::finishParse() {
    // EXISTING_CODE
    receipt.pTrans = this;
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CTransaction::Serialize(CArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
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
//    archive >> articulatedTx;
//    archive >> compressedTx;
//    archive >> finalized;
//    archive >> traces;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CTransaction::SerializeC(CArchive& archive) const {

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
//    archive << articulatedTx;
//    archive << compressedTx;
//    archive << finalized;
//    archive << traces;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CTransactionArray& array) {
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
CArchive& operator<<(CArchive& archive, const CTransactionArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CTransaction::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CTransaction, "schema")) return;

    size_t fieldNum = 1000;
    ADD_FIELD(CTransaction, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CTransaction, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CTransaction, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CTransaction, "cname", T_TEXT,  ++fieldNum);
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
    ADD_FIELD(CTransaction, "articulatedTx", T_OBJECT, ++fieldNum);
    HIDE_FIELD(CTransaction, "articulatedTx");
    ADD_FIELD(CTransaction, "compressedTx", T_TEXT, ++fieldNum);
    HIDE_FIELD(CTransaction, "compressedTx");
    ADD_FIELD(CTransaction, "finalized", T_BOOL, ++fieldNum);
    HIDE_FIELD(CTransaction, "finalized");
    ADD_FIELD(CTransaction, "traces", T_OBJECT|TS_ARRAY, ++fieldNum);
    HIDE_FIELD(CTransaction, "traces");

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CTransaction, "schema");
    HIDE_FIELD(CTransaction, "deleted");
    HIDE_FIELD(CTransaction, "showing");
    HIDE_FIELD(CTransaction, "cname");

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
    ADD_FIELD(CTransaction, "etherGasCost", T_WEI, ++fieldNum);
    ADD_FIELD(CTransaction, "function", T_TEXT, ++fieldNum);
    ADD_FIELD(CTransaction, "events", T_TEXT, ++fieldNum);
    ADD_FIELD(CTransaction, "price", T_TEXT, ++fieldNum);
    ADD_FIELD(CTransaction, "gasUsed", T_GAS, ++fieldNum);
    ADD_FIELD(CTransaction, "date", T_DATE, ++fieldNum);
    ADD_FIELD(CTransaction, "datesh", T_DATE, ++fieldNum);
    ADD_FIELD(CTransaction, "time", T_DATE, ++fieldNum);
    ADD_FIELD(CTransaction, "age", T_DATE, ++fieldNum);
    ADD_FIELD(CTransaction, "ether", T_ETHER, ++fieldNum);
    ADD_FIELD(CTransaction, "encoding", T_TEXT, ++fieldNum);

    // Hide fields we don't want to show by default
    HIDE_FIELD(CTransaction, "function");
    HIDE_FIELD(CTransaction, "events");
    HIDE_FIELD(CTransaction, "price");
    HIDE_FIELD(CTransaction, "encoding");
    HIDE_FIELD(CTransaction, "gasCost");
    HIDE_FIELD(CTransaction, "etherGasCost");
    HIDE_FIELD(CTransaction, "etherGasPrice");
    HIDE_FIELD(CTransaction, "isError");
    HIDE_FIELD(CTransaction, "isInternal");
    HIDE_FIELD(CTransaction, "date");
    HIDE_FIELD(CTransaction, "datesh");
    HIDE_FIELD(CTransaction, "time");
    HIDE_FIELD(CTransaction, "age");
    HIDE_FIELD(CTransaction, "ether");
    if (isTestMode()) {
        UNHIDE_FIELD(CTransaction, "isError");
    }
    if (!getEnvStr("API_MODE").empty()) {
        UNHIDE_FIELD(CTransaction, "datesh");
        UNHIDE_FIELD(CTransaction, "time");
        UNHIDE_FIELD(CTransaction, "date");
        UNHIDE_FIELD(CTransaction, "age");
        UNHIDE_FIELD(CTransaction, "ether");
    }
    //    HIDE_FIELD(CTransaction, "receipt");
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextTransactionChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CTransaction *tra = reinterpret_cast<const CTransaction *>(dataPtr);
    if (tra) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'a':
                if ( fieldIn % "age" ) {
                    if (isTestMode())
                        return "100";
                    static CBlock latest;
                    if (latest.timestamp == 0)
                        getBlock_light(latest, "latest");
                    timestamp_t myTs = (tra->pBlock ? tra->pBlock->timestamp : tra->timestamp);
                    timestamp_t blkTs = ((timestamp_t)latest.timestamp);
                    if (blkTs > myTs) {
                        return int_2_Str(blkTs - myTs);
                    }
                    return "0";
                }
                break;
            case 'c':
                if ( fieldIn % "contractAddress" ) return addr_2_Str(tra->receipt.contractAddress);
                if ( fieldIn % "compressedTx" ) return tra->articulatedTx.compressed();
                break;
            case 'd':
                if (fieldIn % "date" || fieldIn % "datesh") {
                    timestamp_t ts = (tra->pBlock ? tra->pBlock->timestamp : tra->timestamp);
                    string_q ret = ts_2_Date(ts).Format(FMT_JSON);
                    if (fieldIn % "datesh")  // short date
                        return extract(ret, 0, 10);
                    return ret;
                }
                break;
            case 'e':
                if ( fieldIn % "ether" )
                    return wei_2_Ether(bnu_2_Str(tra->value));
                if ( fieldIn % "encoding" )
                    return extract(tra->input, 0, 10);
                if ( fieldIn % "events" || fieldIn % "eventnames") {
                    string_q ret;
                    for (uint64_t n = 0 ; n < tra->receipt.logs.size() ; n++) {
                        string_q v = tra->receipt.logs[n].Format("[{ARTICULATEDLOG}]");
                        if (v.empty())
                            return "";
                        CFunction func;
                        func.parseJson3(v);
                        if (fieldIn % "events") {
                            if (func.name.empty())
                                func.name = "UnknownEvent";
                            ret += (func.name + "(\"");
                            for (auto param : func.inputs) {
                                if (param.name != func.inputs[0].name)
                                    ret += "\",\"";
                                ret += param.value;
                            }
                            ret += "\")";
                            if (n < tra->receipt.logs.size() - 1)
                                ret += ";";
                        } else {
                            if (!ret.empty())
                                ret += ",";
                            ret += func.name;
                        }
                    }
                    return ret;
                }
                if ( fieldIn % "etherGasCost" ) {
                    biguint_t used = tra->receipt.gasUsed;
                    biguint_t price = tra->gasPrice;
                    return wei_2_Ether(bnu_2_Str(used * price));
                }
                if ( fieldIn % "etherGasPrice" ) { return wei_2_Ether(bnu_2_Str(tra->gasPrice)); }
                break;
            case 'f':
                if ( fieldIn % "function" ) {
                    string_q ret = tra->Format("[{ARTICULATEDTX}]");
                    if (ret.empty())
                        return "";
                    CFunction func;
                    func.parseJson3(ret);
                    return func.name;
                }
                break;
            case 'g':
                if ( fieldIn % "gasUsed" ) return uint_2_Str(tra->receipt.gasUsed);
                if ( fieldIn % "gasCost" ) {
                    biguint_t used = tra->receipt.gasUsed;
                    biguint_t price = tra->gasPrice;
                    return bnu_2_Str(used * price);
                }
                break;
            case 't':
                if ( fieldIn % "tracescnt" ) return uint_2_Str(getTraceCount(tra->hash));
                if ( fieldIn % "timestamp" && tra->pBlock) return int_2_Str(tra->pBlock->timestamp);
                if ( fieldIn % "time" ) {
                    timestamp_t ts = (tra->pBlock ? tra->pBlock->timestamp : tra->timestamp);
                    return extract(ts_2_Date(ts).Format(FMT_JSON), 12);
                }
                break;
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, tra);
                // EXISTING_CODE
                if ( fieldIn % "price" ) {
                    if (!IS_HIDDEN(CTransaction, "price")) {
                        timestamp_t ts = str_2_Ts(tra->Format("[{TIMESTAMP}]"));  // it may only be on the block
                        return wei_2_Dollars(ts, weiPerEther()); // this has huge performance implications because it loads a big file
                    }
                }
                // EXISTING_CODE
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool CTransaction::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    if (m_schema <= getVersionNum(0, 4, 0)) {
        wei_t removed;  // used to be cumulativeGasUsed
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
CArchive& operator<<(CArchive& archive, const CTransaction& tra) {
    tra.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CTransaction& tra) {
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
        case 'a':
            if ( fieldName % "articulatedTx" ) {
                if (articulatedTx == CFunction())
                    return "";
                expContext().noFrst=true;
                return articulatedTx.Format();
            }
            break;
        case 'b':
            if ( fieldName % "blockHash" ) return hash_2_Str(blockHash);
            if ( fieldName % "blockNumber" ) return uint_2_Str(blockNumber);
            break;
        case 'c':
            if ( fieldName % "compressedTx" ) return compressedTx;
            break;
        case 'f':
            if ( fieldName % "from" ) return addr_2_Str(from);
            if ( fieldName % "finalized" ) return int_2_Str(finalized);
            break;
        case 'g':
            if ( fieldName % "gas" ) return gas_2_Str(gas);
            if ( fieldName % "gasPrice" ) return gas_2_Str(gasPrice);
            break;
        case 'h':
            if ( fieldName % "hash" ) return hash_2_Str(hash);
            break;
        case 'i':
            if ( fieldName % "input" ) return input;
            if ( fieldName % "isError" ) return uint_2_Str(isError);
            if ( fieldName % "isInternal" ) return uint_2_Str(isInternal);
            break;
        case 'n':
            if ( fieldName % "nonce" ) return uint_2_Str(nonce);
            break;
        case 'r':
            if ( fieldName % "receipt" ) { expContext().noFrst=true; return receipt.Format(); }
            break;
        case 't':
            if ( fieldName % "transactionIndex" ) return uint_2_Str(transactionIndex);
            if ( fieldName % "timestamp" ) return ts_2_Str(timestamp);
            if ( fieldName % "to" ) return addr_2_Str(to);
            if ( fieldName % "traces" || fieldName % "tracesCnt" ) {
                size_t cnt = traces.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
                if (!cnt) return "";
                string_q retS;
                for (size_t i = 0 ; i < cnt ; i++) {
                    retS += traces[i].Format();
                    retS += ((i < cnt - 1) ? ",\n" : "\n");
                }
                return retS;
            }
            break;
        case 'v':
            if ( fieldName % "value" ) return wei_2_Str(value);
            break;
    }

    // EXISTING_CODE
    if (fieldName != "schema" && fieldName != "deleted" && fieldName != "showing" && fieldName != "cname") {
        // See if this field belongs to the item's container
        ret = nextBlockChunk(fieldName, pBlock);
        if (contains(ret, "Field not found"))
            ret = "";
        if (!ret.empty())
            return ret;
    }
    // EXISTING_CODE

    string_q s;
    s = toUpper(string_q("receipt")) + "::";
    if (contains(fieldName, s)) {
        string_q f = fieldName;
        replaceAll(f, s, "");
        f = receipt.getValueByName(f);
        return f;
    }

    s = toUpper(string_q("articulatedTx")) + "::";
    if (contains(fieldName, s)) {
        string_q f = fieldName;
        replaceAll(f, s, "");
        f = articulatedTx.getValueByName(f);
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
    if ( fieldName % "articulatedTx" )
        return &articulatedTx;
    if ( fieldName % "traces" && index < traces.size() )
        return &traces[index];
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

//-------------------------------------------------------------------------
bool CTransaction::loadAsBlockReward(blknum_t bn, blknum_t txid, const address_t& addr) {
    blockNumber = bn;
    transactionIndex = txid;
    hash = uint_2_Hex(bn * 100000 + txid);
    to = addr;
    from = (txid == 99999 ? "0xBlockReward" : "0xUncleReward");
    value = blockReward(bn, txid, false);
    return true;
}

//-------------------------------------------------------------------------
bool CTransaction::loadAsPrefund(const CStringArray& prefunds, const address_t& addr) {
    if (prefunds.size() == 0)
        return false;

    blknum_t id = transactionIndex;
    initialize();
    transactionIndex = id;
    for (auto prefund : prefunds) {
        CStringArray parts;
        explode(parts, prefund, '\t');
        if (toLower(parts[0]) == addr) {
            to = addr;
            from = "0xPrefund";
            value = str_2_Wei(parts[1]);
            return true;
        }
    }
    return false;
}
// EXISTING_CODE
}  // namespace qblocks

