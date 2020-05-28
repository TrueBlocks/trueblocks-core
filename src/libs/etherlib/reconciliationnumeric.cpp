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
#include <algorithm>
#include "reconciliationnumeric.h"
#include "etherlib.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CReconciliationNumeric, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextReconciliationnumericChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextReconciliationnumericChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CReconciliationNumeric::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["reconciliationnumeric_fmt"] : fmtIn);
    if (fmt.empty()) {
        toJson(ctx);
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextReconciliationnumericChunk, this);
}

//---------------------------------------------------------------------------
string_q nextReconciliationnumericChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CReconciliationNumeric*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CReconciliationNumeric::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextReconciliationnumericChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "asset") {
                return asset;
            }
            break;
        case 'b':
            if (fieldName % "blockNum") {
                return uint_2_Str(blockNum);
            }
            if (fieldName % "begBal") {
                return bni_2_Str(begBal);
            }
            if (fieldName % "begBalDiff") {
                return bni_2_Str(begBalDiff);
            }
            break;
        case 'e':
            if (fieldName % "endBal") {
                return bni_2_Str(endBal);
            }
            if (fieldName % "endBalCalc") {
                return bni_2_Str(endBalCalc);
            }
            if (fieldName % "endBalDiff") {
                return bni_2_Str(endBalDiff);
            }
            break;
        case 'i':
            if (fieldName % "inflow") {
                return bni_2_Str(inflow);
            }
            if (fieldName % "intInflow") {
                return bni_2_Str(intInflow);
            }
            if (fieldName % "intOutflow") {
                return bni_2_Str(intOutflow);
            }
            break;
        case 'o':
            if (fieldName % "outflow") {
                return bni_2_Str(outflow);
            }
            break;
        case 'r':
            if (fieldName % "reconciliationType") {
                return reconciliationType;
            }
            if (fieldName % "reconciled") {
                return bool_2_Str(reconciled);
            }
            break;
        case 's':
            if (fieldName % "suicideInflow") {
                return bni_2_Str(suicideInflow);
            }
            if (fieldName % "suicideOutflow") {
                return bni_2_Str(suicideOutflow);
            }
            break;
        case 'w':
            if (fieldName % "weiGasCost") {
                return bni_2_Str(weiGasCost);
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
bool CReconciliationNumeric::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "asset") {
                asset = fieldValue;
                return true;
            }
            break;
        case 'b':
            if (fieldName % "blockNum") {
                blockNum = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "begBal") {
                begBal = str_2_Wei(fieldValue);
                return true;
            }
            if (fieldName % "begBalDiff") {
                begBalDiff = str_2_Wei(fieldValue);
                return true;
            }
            break;
        case 'e':
            if (fieldName % "endBal") {
                endBal = str_2_Wei(fieldValue);
                return true;
            }
            if (fieldName % "endBalCalc") {
                endBalCalc = str_2_Wei(fieldValue);
                return true;
            }
            if (fieldName % "endBalDiff") {
                endBalDiff = str_2_Wei(fieldValue);
                return true;
            }
            break;
        case 'i':
            if (fieldName % "inflow") {
                inflow = str_2_Wei(fieldValue);
                return true;
            }
            if (fieldName % "intInflow") {
                intInflow = str_2_Wei(fieldValue);
                return true;
            }
            if (fieldName % "intOutflow") {
                intOutflow = str_2_Wei(fieldValue);
                return true;
            }
            break;
        case 'o':
            if (fieldName % "outflow") {
                outflow = str_2_Wei(fieldValue);
                return true;
            }
            break;
        case 'r':
            if (fieldName % "reconciliationType") {
                reconciliationType = fieldValue;
                return true;
            }
            if (fieldName % "reconciled") {
                reconciled = str_2_Bool(fieldValue);
                return true;
            }
            break;
        case 's':
            if (fieldName % "suicideInflow") {
                suicideInflow = str_2_Wei(fieldValue);
                return true;
            }
            if (fieldName % "suicideOutflow") {
                suicideOutflow = str_2_Wei(fieldValue);
                return true;
            }
            break;
        case 'w':
            if (fieldName % "weiGasCost") {
                weiGasCost = str_2_Wei(fieldValue);
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CReconciliationNumeric::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CReconciliationNumeric::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> blockNum;
    archive >> asset;
    archive >> begBal;
    archive >> begBalDiff;
    archive >> inflow;
    archive >> outflow;
    archive >> intInflow;
    archive >> intOutflow;
    archive >> suicideInflow;
    archive >> suicideOutflow;
    archive >> weiGasCost;
    archive >> endBal;
    archive >> endBalCalc;
    archive >> endBalDiff;
    archive >> reconciliationType;
    archive >> reconciled;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CReconciliationNumeric::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << blockNum;
    archive << asset;
    archive << begBal;
    archive << begBalDiff;
    archive << inflow;
    archive << outflow;
    archive << intInflow;
    archive << intOutflow;
    archive << suicideInflow;
    archive << suicideOutflow;
    archive << weiGasCost;
    archive << endBal;
    archive << endBalCalc;
    archive << endBalDiff;
    archive << reconciliationType;
    archive << reconciled;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CReconciliationNumericArray& array) {
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
CArchive& operator<<(CArchive& archive, const CReconciliationNumericArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CReconciliationNumeric::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CReconciliationNumeric, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CReconciliationNumeric, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CReconciliationNumeric, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CReconciliationNumeric, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CReconciliationNumeric, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CReconciliationNumeric, "blockNum", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CReconciliationNumeric, "asset", T_TEXT, ++fieldNum);
    ADD_FIELD(CReconciliationNumeric, "begBal", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliationNumeric, "begBalDiff", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliationNumeric, "inflow", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliationNumeric, "outflow", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliationNumeric, "intInflow", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliationNumeric, "intOutflow", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliationNumeric, "suicideInflow", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliationNumeric, "suicideOutflow", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliationNumeric, "weiGasCost", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliationNumeric, "endBal", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliationNumeric, "endBalCalc", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliationNumeric, "endBalDiff", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliationNumeric, "reconciliationType", T_TEXT, ++fieldNum);
    ADD_FIELD(CReconciliationNumeric, "reconciled", T_BOOL, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CReconciliationNumeric, "schema");
    HIDE_FIELD(CReconciliationNumeric, "deleted");
    HIDE_FIELD(CReconciliationNumeric, "showing");
    HIDE_FIELD(CReconciliationNumeric, "cname");

    builtIns.push_back(_biCReconciliationNumeric);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextReconciliationnumericChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CReconciliationNumeric* rec = reinterpret_cast<const CReconciliationNumeric*>(dataPtr);
    if (rec) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, rec);
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
bool CReconciliationNumeric::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CReconciliationNumeric& rec) {
    rec.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CReconciliationNumeric& rec) {
    rec.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CReconciliationNumeric& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_RECONCILIATIONNUMERIC = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
//---------------------------------------------------------------------------
bool CReconciliationNumeric::reconcile(const CStringArray& corrections, const CReconciliationNumeric& lastStatement,
                                       const address_t& accountingFor, blknum_t nextBlock, const CTransaction* trans) {
    asset = "ETH";

    // Note: In the case of an error, we need to account for gas usage if the account is the transaction's sender
    //
    // Note: There are many complications with reconcilation. To start, we do a top level reconcilation
    // and if it works, we return...

    // Case 1: We try to reconcile from top line (i.e. transaction level) data...
    reconciliationType = "";

    // We need to account for both the case where the account is the sender...
    if (trans->from == accountingFor) {
        outflow = trans->isError ? 0 : trans->value;
        weiGasCost = str_2_BigInt(trans->getValueByName("gasCost"));
    }

    // ... and/or the receiver...
    if (trans->to == accountingFor) {
        inflow = trans->isError ? 0 : trans->value;
    }

    // Ask the node what it thinks the balances are...
    begBal = getBalanceAt(accountingFor, blockNum == 0 ? 0 : blockNum - 1);
    endBal = getBalanceAt(accountingFor, blockNum);

    // If the user has given us corrections, use them...
    if (corrections.size() > 0) {
        reconciliationType = "correction";
        for (auto correction : corrections) {
            CStringArray parts;
            explode(parts, correction, '|');
            setValueByName(parts[0], parts[1]);
        }
    }

    // Calculate what we think the balances should be...
    endBalCalc = begBal + inflow + intInflow + suicideInflow - outflow - intOutflow - suicideOutflow - weiGasCost;

    // Check to see if there are any mismatches...
    begBalDiff = begBal - lastStatement.endBal;
    endBalDiff = endBal - endBalCalc;

    // ...if not, we're reconciled, so we can return...
    reconciled = (endBalDiff == 0 && begBalDiff == 0);
    if (reconciled)
        return true;

    // ...otherwise, we try to recover
    // Case 4: We need to dig into the traces (Note: this is the first place where we dig into the traces...
    // doing so without having been forced to causes a huge performance penalty.)
    if (reconcileUsingTraces(lastStatement, accountingFor, nextBlock, trans))
        return true;

    // Case 2: The blockchain only returns balances PER block. This means that if two value changing transactions
    // occur in the same block the first will have an incorrect ending balance and the second will have an
    // incorrect beginning balance. It's a bit more complicated than that, because there could be transactions
    // in the same block both before and after the current transaction...we handle all cases here.

    /*
        Possibilities:

        prev blk    cur blk     next blk
           9          10           12        both different (handled above)
          10          12           12        previous is different, next is same block
          12          12           12        both prev and next are same block
          12          12           13        previous is the same block, next is different
          12          13           15        both next and previous are different (handled above)
     */

    bool prevDifferent = lastStatement.blockNum != blockNum;
    bool nextDifferent = blockNum != nextBlock;

    if (prevDifferent && nextDifferent) {
        // handled above...

    } else if (prevDifferent) {
        // The next transaction on this address is from the same block, but the previous transaction is from a
        // different block. We can't assume that the next transaction doesn't transfer value, so we have to handle
        // that case.

        // Ending balance at the previous block should be the same as beginning balance at this block...
        begBal = getBalanceAt(accountingFor, blockNum == 0 ? 0 : blockNum - 1);
        begBalDiff = begBal - lastStatement.endBal;

        // We use the same "in-transaction" data to arrive at...
        endBalCalc = begBal + inflow + intInflow + suicideInflow - outflow - intOutflow - suicideOutflow - weiGasCost;

        // ...a calculated ending balance. Important note; the "true" ending balance for this transaction is not
        // available until the end of the block. The best we can do is temporarily assume the calculated balance
        // is correct and make a note of the fact that we've done that.
        endBal = endBalCalc;
        endBalDiff = endBal - endBalCalc;
        reconciliationType = "prevdiff-partial";

    } else if (nextDifferent) {
        // In this case, the previous transaction on this account is in the same block. We cannot use the
        // beginning of block balance because that previous transaction may have altered the account's balance
        // after the start of the block. We have to use the previously calculated ending balance as the
        // beginning balance for this transaction. Note: diff will be zero in every case.
        begBal = lastStatement.endBalCalc;
        begBalDiff = begBal - lastStatement.endBalCalc;

        // Again, we use the same "in-transaction" data to arrive at...
        endBalCalc = begBal + inflow + intInflow + suicideInflow - outflow - intOutflow - suicideOutflow - weiGasCost;

        // the true ending balance (since we know that the next transaction on this account is in a different
        // block, we can use the balance from the node, and it should reconcile.
        endBal = getBalanceAt(accountingFor, blockNum);
        endBalDiff = endBal - endBalCalc;

        reconciliationType = "nextdiff-partial";

    } else {
        // In this case, both the previous transaction and the next transactions are in the same block. Neither
        // the beginning balance at the block nor the ending balance can be used (becuase either the previous
        // transaction or the next transaction or both could have changed the balance). Our only recourse is to
        // make use of the calculated balances and make a note of the fact that we've done this...We have to use
        // calculated values
        begBal = lastStatement.endBalCalc;
        begBalDiff = begBal - lastStatement.endBalCalc;

        endBalCalc = begBal + inflow + intInflow + suicideInflow - outflow - intOutflow - suicideOutflow - weiGasCost;

        // ... the next transaction is from the same block, we have to use the calculated balance
        endBal = endBalCalc;
        endBalDiff = endBal - endBalCalc;
        reconciliationType = "both-partial";
    }

    // If we're reconciled, we're done...
    reconciled = (endBalDiff == 0 && begBalDiff == 0);
    return reconciled;
}

extern bool loadTraces(CTransaction& trans, blknum_t bn, blknum_t txid, bool useCache, bool skipDdos);
//---------------------------------------------------------------------------
bool CReconciliationNumeric::reconcileUsingTraces(const CReconciliationNumeric& lastStatement,
                                                  const address_t& accountingFor, blknum_t nextBlock,
                                                  const CTransaction* trans) {
    outflow = inflow = 0;  // we will store it in the internal values
    loadTraces(*((CTransaction*)trans), trans->blockNumber, trans->transactionIndex, false, false);
    for (auto trace : trans->traces) {
        bool isSuicide = trace.action.selfDestructed != "";
        if (isSuicide) {
            if (trace.action.refundAddress == accountingFor) {
                // receives suicided ether
                suicideInflow += trace.action.balance;
            }

            // do not collapse. It may be both
            if (trace.action.selfDestructed == accountingFor) {
                // the smart contract that is being killed and thereby loses the eth
                suicideOutflow += trace.action.balance;
            }

        } else {
            if (trace.action.from == accountingFor) {
                intOutflow += trans->isError ? 0 : trace.action.value;
                // weiGasCost = str_2_BigInt(trans->getValueByName("gasCost"));
            }

            // do not collapse. It may be both
            if (trace.action.to == accountingFor) {
                intInflow += trans->isError ? 0 : trace.action.value;
            }
        }
    }

    endBalCalc = begBal + inflow + intInflow + suicideInflow - outflow - intOutflow - suicideOutflow - weiGasCost;
    endBalDiff = endBal - endBalCalc;
    begBalDiff = begBal - lastStatement.endBal;
    reconciled = (endBalDiff == 0 && begBalDiff == 0);
    return reconciled;
}
// EXISTING_CODE
}  // namespace qblocks
