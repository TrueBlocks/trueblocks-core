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
#include "node.h"

namespace qblocks {

//---------------------------------------------------------------------------
void CTrace::loadTraceAsBlockReward(const CTransaction& trans, blknum_t bn, blknum_t txid) {
    ASSERT(txid == 99999 || txid == 99997);
    blockNumber = bn;
    transactionIndex = txid;
    action.from = "0xBlockReward";
    action.to = trans.to;
    action.callType = "block-reward";
    action.value = getBlockReward2(bn);
    action.extraValue1 = getNephewReward(bn);
    action.extraValue2 = (result.gasUsed * trans.gasPrice);
    traceAddress.push_back("null-b-s");
    // transactionHash = uint_2_Hex(bn * 100000 + txid);
    action.input = "0x";
    pTransaction = &trans;
}

//---------------------------------------------------------------------------
void CTrace::loadTraceAsUncleReward(const CTransaction& trans, blknum_t bn, blknum_t uncleBn) {
    ASSERT(txid == 99998);
    blockNumber = bn;
    transactionIndex = 99998;
    action.from = "0xUncleReward";
    action.to = trans.to;
    action.callType = "uncle-reward";
    action.value +=
        getUncleReward(bn, uncleBn);  // we use += here because you can win more than one uncle block per block
    action.input = "0x";
    pTransaction = &trans;
}

//---------------------------------------------------------------------------
void CTrace::loadTraceAsTransFee(const CTransaction& trans, blknum_t bn, blknum_t txid) {
    blockNumber = bn;
    transactionIndex = txid;
    action.from = "0xTransactionFee";
    action.to = trans.to;
    action.callType = "tx-fee";
    // action.value = getTransactionFees(bn);
    traceAddress.push_back("null-f-s");
    // transactionHash = uint_2_Hex(bn * 100000 + txid);
    action.input = "0x";
    pTransaction = &trans;
}

//---------------------------------------------------------------------------
void CTrace::loadTraceAsDdos(const CTransaction& trans, blknum_t bn, blknum_t txid) {
    blockNumber = bn;
    transactionIndex = txid;
    action.from = "0xdd05dd05dd05dd05dd05dd05dd05dd05";
    action.to = "0xdd05dd05dd05dd05dd05dd05dd05dd05";
    action.callType = "ddos";
    action.value = trans.value;
    traceAddress.push_back("s");
    transactionHash = uint_2_Hex(bn * 100000 + txid);
    action.input = "0xdd05";
    pTransaction = &trans;
}

//-------------------------------------------------------------------------
bool CTransaction::loadTransAsPrefund(blknum_t bn, blknum_t txid, const address_t& addr, const wei_t& amount) {
    initialize();
    blockNumber = bn;
    transactionIndex = txid;
    from = "0xPrefund";
    to = addr;
    value = amount;
    receipt = CReceipt();
    receipt.pTransaction = this;
    return true;
}

//-------------------------------------------------------------------------
bool CTransaction::loadTransAsBlockReward(blknum_t bn, blknum_t txid, const address_t& addr) {
    ASSERT(txid == 99999 || txid == 99997);
    initialize();
    blockNumber = bn;
    transactionIndex = txid;
    from = "0xBlockReward";
    to = addr;
    value = getBlockReward2(bn);
    extraValue1 = getNephewReward(bn);
    extraValue2 = getTransFees(bn);  // weird temp value for reconciliation only
    receipt = CReceipt();
    receipt.pTransaction = this;
    return true;
}

//-------------------------------------------------------------------------
bool CTransaction::loadTransAsUncleReward(blknum_t bn, blknum_t uncleBn, const address_t& addr) {
    ASSERT(txid == 99998);
    initialize();
    blockNumber = bn;
    transactionIndex = 99998;
    from = "0xUncleReward";
    to = addr;
    value += getUncleReward(bn, uncleBn);  // we use += here because you can win more than one uncle block per block
    receipt = CReceipt();
    receipt.pTransaction = this;
    return true;
}

//---------------------------------------------------------------------------
wei_t getBlockReward2(blknum_t bn) {
    if (bn == 0)
        return 0;

    wei_t reward = 0;
    if (bn < byzantiumBlock) {
        reward = str_2_Wei("5000000000000000000");
    } else if (bn < constantinopleBlock) {
        reward = str_2_Wei("3000000000000000000");
    } else {
        reward = str_2_Wei("2000000000000000000");
    }
    return reward;
}

//---------------------------------------------------------------------------
wei_t getNephewReward(blknum_t bn) {
    if (bn == 0)
        return 0;

    wei_t reward = 0;
    blknum_t nUncles = getUncleCount(bn);
    if (nUncles)
        reward += ((getBlockReward2(bn) * nUncles) / 32);
    return reward;
}

//---------------------------------------------------------------------------
wei_t getUncleReward(blknum_t bn, blknum_t uncleBn) {
    if (bn == 0)
        return 0;

    wei_t reward = getBlockReward2(bn);
    if ((uncleBn + 6) < bn)
        return 0;
    blknum_t diff = (uncleBn + 8 - bn);
    return (reward / 8 * diff);
}

//---------------------------------------------------------------------------
extern wei_t getTransFees(blknum_t bn) {
    if (bn == 0)
        return 0;

    wei_t fees = 0;
    CBlock block;
    getBlock(block, bn);
    for (auto tx : block.transactions)
        fees += (tx.receipt.gasUsed * tx.gasPrice);
    return fees;
}

}  // namespace qblocks
