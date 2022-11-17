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
#include "reconciliation.h"
#include "etherlib.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CReconciliation, CBaseNode);

//---------------------------------------------------------------------------
extern string_q nextReconciliationChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextReconciliationChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CReconciliation::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["reconciliation_fmt"] : fmtIn);
    if (fmt.empty()) {
        toJson(ctx);
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextReconciliationChunk, this);
}

//---------------------------------------------------------------------------
string_q nextReconciliationChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CReconciliation*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CReconciliation::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextReconciliationChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "assetAddr") {
                return addr_2_Str(assetAddr);
            }
            if (fieldName % "assetSymbol") {
                return assetSymbol;
            }
            if (fieldName % "accountedFor") {
                return addr_2_Str(accountedFor);
            }
            if (fieldName % "amountIn") {
                return bni_2_Str(amountIn);
            }
            if (fieldName % "amountOut") {
                return bni_2_Str(amountOut);
            }
            break;
        case 'b':
            if (fieldName % "blockNumber") {
                return uint_2_Str(blockNumber);
            }
            if (fieldName % "begBal") {
                return bni_2_Str(begBal);
            }
            break;
        case 'd':
            if (fieldName % "decimals") {
                return uint_2_Str(decimals);
            }
            break;
        case 'e':
            if (fieldName % "endBal") {
                return bni_2_Str(endBal);
            }
            if (fieldName % "encoding") {
                return encoding;
            }
            break;
        case 'g':
            if (fieldName % "gasOut") {
                return bni_2_Str(gasOut);
            }
            break;
        case 'i':
            if (fieldName % "internalIn") {
                return bni_2_Str(internalIn);
            }
            if (fieldName % "internalOut") {
                return bni_2_Str(internalOut);
            }
            break;
        case 'l':
            if (fieldName % "logIndex") {
                return uint_2_Str(logIndex);
            }
            break;
        case 'm':
            if (fieldName % "minerBaseRewardIn") {
                return bni_2_Str(minerBaseRewardIn);
            }
            if (fieldName % "minerNephewRewardIn") {
                return bni_2_Str(minerNephewRewardIn);
            }
            if (fieldName % "minerTxFeeIn") {
                return bni_2_Str(minerTxFeeIn);
            }
            if (fieldName % "minerUncleRewardIn") {
                return bni_2_Str(minerUncleRewardIn);
            }
            break;
        case 'p':
            if (fieldName % "priceSource") {
                return priceSource;
            }
            if (fieldName % "prefundIn") {
                return bni_2_Str(prefundIn);
            }
            if (fieldName % "prevAppBlk") {
                return uint_2_Str(prevAppBlk);
            }
            if (fieldName % "prevBal") {
                return bni_2_Str(prevBal);
            }
            break;
        case 'r':
            if (fieldName % "recipient") {
                return addr_2_Str(recipient);
            }
            if (fieldName % "reconciliationType") {
                return reconciliationType;
            }
            break;
        case 's':
            if (fieldName % "spotPrice") {
                return double_2_Str(spotPrice, 5);
            }
            if (fieldName % "sender") {
                return addr_2_Str(sender);
            }
            if (fieldName % "signature") {
                return signature;
            }
            if (fieldName % "selfDestructIn") {
                return bni_2_Str(selfDestructIn);
            }
            if (fieldName % "selfDestructOut") {
                return bni_2_Str(selfDestructOut);
            }
            break;
        case 't':
            if (fieldName % "transactionIndex") {
                return uint_2_Str(transactionIndex);
            }
            if (fieldName % "transactionHash") {
                return hash_2_Str(transactionHash);
            }
            if (fieldName % "timestamp") {
                return ts_2_Str(timestamp);
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
bool CReconciliation::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "assetAddr") {
                assetAddr = str_2_Addr(fieldValue);
                return true;
            }
            if (fieldName % "assetSymbol") {
                assetSymbol = fieldValue;
                return true;
            }
            if (fieldName % "accountedFor") {
                accountedFor = str_2_Addr(fieldValue);
                return true;
            }
            if (fieldName % "amountIn") {
                amountIn = str_2_BigInt(fieldValue);
                return true;
            }
            if (fieldName % "amountOut") {
                amountOut = str_2_BigInt(fieldValue);
                return true;
            }
            break;
        case 'b':
            if (fieldName % "blockNumber") {
                blockNumber = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "begBal") {
                begBal = str_2_BigInt(fieldValue);
                return true;
            }
            break;
        case 'd':
            if (fieldName % "decimals") {
                decimals = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'e':
            if (fieldName % "endBal") {
                endBal = str_2_BigInt(fieldValue);
                return true;
            }
            if (fieldName % "encoding") {
                encoding = fieldValue;
                return true;
            }
            break;
        case 'g':
            if (fieldName % "gasOut") {
                gasOut = str_2_BigInt(fieldValue);
                return true;
            }
            break;
        case 'i':
            if (fieldName % "internalIn") {
                internalIn = str_2_BigInt(fieldValue);
                return true;
            }
            if (fieldName % "internalOut") {
                internalOut = str_2_BigInt(fieldValue);
                return true;
            }
            break;
        case 'l':
            if (fieldName % "logIndex") {
                logIndex = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'm':
            if (fieldName % "minerBaseRewardIn") {
                minerBaseRewardIn = str_2_BigInt(fieldValue);
                return true;
            }
            if (fieldName % "minerNephewRewardIn") {
                minerNephewRewardIn = str_2_BigInt(fieldValue);
                return true;
            }
            if (fieldName % "minerTxFeeIn") {
                minerTxFeeIn = str_2_BigInt(fieldValue);
                return true;
            }
            if (fieldName % "minerUncleRewardIn") {
                minerUncleRewardIn = str_2_BigInt(fieldValue);
                return true;
            }
            break;
        case 'p':
            if (fieldName % "priceSource") {
                priceSource = fieldValue;
                return true;
            }
            if (fieldName % "prefundIn") {
                prefundIn = str_2_BigInt(fieldValue);
                return true;
            }
            if (fieldName % "prevAppBlk") {
                prevAppBlk = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "prevBal") {
                prevBal = str_2_BigInt(fieldValue);
                return true;
            }
            break;
        case 'r':
            if (fieldName % "recipient") {
                recipient = str_2_Addr(fieldValue);
                return true;
            }
            if (fieldName % "reconciliationType") {
                reconciliationType = fieldValue;
                return true;
            }
            break;
        case 's':
            if (fieldName % "spotPrice") {
                spotPrice = str_2_Double(fieldValue);
                return true;
            }
            if (fieldName % "sender") {
                sender = str_2_Addr(fieldValue);
                return true;
            }
            if (fieldName % "signature") {
                signature = fieldValue;
                return true;
            }
            if (fieldName % "selfDestructIn") {
                selfDestructIn = str_2_BigInt(fieldValue);
                return true;
            }
            if (fieldName % "selfDestructOut") {
                selfDestructOut = str_2_BigInt(fieldValue);
                return true;
            }
            break;
        case 't':
            if (fieldName % "transactionIndex") {
                transactionIndex = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "transactionHash") {
                transactionHash = str_2_Hash(fieldValue);
                return true;
            }
            if (fieldName % "timestamp") {
                timestamp = str_2_Ts(fieldValue);
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CReconciliation::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CReconciliation::Serialize(CArchive& archive) {
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
    archive >> transactionIndex;
    archive >> logIndex;
    archive >> transactionHash;
    archive >> timestamp;
    archive >> assetAddr;
    archive >> assetSymbol;
    archive >> decimals;
    archive >> spotPrice;
    archive >> priceSource;
    archive >> accountedFor;
    archive >> sender;
    archive >> recipient;
    archive >> begBal;
    archive >> endBal;
    archive >> encoding;
    archive >> signature;
    archive >> reconciliationType;
    archive >> amountIn;
    archive >> internalIn;
    archive >> selfDestructIn;
    archive >> minerBaseRewardIn;
    archive >> minerNephewRewardIn;
    archive >> minerTxFeeIn;
    archive >> minerUncleRewardIn;
    archive >> prefundIn;
    archive >> amountOut;
    archive >> internalOut;
    archive >> selfDestructOut;
    archive >> gasOut;
    archive >> prevAppBlk;
    archive >> prevBal;
    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CReconciliation::SerializeC(CArchive& archive) const {
    // Writing always writes the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << blockNumber;
    archive << transactionIndex;
    archive << logIndex;
    archive << transactionHash;
    archive << timestamp;
    archive << assetAddr;
    archive << assetSymbol;
    archive << decimals;
    archive << spotPrice;
    archive << priceSource;
    archive << accountedFor;
    archive << sender;
    archive << recipient;
    archive << begBal;
    archive << endBal;
    archive << encoding;
    archive << signature;
    archive << reconciliationType;
    archive << amountIn;
    archive << internalIn;
    archive << selfDestructIn;
    archive << minerBaseRewardIn;
    archive << minerNephewRewardIn;
    archive << minerTxFeeIn;
    archive << minerUncleRewardIn;
    archive << prefundIn;
    archive << amountOut;
    archive << internalOut;
    archive << selfDestructOut;
    archive << gasOut;
    archive << prevAppBlk;
    archive << prevBal;
    // EXISTING_CODE
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CReconciliation::Migrate(CArchive& archiveIn, CArchive& archiveOut) const {
    ASSERT(archiveIn.isReading());
    ASSERT(archiveOut.isWriting());
    CReconciliation copy;
    // EXISTING_CODE
    // EXISTING_CODE
    copy.Serialize(archiveIn);
    copy.SerializeC(archiveOut);
    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CReconciliationArray& array) {
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
CArchive& operator<<(CArchive& archive, const CReconciliationArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CReconciliation::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CReconciliation, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CReconciliation, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CReconciliation, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CReconciliation, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CReconciliation, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CReconciliation, "blockNumber", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CReconciliation, "transactionIndex", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CReconciliation, "logIndex", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CReconciliation, "transactionHash", T_HASH | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CReconciliation, "timestamp", T_TIMESTAMP, ++fieldNum);
    ADD_FIELD(CReconciliation, "date", T_DATE, ++fieldNum);
    HIDE_FIELD(CReconciliation, "date");
    ADD_FIELD(CReconciliation, "assetAddr", T_ADDRESS | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CReconciliation, "assetSymbol", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CReconciliation, "decimals", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CReconciliation, "spotPrice", T_DOUBLE, ++fieldNum);
    ADD_FIELD(CReconciliation, "priceSource", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CReconciliation, "accountedFor", T_ADDRESS | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CReconciliation, "sender", T_ADDRESS | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CReconciliation, "recipient", T_ADDRESS | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CReconciliation, "begBal", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliation, "amountNet", T_INT256, ++fieldNum);
    HIDE_FIELD(CReconciliation, "amountNet");
    ADD_FIELD(CReconciliation, "endBal", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliation, "encoding", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CReconciliation, "signature", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CReconciliation, "reconciliationType", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CReconciliation, "reconciled", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CReconciliation, "reconciled");
    ADD_FIELD(CReconciliation, "totalIn", T_INT256, ++fieldNum);
    HIDE_FIELD(CReconciliation, "totalIn");
    ADD_FIELD(CReconciliation, "amountIn", T_INT256 | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CReconciliation, "internalIn", T_INT256 | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CReconciliation, "selfDestructIn", T_INT256 | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CReconciliation, "minerBaseRewardIn", T_INT256 | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CReconciliation, "minerNephewRewardIn", T_INT256 | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CReconciliation, "minerTxFeeIn", T_INT256 | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CReconciliation, "minerUncleRewardIn", T_INT256 | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CReconciliation, "prefundIn", T_INT256 | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CReconciliation, "totalOut", T_INT256, ++fieldNum);
    HIDE_FIELD(CReconciliation, "totalOut");
    ADD_FIELD(CReconciliation, "amountOut", T_INT256 | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CReconciliation, "internalOut", T_INT256 | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CReconciliation, "selfDestructOut", T_INT256 | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CReconciliation, "gasOut", T_INT256 | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CReconciliation, "totalOutLessGas", T_INT256, ++fieldNum);
    HIDE_FIELD(CReconciliation, "totalOutLessGas");
    ADD_FIELD(CReconciliation, "prevAppBlk", T_BLOCKNUM | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CReconciliation, "prevBal", T_INT256 | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CReconciliation, "begBalDiff", T_INT256 | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CReconciliation, "begBalDiff");
    ADD_FIELD(CReconciliation, "endBalDiff", T_INT256 | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CReconciliation, "endBalDiff");
    ADD_FIELD(CReconciliation, "endBalCalc", T_INT256 | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CReconciliation, "endBalCalc");

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CReconciliation, "schema");
    HIDE_FIELD(CReconciliation, "deleted");
    HIDE_FIELD(CReconciliation, "showing");
    HIDE_FIELD(CReconciliation, "cname");

    builtIns.push_back(_biCReconciliation);

    // EXISTING_CODE
    SET_TYPE(CReconciliation, "reconciled", T_BOOL);  // remove TS_OMITEMPTY
    UNHIDE_FIELD(CReconciliation, "date");
    UNHIDE_FIELD(CReconciliation, "amountNet");
    UNHIDE_FIELD(CReconciliation, "reconciled");
    UNHIDE_FIELD(CReconciliation, "totalIn");
    UNHIDE_FIELD(CReconciliation, "totalOut");
    UNHIDE_FIELD(CReconciliation, "totalOutLessGas");
    UNHIDE_FIELD(CReconciliation, "begBalDiff");
    UNHIDE_FIELD(CReconciliation, "endBalDiff");
    UNHIDE_FIELD(CReconciliation, "endBalCalc");
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextReconciliationChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CReconciliation* rec = reinterpret_cast<const CReconciliation*>(dataPtr);
    if (rec) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'a':
                if (fieldIn % "assetSymbol") {
                    if (!rec->assetSymbol.empty() && rec->assetSymbol != "ETH")
                        return rec->assetSymbol;
                    if (expContext().asEther) {
                        return "ETH";
                    }
                    return "WEI";
                }
                if (fieldIn % "amountIn") {
                    return bni_2_Export(rec->timestamp, rec->amountIn, rec->decimals);
                }
                if (fieldIn % "amountOut") {
                    return bni_2_Export(rec->timestamp, rec->amountOut, rec->decimals);
                }
                if (fieldIn % "amountNet") {
                    return bni_2_Export(rec->timestamp, rec->amountNet(), rec->decimals);
                }
                break;
            case 'b':
                if (fieldIn % "begBal") {
                    return bni_2_Export(rec->timestamp, rec->begBal, rec->decimals);
                }
                if (fieldIn % "begBalDiff") {
                    return bni_2_Export(rec->timestamp, rec->begBalDiff(), rec->decimals);
                }
                break;
            case 'd':
                if (fieldIn % "day") {
                    return ts_2_Date(rec->timestamp).Format(FMT_JSON).substr(0, 10);
                }
                if (fieldIn % "date") {
                    return ts_2_Date(rec->timestamp).Format(FMT_JSON);
                }
                break;
            case 'e':
                if (fieldIn % "endBal") {
                    return bni_2_Export(rec->timestamp, rec->endBal, rec->decimals);
                }
                if (fieldIn % "endBalCalc") {
                    return bni_2_Export(rec->timestamp, rec->endBalCalc(), rec->decimals);
                }
                if (fieldIn % "endBalDiff") {
                    return bni_2_Export(rec->timestamp, rec->endBalDiff(), rec->decimals);
                }
                break;
            case 'g':
                if (fieldIn % "gasOut") {
                    return bni_2_Export(rec->timestamp, rec->gasOut, rec->decimals);
                }
                break;
            case 'h':
                if (fieldIn % "hour") {
                    return BOH(ts_2_Date(rec->timestamp)).Format(FMT_EXPORT).substr(0, 13);
                }
                break;
            case 'i':
                if (fieldIn % "internalIn") {
                    return bni_2_Export(rec->timestamp, rec->internalIn, rec->decimals);
                }
                if (fieldIn % "internalOut") {
                    return bni_2_Export(rec->timestamp, rec->internalOut, rec->decimals);
                }
                if (fieldIn % "issuance") {
                    return bni_2_Export(rec->timestamp,
                                        rec->minerBaseRewardIn + rec->minerNephewRewardIn + rec->minerUncleRewardIn,
                                        18);
                }
                break;
            case 'm':
                if (fieldIn % "minerBaseRewardIn") {
                    return bni_2_Export(rec->timestamp, rec->minerBaseRewardIn, rec->decimals);
                }
                if (fieldIn % "minerNephewRewardIn") {
                    return bni_2_Export(rec->timestamp, rec->minerNephewRewardIn, rec->decimals);
                }
                if (fieldIn % "minerTxFeeIn") {
                    return bni_2_Export(rec->timestamp, rec->minerTxFeeIn, rec->decimals);
                }
                if (fieldIn % "minerUncleRewardIn") {
                    return bni_2_Export(rec->timestamp, rec->minerUncleRewardIn, rec->decimals);
                }
                if (fieldIn % "month") {
                    return ts_2_Date(rec->timestamp).Format(FMT_EXPORT).substr(0, 7);
                }
                if (fieldIn % "minerissuance") {
                    return bni_2_Export(rec->timestamp, rec->minerBaseRewardIn + rec->minerNephewRewardIn,
                                        rec->decimals);
                }
                break;
            case 'r':
                if (fieldIn % "reconciled") {
                    return bool_2_Str(rec->reconciled());
                }
                break;
            case 's':
                if (fieldIn % "selfDestructIn") {
                    return bni_2_Export(rec->timestamp, rec->selfDestructIn, rec->decimals);
                }
                if (fieldIn % "selfDestructOut") {
                    return bni_2_Export(rec->timestamp, rec->selfDestructOut, rec->decimals);
                }
                break;
            case 't':
                if (fieldIn % "totalIn") {
                    return bni_2_Export(rec->timestamp, rec->totalIn(), rec->decimals);
                }
                if (fieldIn % "totalOut") {
                    return bni_2_Export(rec->timestamp, rec->totalOut(), rec->decimals);
                }
                if (fieldIn % "totalOutLessGas") {
                    return bni_2_Export(rec->timestamp, (rec->totalOutLessGas()), rec->decimals);
                }
                if (fieldIn % "transactionHash" && rec->pTransaction) {
                    return rec->pTransaction->hash;
                }
                break;
            case 'w':
                if (fieldIn % "week") {
                    return BOW(ts_2_Date(rec->timestamp)).Format(FMT_EXPORT).substr(0, 10);
                }
                break;
            case 'y':
                if (fieldIn % "year") {
                    return ts_2_Date(rec->timestamp).Format(FMT_EXPORT).substr(0, 4);
                }
                break;
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, rec);
                // EXISTING_CODE
                if (fieldIn % "prefundIn") {
                    return bni_2_Export(rec->timestamp, rec->prefundIn, rec->decimals);
                }
                if (fieldIn % "prevBal") {
                    return bni_2_Export(rec->timestamp, rec->prevBal, rec->decimals);
                }
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
bool CReconciliation::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    if (m_schema <= getVersionNum(0, 11, 4)) {
        LOG_ERR("Reconcilation file is too old. Delete all reconciliations from the cache.");
        quickQuitHandler(1);

    } else if (m_schema < getVersionNum(0, 42, 6)) {
        archive >> blockNumber;
        archive >> transactionIndex;
        archive >> timestamp;
        archive >> assetAddr;
        archive >> assetSymbol;
        archive >> decimals;
        archive >> prevAppBlk;
        archive >> prevBal;
        archive >> begBal;
        archive >> endBal;
        archive >> amountIn;
        archive >> internalIn;
        archive >> selfDestructIn;
        archive >> minerBaseRewardIn;
        archive >> minerNephewRewardIn;
        archive >> minerTxFeeIn;
        archive >> minerUncleRewardIn;
        archive >> prefundIn;
        archive >> amountOut;
        archive >> internalOut;
        archive >> selfDestructOut;
        archive >> gasOut;
        archive >> reconciliationType;
        archive >> spotPrice;
        archive >> priceSource;

        bool isEth = assetSymbol == "ETH" || assetSymbol == "WEI" || assetSymbol.empty();
        spotPrice = getPriceInUsd(isEth ? FAKE_ETH_ADDRESS : assetAddr, priceSource, blockNumber);
        finishParse();
        done = true;
    }
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CReconciliation& rec) {
    rec.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CReconciliation& rec) {
    rec.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CReconciliation& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_RECONCILIATION =
    "[{BLOCKNUMBER}]\t"
    "[{TRANSACTIONINDEX}]\t"
    "[{LOGINDEX}]\t"
    "[{TRANSACTIONHASH}]\t"
    "[{TIMESTAMP}]\t"
    "[{DATE}]\t"
    "[{ASSETADDR}]\t"
    "[{ASSETSYMBOL}]\t"
    "[{DECIMALS}]\t"
    "[{SPOTPRICE}]\t"
    "[{PRICESOURCE}]\t"
    "[{ACCOUNTEDFOR}]\t"
    "[{SENDER}]\t"
    "[{RECIPIENT}]\t"
    "[{BEGBAL}]\t"
    "[{AMOUNTNET}]\t"
    "[{ENDBAL}]\t"
    "[{ENCODING}]\t"
    "[{SIGNATURE}]\t"
    "[{RECONCILIATIONTYPE}]\t"
    "[{RECONCILED}]\t"
    "[{TOTALIN}]\t"
    "[{AMOUNTIN}]\t"
    "[{INTERNALIN}]\t"
    "[{SELFDESTRUCTIN}]\t"
    "[{MINERBASEREWARDIN}]\t"
    "[{MINERNEPHEWREWARDIN}]\t"
    "[{MINERTXFEEIN}]\t"
    "[{MINERUNCLEREWARDIN}]\t"
    "[{PREFUNDIN}]\t"
    "[{TOTALOUT}]\t"
    "[{AMOUNTOUT}]\t"
    "[{INTERNALOUT}]\t"
    "[{SELFDESTRUCTOUT}]\t"
    "[{GASOUT}]\t"
    "[{TOTALOUTLESSGAS}]\t"
    "[{PREVAPPBLK}]\t"
    "[{PREVBAL}]\t"
    "[{BEGBALDIFF}]\t"
    "[{ENDBALDIFF}]\t"
    "[{ENDBALCALC}]";

//---------------------------------------------------------------------------
// EXISTING_CODE
//-----------------------------------------------------------------------
static string_q wei_2_Ether_local(const wei_t& weiIn, uint64_t decimals) {
    string_q ret = str_2_Ether(bnu_2_Str(weiIn), decimals);
    if (contains(ret, "."))
        ret = trimTrailing(ret, '0');
    return trimTrailing(ret, '.');
}

//--------------------------------------------------------------------------------
string_q wei_2_Str(const wei_t& weiIn) {
    return bnu_2_Str(weiIn);
}

//-----------------------------------------------------------------------
string_q wei_2_Ether(const wei_t& weiIn, uint64_t decimals) {
    return str_2_Ether(bnu_2_Str(weiIn), decimals);
}

//--------------------------------------------------------------------------------
string_q bni_2_Str(const bigint_t& num) {
    return (num.isNegative() ? string_q("-") : "") + bnu_2_Str(num.getMagnitude());
}

//---------------------------------------------------------------------------
string_q bni_2_Ether(const bigint_t& num, uint64_t decimals) {
    if (num == 0)
        return "";

    bigint_t n = num;
    bool negative = false;
    if (n < 0) {
        negative = true;
        n = n * -1;
    }

    static uint64_t round = NOPOS;
    if (round == NOPOS) {
        round = getGlobalConfig("acctExport")->getConfigInt("settings", "ether_rounding", 18);
    }
    string_q ret = wei_2_Ether_local(str_2_Wei(bni_2_Str(n)), decimals);
    CStringArray parts;
    explode(parts, ret, '.');
    ret = parts[0] + ".";
    if (parts.size() == 1)
        return (negative ? "-" : "") + ret + "0000000";
    if (parts[1].length() >= round)
        return (negative ? "-" : "") + ret + parts[1].substr(0, round);
    return (negative ? "-" : "") + ret + parts[1] + string_q(round - parts[1].length(), '0');
}

//--------------------------------------------------------------------------------
string_q wei_2_Export(const blknum_t& bn, const wei_t& weiIn, uint64_t decimals) {
    string_q ret;
    if (weiIn != 0) {
        if (expContext().asEther) {
            ret = wei_2_Ether_local(weiIn, decimals);
        } else {
            ret = wei_2_Str(weiIn);
        }
    }
    return "\"" + ret + "\"";
}

//---------------------------------------------------------------------------
string_q bni_2_Export(const timestamp_t& ts, const bigint_t& numIn, uint64_t decimals) {
    string_q quote = "\"";
    if (expContext().exportFmt == CSV1)
        quote = "";  // will be quoted automatically

    if (numIn == 0)
        return quote + quote;
    if (expContext().asEther) {
        return quote + bni_2_Ether(numIn, decimals) + quote;
    } else {
        return quote + bni_2_Str(numIn) + quote;
    }
}

//--------------------------------------------------------------
CReconciliation operator+(const CReconciliation& a, const CReconciliation& b) {
    CReconciliation rec = a;
    rec.blockNumber = b.blockNumber;            // assign
    rec.transactionIndex = b.transactionIndex;  // assign
    rec.timestamp = b.timestamp;                // assign
    rec.amountIn += b.amountIn;
    rec.internalIn += b.internalIn;
    rec.selfDestructIn += b.selfDestructIn;
    rec.minerBaseRewardIn += b.minerBaseRewardIn;
    rec.minerNephewRewardIn += b.minerNephewRewardIn;
    rec.minerTxFeeIn += b.minerTxFeeIn;
    rec.minerUncleRewardIn += b.minerUncleRewardIn;
    rec.prefundIn += b.prefundIn;
    rec.amountOut += b.amountOut;
    rec.internalOut += b.internalOut;
    rec.selfDestructOut += b.selfDestructOut;
    rec.gasOut += b.gasOut;
    return rec;
}

//---------------------------------------------------------------------------
CReconciliation& CReconciliation::operator+=(const CReconciliation& r) {
    CReconciliation prev = *this;
    *this = *this + r;
    reconciliationType = "summary";
    prevAppBlk = prev.prevAppBlk;
    prevBal = prev.prevBal;
    begBal = prev.begBal;
    return *this;
}

//---------------------------------------------------------------------------
CReconciliation::CReconciliation(const address_t& aF, const address_t& asset, const CTransaction* pT) {
    initialize();
    blockNumber = pT->blockNumber;
    transactionIndex = pT->transactionIndex;
    timestamp = pT->timestamp;
    pTransaction = pT;
    accountedFor = aF;
    assetAddr = asset;
    transactionHash = pT->hash;
}
// EXISTING_CODE
}  // namespace qblocks
