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
#include "transaction.h"
#include "etherlib.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CTransaction, CBaseNode);

//---------------------------------------------------------------------------
extern string_q nextTransactionChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextTransactionChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CTransaction::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["transaction_fmt"] : fmtIn);
    if (fmt.empty()) {
        toJson(ctx);
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextTransactionChunk, this);
}

//---------------------------------------------------------------------------
string_q nextTransactionChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CTransaction*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CTransaction::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextTransactionChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "articulatedTx") {
                if (articulatedTx == CFunction())
                    return "{}";
                return articulatedTx.Format();
            }
            break;
        case 'b':
            if (fieldName % "blockHash") {
                return hash_2_Str(blockHash);
            }
            if (fieldName % "blockNumber") {
                return uint_2_Str(blockNumber);
            }
            break;
        case 'c':
            if (fieldName % "cachebits") {
                return uint_2_Str(cachebits);
            }
            if (fieldName % "compressedTx") {
                return compressedTx;
            }
            break;
        case 'e':
            if (fieldName % "extraValue1") {
                return wei_2_Str(extraValue1);
            }
            if (fieldName % "extraValue2") {
                return wei_2_Str(extraValue2);
            }
            break;
        case 'f':
            if (fieldName % "from") {
                return addr_2_Str(from);
            }
            if (fieldName % "finalized") {
                return bool_2_Str_t(finalized);
            }
            break;
        case 'g':
            if (fieldName % "gas") {
                return gas_2_Str(gas);
            }
            if (fieldName % "gasPrice") {
                return gas_2_Str(gasPrice);
            }
            break;
        case 'h':
            if (fieldName % "hash") {
                return hash_2_Str(hash);
            }
            if (fieldName % "hasToken") {
                return uint_2_Str(hasToken);
            }
            break;
        case 'i':
            if (fieldName % "input") {
                return input;
            }
            if (fieldName % "isError") {
                return uint_2_Str(isError);
            }
            break;
        case 'm':
            if (fieldName % "maxFeePerGas") {
                return gas_2_Str(maxFeePerGas);
            }
            if (fieldName % "maxPriorityFeePerGas") {
                return gas_2_Str(maxPriorityFeePerGas);
            }
            break;
        case 'n':
            if (fieldName % "nonce") {
                return uint_2_Str(nonce);
            }
            break;
        case 'r':
            if (fieldName % "reserved2") {
                return uint_2_Str(reserved2);
            }
            if (fieldName % "receipt") {
                if (receipt == CReceipt())
                    return "{}";
                return receipt.Format();
            }
            break;
        case 's':
            if (fieldName % "statements" || fieldName % "statementsCnt") {
                size_t cnt = statements.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
                if (!cnt)
                    return "";
                string_q retS;
                for (size_t i = 0; i < cnt; i++) {
                    retS += statements[i].Format();
                    retS += ((i < cnt - 1) ? ",\n" : "\n");
                }
                return retS;
            }
            break;
        case 't':
            if (fieldName % "transactionIndex") {
                return uint_2_Str(transactionIndex);
            }
            if (fieldName % "timestamp") {
                return ts_2_Str(timestamp);
            }
            if (fieldName % "to") {
                return addr_2_Str(to);
            }
            if (fieldName % "traces" || fieldName % "tracesCnt") {
                size_t cnt = traces.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
                if (!cnt)
                    return "";
                string_q retS;
                for (size_t i = 0; i < cnt; i++) {
                    retS += traces[i].Format();
                    retS += ((i < cnt - 1) ? ",\n" : "\n");
                }
                return retS;
            }
            break;
        case 'v':
            if (fieldName % "value") {
                return wei_2_Str(value);
            }
            break;
        default:
            break;
    }

    extern string_q nextBlockChunk(const string_q& fieldIn, const void* data);
    ret = nextBlockChunk(fieldName, pBlock);
    if (contains(ret, "Field not found"))
        ret = "";
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // test for contained object field specifiers
    string_q objSpec;
    objSpec = toUpper("receipt") + "::";
    if (contains(fieldName, objSpec))
        return receipt.getValueByName(substitute(fieldName, objSpec, ""));

    objSpec = toUpper("articulatedTx") + "::";
    if (contains(fieldName, objSpec))
        return articulatedTx.getValueByName(substitute(fieldName, objSpec, ""));

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//---------------------------------------------------------------------------------------------------
bool CTransaction::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "articulatedTx") {
                return articulatedTx.parseJson3(fieldValue);
            }
            break;
        case 'b':
            if (fieldName % "blockHash") {
                blockHash = str_2_Hash(fieldValue);
                return true;
            }
            if (fieldName % "blockNumber") {
                blockNumber = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'c':
            if (fieldName % "cachebits") {
                cachebits = (uint8_t)str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "compressedTx") {
                compressedTx = fieldValue;
                return true;
            }
            break;
        case 'e':
            if (fieldName % "extraValue1") {
                extraValue1 = str_2_Wei(fieldValue);
                return true;
            }
            if (fieldName % "extraValue2") {
                extraValue2 = str_2_Wei(fieldValue);
                return true;
            }
            break;
        case 'f':
            if (fieldName % "from") {
                from = str_2_Addr(fieldValue);
                return true;
            }
            if (fieldName % "finalized") {
                finalized = str_2_Bool(fieldValue);
                return true;
            }
            break;
        case 'g':
            if (fieldName % "gas") {
                gas = str_2_Gas(fieldValue);
                return true;
            }
            if (fieldName % "gasPrice") {
                gasPrice = str_2_Gas(fieldValue);
                return true;
            }
            break;
        case 'h':
            if (fieldName % "hash") {
                hash = str_2_Hash(fieldValue);
                return true;
            }
            if (fieldName % "hasToken") {
                hasToken = (uint8_t)str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'i':
            if (fieldName % "input") {
                input = toLower(fieldValue);
                return true;
            }
            if (fieldName % "isError") {
                isError = (uint8_t)str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'm':
            if (fieldName % "maxFeePerGas") {
                maxFeePerGas = str_2_Gas(fieldValue);
                return true;
            }
            if (fieldName % "maxPriorityFeePerGas") {
                maxPriorityFeePerGas = str_2_Gas(fieldValue);
                return true;
            }
            break;
        case 'n':
            if (fieldName % "nonce") {
                nonce = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'r':
            if (fieldName % "reserved2") {
                reserved2 = (uint8_t)str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "receipt") {
                return receipt.parseJson3(fieldValue);
            }
            break;
        case 's':
            if (fieldName % "statements") {
                CReconciliation obj;
                string_q str = fieldValue;
                while (obj.parseJson3(str)) {
                    statements.push_back(obj);
                    obj = CReconciliation();  // reset
                }
                return true;
            }
            break;
        case 't':
            if (fieldName % "transactionIndex") {
                transactionIndex = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "timestamp") {
                timestamp = str_2_Ts(fieldValue);
                return true;
            }
            if (fieldName % "to") {
                to = str_2_Addr(fieldValue);
                return true;
            }
            if (fieldName % "traces") {
                CTrace obj;
                string_q str = fieldValue;
                while (obj.parseJson3(str)) {
                    traces.push_back(obj);
                    obj = CTrace();  // reset
                }
                return true;
            }
            break;
        case 'v':
            if (fieldName % "value") {
                value = str_2_Wei(fieldValue);
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CTransaction::finishParse() {
    // EXISTING_CODE
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
    archive >> extraValue1;
    archive >> extraValue2;
    archive >> gas;
    archive >> gasPrice;
    archive >> maxFeePerGas;
    archive >> maxPriorityFeePerGas;
    archive >> input;
    archive >> isError;
    archive >> hasToken;
    archive >> cachebits;
    archive >> reserved2;
    archive >> receipt;
    archive >> traces;
    archive >> articulatedTx;
    // archive >> compressedTx;
    // archive >> statements;
    // archive >> finalized;
    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CTransaction::SerializeC(CArchive& archive) const {
    // Writing always writes the latest version of the data
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
    archive << extraValue1;
    archive << extraValue2;
    archive << gas;
    archive << gasPrice;
    archive << maxFeePerGas;
    archive << maxPriorityFeePerGas;
    archive << input;
    archive << isError;
    archive << hasToken;
    archive << cachebits;
    archive << reserved2;
    archive << receipt;
    archive << traces;
    archive << articulatedTx;
    // archive << compressedTx;
    // archive << statements;
    // archive << finalized;
    // EXISTING_CODE
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CTransaction::Migrate(CArchive& archiveIn, CArchive& archiveOut) const {
    ASSERT(archiveIn.isReading());
    ASSERT(archiveOut.isWriting());
    CTransaction copy;
    // EXISTING_CODE
    // EXISTING_CODE
    copy.Serialize(archiveIn);
    copy.SerializeC(archiveOut);
    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CTransactionArray& array) {
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
CArchive& operator<<(CArchive& archive, const CTransactionArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CTransaction::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CTransaction, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CTransaction, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTransaction, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CTransaction, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CTransaction, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CTransaction, "hash", T_HASH | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CTransaction, "blockHash", T_HASH | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CTransaction, "blockNumber", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CTransaction, "transactionIndex", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CTransaction, "nonce", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CTransaction, "timestamp", T_TIMESTAMP, ++fieldNum);
    ADD_FIELD(CTransaction, "from", T_ADDRESS | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CTransaction, "to", T_ADDRESS | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CTransaction, "value", T_WEI, ++fieldNum);
    ADD_FIELD(CTransaction, "extraValue1", T_WEI, ++fieldNum);
    ADD_FIELD(CTransaction, "extraValue2", T_WEI, ++fieldNum);
    ADD_FIELD(CTransaction, "gas", T_GAS, ++fieldNum);
    ADD_FIELD(CTransaction, "gasPrice", T_GAS, ++fieldNum);
    ADD_FIELD(CTransaction, "maxFeePerGas", T_GAS, ++fieldNum);
    ADD_FIELD(CTransaction, "maxPriorityFeePerGas", T_GAS, ++fieldNum);
    ADD_FIELD(CTransaction, "input", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CTransaction, "isError", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CTransaction, "hasToken", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CTransaction, "cachebits", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CTransaction, "reserved2", T_UNUMBER, ++fieldNum);
    ADD_OBJECT(CTransaction, "receipt", T_OBJECT | TS_OMITEMPTY, ++fieldNum, GETRUNTIME_CLASS(CReceipt));
    ADD_FIELD(CTransaction, "traces", T_OBJECT | TS_ARRAY | TS_OMITEMPTY, ++fieldNum);
    ADD_OBJECT(CTransaction, "articulatedTx", T_OBJECT | TS_OMITEMPTY, ++fieldNum, GETRUNTIME_CLASS(CFunction));
    ADD_FIELD(CTransaction, "compressedTx", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CTransaction, "compressedTx");
    ADD_FIELD(CTransaction, "statements", T_OBJECT | TS_ARRAY | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CTransaction, "statements");
    ADD_FIELD(CTransaction, "finalized", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CTransaction, "finalized");

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CTransaction, "schema");
    HIDE_FIELD(CTransaction, "deleted");
    HIDE_FIELD(CTransaction, "showing");
    HIDE_FIELD(CTransaction, "cname");

    builtIns.push_back(_biCTransaction);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextTransactionChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CTransaction* tra = reinterpret_cast<const CTransaction*>(dataPtr);
    if (tra) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, tra);
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
bool CTransaction::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
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

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CTransaction& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode* CTransaction::getObjectAt(const string_q& fieldName, size_t index) const {
    // EXISTING_CODE
    // EXISTING_CODE
    if (fieldName % "receipt")
        return &receipt;
    if (fieldName % "traces") {
        if (index == NOPOS) {
            CTrace empty;
            ((CTransaction*)this)->traces.push_back(empty);  // NOLINT
            index = traces.size() - 1;
        }
        if (index < traces.size())
            return &traces[index];
    }
    if (fieldName % "articulatedTx")
        return &articulatedTx;
    if (fieldName % "statements") {
        if (index == NOPOS) {
            CReconciliation empty;
            ((CTransaction*)this)->statements.push_back(empty);  // NOLINT
            index = statements.size() - 1;
        }
        if (index < statements.size())
            return &statements[index];
    }
    // EXISTING_CODE
    // EXISTING_CODE

    return NULL;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_TRANSACTION =
    "[{BLOCKNUMBER}]\t"
    "[{TRANSACTIONINDEX}]\t"
    "[{DATE}]\t"
    "[{TIMESTAMP}]\t"
    "[{FROM}]\t"
    "[{TO}]\t"
    "[{ETHER}]\t"
    "[{ETHERGASPRICE}]\t"
    "[{GASUSED}]\t"
    "[{HASH}]\t"
    "[{ISERROR}]\t"
    "[{ENCODING}]\t"
    "[{COMPRESSEDTX}]";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
