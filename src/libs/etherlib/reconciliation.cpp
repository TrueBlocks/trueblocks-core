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
static string_q nextReconciliationChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextReconciliationChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CReconciliation::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["reconciliation_fmt"] : fmtIn);
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
            break;
        case 'g':
            if (fieldName % "gasCostOut") {
                return bni_2_Str(gasCostOut);
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
            if (fieldName % "prevBlk") {
                return uint_2_Str(prevBlk);
            }
            if (fieldName % "prevBlkBal") {
                return bni_2_Str(prevBlkBal);
            }
            if (fieldName % "prefundIn") {
                return bni_2_Str(prefundIn);
            }
            break;
        case 'r':
            if (fieldName % "reconciliationType") {
                return reconciliationType;
            }
            break;
        case 's':
            if (fieldName % "selfDestructIn") {
                return bni_2_Str(selfDestructIn);
            }
            if (fieldName % "selfDestructOut") {
                return bni_2_Str(selfDestructOut);
            }
            if (fieldName % "spotPrice") {
                return bni_2_Str(spotPrice);
            }
            break;
        case 't':
            if (fieldName % "transactionIndex") {
                return uint_2_Str(transactionIndex);
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
            break;
        case 'g':
            if (fieldName % "gasCostOut") {
                gasCostOut = str_2_BigInt(fieldValue);
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
            if (fieldName % "prevBlk") {
                prevBlk = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "prevBlkBal") {
                prevBlkBal = str_2_BigInt(fieldValue);
                return true;
            }
            if (fieldName % "prefundIn") {
                prefundIn = str_2_BigInt(fieldValue);
                return true;
            }
            break;
        case 'r':
            if (fieldName % "reconciliationType") {
                reconciliationType = fieldValue;
                return true;
            }
            break;
        case 's':
            if (fieldName % "selfDestructIn") {
                selfDestructIn = str_2_BigInt(fieldValue);
                return true;
            }
            if (fieldName % "selfDestructOut") {
                selfDestructOut = str_2_BigInt(fieldValue);
                return true;
            }
            if (fieldName % "spotPrice") {
                spotPrice = str_2_BigInt(fieldValue);
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
    archive >> timestamp;
    archive >> assetAddr;
    archive >> assetSymbol;
    archive >> decimals;
    archive >> prevBlk;
    archive >> prevBlkBal;
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
    archive >> gasCostOut;
    archive >> reconciliationType;
    archive >> spotPrice;
    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CReconciliation::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << blockNumber;
    archive << transactionIndex;
    archive << timestamp;
    archive << assetAddr;
    archive << assetSymbol;
    archive << decimals;
    archive << prevBlk;
    archive << prevBlkBal;
    archive << begBal;
    archive << endBal;
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
    archive << gasCostOut;
    archive << reconciliationType;
    archive << spotPrice;
    // EXISTING_CODE
    // EXISTING_CODE
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
    ADD_FIELD(CReconciliation, "timestamp", T_TIMESTAMP, ++fieldNum);
    ADD_FIELD(CReconciliation, "assetAddr", T_ADDRESS | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CReconciliation, "assetSymbol", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CReconciliation, "decimals", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CReconciliation, "prevBlk", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CReconciliation, "prevBlkBal", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliation, "begBal", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliation, "endBal", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliation, "amountIn", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliation, "internalIn", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliation, "selfDestructIn", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliation, "minerBaseRewardIn", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliation, "minerNephewRewardIn", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliation, "minerTxFeeIn", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliation, "minerUncleRewardIn", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliation, "prefundIn", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliation, "amountOut", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliation, "internalOut", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliation, "selfDestructOut", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliation, "gasCostOut", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliation, "reconciliationType", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CReconciliation, "spotPrice", T_INT256, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CReconciliation, "schema");
    HIDE_FIELD(CReconciliation, "deleted");
    HIDE_FIELD(CReconciliation, "showing");
    HIDE_FIELD(CReconciliation, "cname");

    builtIns.push_back(_biCReconciliation);

    // EXISTING_CODE
    SET_TYPE(CReconciliation, "spotPrice", T_INT256 | TS_OMITEMPTY);
    ADD_FIELD(CReconciliation, "begBalDiff", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliation, "endBalCalc", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliation, "endBalDiff", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliation, "totalIn", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliation, "totalOut", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliation, "totalOutLessGas", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliation, "amountNet", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliation, "reconciled", T_BOOL | TS_OMITEMPTY, ++fieldNum);
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
                    } else if (expContext().asDollars) {
                        return "USD";
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
                if (fieldIn % "gasCostOut") {
                    return bni_2_Export(rec->timestamp, rec->gasCostOut, rec->decimals);
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
                if (fieldIn % "spotPrice") {
                    if (rec->spotPrice == int64_t(-1))
                        return "\"-1\"";
                    return bni_2_Export(rec->timestamp, rec->spotPrice, rec->decimals);
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
                if (fieldIn % "prevBlkBal") {
                    return bni_2_Export(rec->timestamp, rec->prevBlkBal, rec->decimals);
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
bool CReconciliation::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    bigint_t unused;
    bool unusedBool;
    if (m_schema < getVersionNum(0, 10, 1)) {
        archive >> blockNumber;
        archive >> transactionIndex;
        archive >> timestamp;
        archive >> assetAddr;
        archive >> assetSymbol;
        archive >> decimals;
        // archive >> prevBlk;
        // archive >> prevBlkBal;
        archive >> begBal;
        archive >> unused;  // begBalDiff
        archive >> amountIn;
        archive >> amountOut;
        archive >> internalIn;
        archive >> internalOut;
        archive >> selfDestructIn;
        archive >> selfDestructOut;
        archive >> minerBaseRewardIn;
        archive >> minerNephewRewardIn;
        archive >> minerTxFeeIn;
        archive >> minerUncleRewardIn;
        archive >> prefundIn;
        archive >> gasCostOut;
        archive >> endBal;
        archive >> unused;  // endBalCalc;
        archive >> unused;  // endBalDiff;
        archive >> unused;  // amountNet;
        // archive >> spotPrice;  // not present before
        archive >> reconciliationType;
        archive >> unusedBool;  // reconciled;
        spotPrice = -1;
        finishParse();
        done = true;
    } else if (m_schema < getVersionNum(0, 10, 2)) {
        archive >> blockNumber;
        archive >> transactionIndex;
        archive >> timestamp;
        archive >> assetAddr;
        archive >> assetSymbol;
        archive >> decimals;
        // archive >> prevBlk;
        // archive >> prevBlkBal;
        archive >> begBal;
        archive >> unused;  // begBalDiff;
        archive >> amountIn;
        archive >> amountOut;
        archive >> internalIn;
        archive >> internalOut;
        archive >> selfDestructIn;
        archive >> selfDestructOut;
        archive >> minerBaseRewardIn;
        archive >> minerNephewRewardIn;
        archive >> minerTxFeeIn;
        archive >> minerUncleRewardIn;
        archive >> prefundIn;
        archive >> gasCostOut;
        archive >> endBal;
        archive >> unused;  // endBalCalc;
        archive >> unused;  // endBalDiff;
        archive >> unused;  // amountNet;
        archive >> spotPrice;
        archive >> reconciliationType;
        // archive >> reconTrail;
        archive >> unusedBool;  // reconciled;
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
    "[{ASSETADDR}]\t"
    "[{ASSETSYMBOL}]\t"
    "[{DECIMALS}]\t"
    "[{BLOCKNUMBER}]\t"
    "[{TRANSACTIONINDEX}]\t"
    "[{TIMESTAMP}]\t"
    "[{PREVBLK}]\t"
    "[{PREVBLKBAL}]\t"
    "[{BEGBAL}]\t"
    "[{AMOUNTIN}]\t"
    "[{INTERNALIN}]\t"
    "[{SELFDESTRUCTIN}]\t"
    "[{MINERBASEREWARDIN}]\t"
    "[{MINERNEPHEWREWARDIN}]\t"
    "[{MINERTXFEEIN}]\t"
    "[{MINERUNCLEREWARDIN}]\t"
    "[{PREFUNDIN}]\t"
    "[{TOTALIN}]\t"
    "[{AMOUNTOUT}]\t"
    "[{INTERNALOUT}]\t"
    "[{SELFDESTRUCTOUT}]\t"
    "[{GASCOSTOUT}]\t"
    "[{TOTALOUT}]\t"
    "[{TOTALOUTLESSGAS}]\t"
    "[{AMOUNTNET}]\t"
    "[{ENDBAL}]\t"
    "[{BEGBALDIFF}]\t"
    "[{ENDBALCALC}]\t"
    "[{ENDBALDIFF}]\t"
    "[{RECONCILED}]\t"
    "[{SPOTPRICE}]\t"
    "[{RECONCILIATIONTYPE}]\t"
    "[{RECONTRAIL}]";

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

//---------------------------------------------------------------------------
string_q bni_2_Dollars(const timestamp_t& ts, const bigint_t& numIn, uint64_t decimals) {
    if (numIn == 0)
        return "";
    bigint_t n = numIn;
    bool negative = false;
    if (n < 0) {
        negative = true;
        n = n * -1;
    }
    return (negative ? "-" : "") + wei_2_Dollars(ts, str_2_Wei(bni_2_Str(n)), decimals);
}

//--------------------------------------------------------------------------------
string_q wei_2_Export(const blknum_t& bn, const wei_t& weiIn, uint64_t decimals) {
    string_q ret;
    if (weiIn != 0) {
        if (expContext().asEther) {
            ret = wei_2_Ether_local(weiIn, decimals);
        } else if (expContext().asDollars) {
            static map<blknum_t, timestamp_t> timestampMap;
            if (timestampMap[bn] == (timestamp_t)0) {
                CBlock blk;
                getBlock_light(blk, bn);
                timestampMap[bn] = blk.timestamp;
            }
            ret = wei_2_Dollars(timestampMap[bn], weiIn, decimals);
        } else {
            ret = wei_2_Str(weiIn);
        }
    }
    return "\"" + ret + "\"";
}

//---------------------------------------------------------------------------
string_q bni_2_Export(const timestamp_t& ts, const bigint_t& numIn, uint64_t decimals) {
    if (numIn == 0)
        return "\"\"";
    if (expContext().asEther) {
        return "\"" + bni_2_Ether(numIn, decimals) + "\"";
    } else if (expContext().asDollars) {
        return "\"" + bni_2_Dollars(ts, numIn, decimals) + "\"";
    } else {
        return "\"" + bni_2_Str(numIn) + "\"";
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
    rec.gasCostOut += b.gasCostOut;
    return rec;
}

//---------------------------------------------------------------------------
CReconciliation& CReconciliation::operator+=(const CReconciliation& r) {
    CReconciliation prev = *this;
    *this = *this + r;
    reconciliationType = "summary";
    prevBlk = prev.prevBlk;
    prevBlkBal = prev.prevBlkBal;
    begBal = prev.begBal;
    return *this;
}

//---------------------------------------------------------------------------
void CReconciliation::initForToken(CAccountName& tokenName) {
    assetAddr = tokenName.address;
    ASSERT(!assetAddr.empty());
    assetSymbol = tokenName.symbol;
    if (assetSymbol.empty())
        assetSymbol = getTokenSymbol(tokenName.address, blockNumber);
    if (assetSymbol.empty())
        assetSymbol = tokenName.address.substr(0, 4);
    decimals = tokenName.decimals != 0 ? tokenName.decimals : 18;
}

#define LOG_TRIAL_BALANCE()                                                                                            \
    LOG4("Trial balance: ", reconciliationType);                                                                       \
    LOG4("  hash: ", trans->hash);                                                                                     \
    LOG4("  ------------------------------");                                                                          \
    LOG4("  prevBal:       ", prevBlkBal);                                                                             \
    LOG4("  begBal:        ", begBal);                                                                                 \
    LOG4("  begBalDiff:    ", begBalDiff());                                                                           \
    LOG4("  ------------------------------");                                                                          \
    LOG8("  amountIn:      ", amountIn);                                                                               \
    LOG8("  internalIn:    ", internalIn);                                                                             \
    LOG8("  slfDstrctIn:   ", selfDestructIn);                                                                         \
    LOG8("  minBRwdIn:     ", minerBaseRewardIn);                                                                      \
    LOG8("  minNRwdIn:     ", minerNephewRewardIn);                                                                    \
    LOG8("  minTxFeeIn:    ", minerTxFeeIn);                                                                           \
    LOG8("  minURwdIn:     ", minerUncleRewardIn);                                                                     \
    LOG8("  prefundIn:     ", prefundIn);                                                                              \
    LOG4("  totalIn:       ", totalIn());                                                                              \
    LOG8("  amountOut:     ", amountOut);                                                                              \
    LOG8("  internalOut:   ", internalOut);                                                                            \
    LOG8("  slfDstrctOt:   ", selfDestructOut);                                                                        \
    LOG8("  gasCostOut:    ", gasCostOut);                                                                             \
    LOG4("  totalOut:      ", totalOut());                                                                             \
    LOG4("  amountNet:     ", amountNet());                                                                            \
    LOG4("  endBal:        ", endBal);                                                                                 \
    LOG4("  ------------------------------");                                                                          \
    LOG4("  endBalCalc:    ", endBalCalc());                                                                           \
    LOG4("  endBalDiff:    ", endBalDiff());                                                                           \
    LOG4("  regular-recon: ", reconciled() ? "true" : "false");

//-----------------------------------------------------------------------
bool CReconciliation::reconcileEth(const CReconciliation& prevRecon, blknum_t nextBlock, const CTransaction* trans,
                                   const CAccountName& accountedFor) {
    address_t acctFor = accountedFor.address;
    prevBlkBal = prevRecon.endBal;
    prevBlk = prevRecon.blockNumber;
    assetSymbol = "ETH";
    assetAddr = acctFor;

    bigint_t balEOLB = getBalanceAt(acctFor, blockNumber == 0 ? 0 : blockNumber - 1);
    bigint_t balEOB = getBalanceAt(acctFor, blockNumber);

    begBal = balEOLB;
    endBal = balEOB;

    if (trans->from == acctFor) {
        amountOut = trans->isError ? 0 : trans->value;
        gasCostOut = str_2_BigInt(trans->getValueByName("gasCost"));
    }

    if (trans->to == acctFor) {
        if (trans->from == "0xPrefund") {
            prefundIn = trans->value;
        } else if (trans->from == "0xBlockReward") {
            minerBaseRewardIn = trans->value;
            minerNephewRewardIn = trans->extraValue1;
            minerTxFeeIn = trans->extraValue2;
        } else if (trans->from == "0xUncleReward") {
            minerUncleRewardIn = trans->value;
        } else {
            amountIn = trans->isError ? 0 : trans->value;
        }
    }

    bool prevDifferent = prevRecon.blockNumber != blockNumber;
    bool nextDifferent = blockNumber != nextBlock;
    if (trans->blockNumber == 0) {
        reconciliationType = "genesis";

    } else {
        if (prevDifferent && nextDifferent) {
            reconciliationType = "regular";

        } else if (prevDifferent) {
            reconciliationType = "prevdiff-partial";

        } else if (nextDifferent) {
            reconciliationType = "partial-nextdiff";

        } else {
            reconciliationType = "partial-partial";
        }
    }

    LOG_TRIAL_BALANCE();
    if (reconciled())
        return true;

    // Reconciliation failed, let's try to reconcile by traces
    if (reconcileUsingTraces(prevRecon.endBal, trans, accountedFor))
        return true;

    // Reconciliation by traces failed, we want to correct for that and try
    // one more method - intra block.
    if (prevDifferent && nextDifferent) {
        // The trace reconcile may have changed values
        begBal = balEOLB;
        endBal = balEOB;
        reconciliationType = "regular";

    } else if (prevDifferent) {
        // This tx has a tx after it in the same block but none before it
        begBal = balEOLB;
        endBal = endBalCalc();
        reconciliationType = "prevdiff-partial";

    } else if (nextDifferent) {
        // This tx has a tx before it in the block but none after it
        begBal = prevRecon.endBal;
        endBal = balEOB;
        reconciliationType = "partial-nextdiff";

    } else {
        // this tx has both a tx before it and one after it in the same block
        begBal = prevRecon.endBal;
        endBal = endBalCalc();
        reconciliationType = "partial-partial";
    }

    LOG_TRIAL_BALANCE();
    return reconciled();
}

//---------------------------------------------------------------------------
bool CReconciliation::reconcileUsingTraces(bigint_t prevEndBal, const CTransaction* trans,
                                           const CAccountName& accountedFor) {
    address_t acctFor = accountedFor.address;
    amountOut = amountIn = 0;
    prefundIn = minerBaseRewardIn = minerNephewRewardIn = minerTxFeeIn + minerUncleRewardIn = 0;

    if (trans->blockNumber == 0) {
        begBal = 0;
        prefundIn = trans->value;
    } else {
        if (trans->traces.size() == 0) {
            blknum_t bn = trans->blockNumber;
            blknum_t txid = trans->transactionIndex;
            loadTraces(*((CTransaction*)trans), bn, txid, false, false);  // NOLINT
        }
        for (auto trace : trans->traces) {
            if (!trace.action.selfDestructed.empty()) {
                // do not collapse
                if (trace.action.refundAddress == acctFor)
                    selfDestructIn += trace.action.balance;
                // do not collapse
                if (trace.action.selfDestructed == acctFor)
                    selfDestructOut += trace.action.balance;
            } else {
                if (trace.action.from == acctFor) {
                    // Sometimes, EOAs appear here, but there is no way
                    // that a trace can initiate an expenditure on an EOA
                    // TODO(tjayrush): unless it's the first trace?
                    // unless the EOA initiated the top level tx. I think
                    // this might be a bug in a smart contract or something.
                    if (accountedFor.is_contract || trans->from == acctFor) {
                        internalOut += trans->isError ? 0 : trace.action.value;
                    }
                }

                if (trace.action.to == acctFor) {
                    if (trans->from == "0xPrefund") {
                        prefundIn = trans->value;
                    } else if (trans->from == "0xBlockReward") {
                        minerBaseRewardIn = trans->value;
                        minerNephewRewardIn = trans->extraValue1;
                        minerTxFeeIn = trans->extraValue2;
                    } else if (trans->from == "0xUncleReward") {
                        minerUncleRewardIn = trans->value;
                    } else {
                        internalIn += trans->isError ? 0 : trace.action.value;
                    }
                }
            }
        }
    }

    reconciliationType = "by-trace";
    LOG_TRIAL_BALANCE();
    if (!reconciled())
        ((CTransaction*)trans)->traces.clear();  // NOLINT
    return reconciled();
}

//-----------------------------------------------------------------------
bigint_t CReconciliation::totalIn(void) const {
    return amountIn + internalIn + selfDestructIn + prefundIn + minerBaseRewardIn + minerNephewRewardIn + minerTxFeeIn +
           minerUncleRewardIn;
}

//-----------------------------------------------------------------------
bigint_t CReconciliation::totalOut(void) const {
    return amountOut + internalOut + selfDestructOut + gasCostOut;
}

//-----------------------------------------------------------------------
bigint_t CReconciliation::totalOutLessGas(void) const {
    return amountOut + internalOut + selfDestructOut;
}

//---------------------------------------------------------------------------
bigint_t CReconciliation::begBalDiff(void) const {
    return blockNumber == 0 ? 0 : begBal - prevBlkBal;
}

//---------------------------------------------------------------------------
bigint_t CReconciliation::endBalCalc(void) const {
    return begBal + amountNet();
}

//---------------------------------------------------------------------------
bigint_t CReconciliation::endBalDiff(void) const {
    return endBalCalc() - endBal;
}

//---------------------------------------------------------------------------
bool CReconciliation::reconciled(void) const {
    return (endBalDiff() == 0 && begBalDiff() == 0);
}

//---------------------------------------------------------------------------
bigint_t CReconciliation::amountNet(void) const {
    return totalIn() - totalOut();
}
// EXISTING_CODE
}  // namespace qblocks
