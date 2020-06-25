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
        case 'g':
            if (fieldName % "gasCostOutflow") {
                return bni_2_Str(gasCostOutflow);
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
        case 'm':
            if (fieldName % "miningInflow") {
                return bni_2_Str(miningInflow);
            }
            break;
        case 'o':
            if (fieldName % "outflow") {
                return bni_2_Str(outflow);
            }
            break;
        case 'p':
            if (fieldName % "prefundInflow") {
                return bni_2_Str(prefundInflow);
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
            if (fieldName % "selfDestructInflow") {
                return bni_2_Str(selfDestructInflow);
            }
            if (fieldName % "selfDestructOutflow") {
                return bni_2_Str(selfDestructOutflow);
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
        case 'g':
            if (fieldName % "gasCostOutflow") {
                gasCostOutflow = str_2_Wei(fieldValue);
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
        case 'm':
            if (fieldName % "miningInflow") {
                miningInflow = str_2_Wei(fieldValue);
                return true;
            }
            break;
        case 'o':
            if (fieldName % "outflow") {
                outflow = str_2_Wei(fieldValue);
                return true;
            }
            break;
        case 'p':
            if (fieldName % "prefundInflow") {
                prefundInflow = str_2_Wei(fieldValue);
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
            if (fieldName % "selfDestructInflow") {
                selfDestructInflow = str_2_Wei(fieldValue);
                return true;
            }
            if (fieldName % "selfDestructOutflow") {
                selfDestructOutflow = str_2_Wei(fieldValue);
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
    archive >> blockNum;
    archive >> asset;
    archive >> begBal;
    archive >> begBalDiff;
    archive >> inflow;
    archive >> intInflow;
    archive >> selfDestructInflow;
    archive >> miningInflow;
    archive >> prefundInflow;
    archive >> outflow;
    archive >> intOutflow;
    archive >> selfDestructOutflow;
    archive >> gasCostOutflow;
    archive >> endBal;
    archive >> endBalCalc;
    archive >> endBalDiff;
    archive >> reconciliationType;
    archive >> reconciled;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CReconciliation::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << blockNum;
    archive << asset;
    archive << begBal;
    archive << begBalDiff;
    archive << inflow;
    archive << intInflow;
    archive << selfDestructInflow;
    archive << miningInflow;
    archive << prefundInflow;
    archive << outflow;
    archive << intOutflow;
    archive << selfDestructOutflow;
    archive << gasCostOutflow;
    archive << endBal;
    archive << endBalCalc;
    archive << endBalDiff;
    archive << reconciliationType;
    archive << reconciled;

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
    ADD_FIELD(CReconciliation, "blockNum", T_BLOCKNUM, ++fieldNum);
    ADD_FIELD(CReconciliation, "asset", T_TEXT, ++fieldNum);
    ADD_FIELD(CReconciliation, "begBal", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliation, "begBalDiff", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliation, "inflow", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliation, "intInflow", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliation, "selfDestructInflow", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliation, "miningInflow", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliation, "prefundInflow", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliation, "outflow", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliation, "intOutflow", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliation, "selfDestructOutflow", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliation, "gasCostOutflow", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliation, "endBal", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliation, "endBalCalc", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliation, "endBalDiff", T_INT256, ++fieldNum);
    ADD_FIELD(CReconciliation, "reconciliationType", T_TEXT, ++fieldNum);
    ADD_FIELD(CReconciliation, "reconciled", T_BOOL, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CReconciliation, "schema");
    HIDE_FIELD(CReconciliation, "deleted");
    HIDE_FIELD(CReconciliation, "showing");
    HIDE_FIELD(CReconciliation, "cname");

    builtIns.push_back(_biCReconciliation);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextReconciliationChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CReconciliation* rec = reinterpret_cast<const CReconciliation*>(dataPtr);
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
bool CReconciliation::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
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
ostream& operator<<(ostream& os, const CReconciliation& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_RECONCILIATION = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
//---------------------------------------------------------------------------
bool CReconciliation::reconcile(const CStringArray& corrections, const CReconciliation& lastStatement,
                                const address_t& accountingFor, blknum_t nextBlock, const CTransaction* trans) {
    asset = "ETH";

    // Note: In the case of an error, we need to account for gas usage if the account is the transaction's sender
    //
    // Note: There are many complications with reconciliation. To start, we do a top level reconciliations
    // and if it works, we return...

    // Case 1: We try to reconcile from top line (i.e. transaction level) data...
    reconciliationType = "";

    // We need to account for both the case where the account is the sender...
    if (trans->from == accountingFor) {
        outflow = trans->isError ? 0 : trans->value;
        gasCostOutflow = str_2_BigInt(trans->getValueByName("gasCost"));
    }

    // ... and/or the receiver...
    if (trans->to == accountingFor) {
        if (trans->from == "0xPrefund") {
            prefundInflow = trans->value;
        } else if (trans->from == "0xBlockReward" || trans->from == "0xUncleReward") {
            miningInflow = trans->value;
        } else {
            inflow = trans->isError ? 0 : trans->value;
        }
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
    endBalCalc = begBal + inflow + intInflow + selfDestructInflow + miningInflow + prefundInflow - outflow -
                 intOutflow - selfDestructOutflow - gasCostOutflow;

    // Check to see if there are any mismatches...
    begBalDiff = trans->blockNumber == 0 ? 0 : begBal - lastStatement.endBal;
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
        begBalDiff = trans->blockNumber == 0 ? 0 : begBal - lastStatement.endBal;

        // We use the same "in-transaction" data to arrive at...
        endBalCalc = begBal + inflow + intInflow + selfDestructInflow + miningInflow + prefundInflow - outflow -
                     intOutflow - selfDestructOutflow - gasCostOutflow;

        // ...a calculated ending balance. Important note; the "true" ending balance for this transaction is not
        // available until the end of the block. The best we can do is temporarily assume the calculated balance
        // is correct and make a note of the fact that we've done that.
        endBal = endBalCalc;
        endBalDiff = endBal - endBalCalc;
        reconciliationType = trans->blockNumber == 0 ? "" : "prevdiff-partial";

    } else if (nextDifferent) {
        // In this case, the previous transaction on this account is in the same block. We cannot use the
        // beginning of block balance because that previous transaction may have altered the account's balance
        // after the start of the block. We have to use the previously calculated ending balance as the
        // beginning balance for this transaction. Note: diff will be zero in every case.
        begBal = lastStatement.endBalCalc;
        ASSERT(trans->blockNumber != 0);
        begBalDiff = begBal - lastStatement.endBalCalc;

        // Again, we use the same "in-transaction" data to arrive at...
        endBalCalc = begBal + inflow + intInflow + selfDestructInflow + miningInflow + prefundInflow - outflow -
                     intOutflow - selfDestructOutflow - gasCostOutflow;

        // the true ending balance (since we know that the next transaction on this account is in a different
        // block, we can use the balance from the node, and it should reconcile.
        endBal = getBalanceAt(accountingFor, blockNum);
        endBalDiff = endBal - endBalCalc;
        reconciliationType = trans->blockNumber == 0 ? "" : "nextdiff-partial";

    } else {
        // In this case, both the previous transaction and the next transactions are in the same block. Neither
        // the beginning balance at the block nor the ending balance can be used (becuase either the previous
        // transaction or the next transaction or both could have changed the balance). Our only recourse is to
        // make use of the calculated balances and make a note of the fact that we've done this...We have to use
        // calculated values
        begBal = lastStatement.endBalCalc;
        ASSERT(trans->blockNumber != 0);
        begBalDiff = begBal - lastStatement.endBalCalc;

        endBalCalc = begBal + inflow + intInflow + selfDestructInflow + miningInflow + prefundInflow - outflow -
                     intOutflow - selfDestructOutflow - gasCostOutflow;

        // ... the next transaction is from the same block, we have to use the calculated balance
        endBal = endBalCalc;
        endBalDiff = endBal - endBalCalc;
        reconciliationType = trans->blockNumber == 0 ? "" : "both-partial";
    }

    // If we're reconciled, we're done...
    reconciled = (endBalDiff == 0 && begBalDiff == 0);
    return reconciled;
}

extern bool loadTraces(CTransaction& trans, blknum_t bn, blknum_t txid, bool useCache, bool skipDdos);
//---------------------------------------------------------------------------
bool CReconciliation::reconcileUsingTraces(const CReconciliation& lastStatement, const address_t& accountingFor,
                                           blknum_t nextBlock, const CTransaction* trans) {
    outflow = inflow = 0;  // we will store it in the internal values
    loadTraces(*((CTransaction*)trans), trans->blockNumber, trans->transactionIndex, false, false);
    for (auto trace : trans->traces) {
        bool isSelfDestruct = trace.action.selfDestructed != "";
        if (isSelfDestruct) {
            if (trace.action.refundAddress == accountingFor) {
                // receives self destructed ether
                selfDestructInflow += trace.action.balance;
            }

            // do not collapse. It may be both
            if (trace.action.selfDestructed == accountingFor) {
                // the smart contract that is being killed and thereby loses the eth
                selfDestructOutflow += trace.action.balance;
            }

        } else {
            if (trace.action.from == accountingFor) {
                intOutflow += trans->isError ? 0 : trace.action.value;
                // gasCostOutflow = str_2_BigInt(trans->getValueByName("gasCost"));
            }

            // do not collapse. It may be both
            if (trace.action.to == accountingFor) {
                intInflow += trans->isError ? 0 : trace.action.value;
            }
        }
    }

    if (trans->blockNumber == 0) {
        begBal = 0;
        prefundInflow = trans->value;
    }

    endBalCalc = begBal + inflow + intInflow + selfDestructInflow + miningInflow + prefundInflow - outflow -
                 intOutflow - selfDestructOutflow - gasCostOutflow;
    endBalDiff = endBal - endBalCalc;
    begBalDiff = trans->blockNumber == 0 ? 0 : begBal - lastStatement.endBal;
    reconciled = (endBalDiff == 0 && begBalDiff == 0);

    // As crazy as this seems, we clear out the traces here.
    // TODO(tjayrush): add an option to the function to allow preservation of the traces.
    if (!reconciled) {
        // remove the traces if we don't need them to balance
        ((CTransaction*)trans)->traces.clear();
    }
    return reconciled;
}
// EXISTING_CODE
}  // namespace qblocks
