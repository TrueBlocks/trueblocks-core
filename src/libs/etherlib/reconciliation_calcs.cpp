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
#include "reconciliation.h"
#include "etherlib.h"

namespace qblocks {

//-----------------------------------------------------------------------
bigint_t CReconciliation::totalIn(void) const {
    return amountIn + internalIn + selfDestructIn + prefundIn + minerBaseRewardIn + minerNephewRewardIn + minerTxFeeIn +
           minerUncleRewardIn;
}

//-----------------------------------------------------------------------
bigint_t CReconciliation::totalOut(void) const {
    return amountOut + internalOut + selfDestructOut + gasOut;
}

//-----------------------------------------------------------------------
bigint_t CReconciliation::totalOutLessGas(void) const {
    return amountOut + internalOut + selfDestructOut;
}

//---------------------------------------------------------------------------
bigint_t CReconciliation::begBalDiff(void) const {
    return blockNumber == 0 ? 0 : begBal - prevBal;
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

//-----------------------------------------------------------------------
bool CReconciliation::trailBalance(void) const {
    return begBal + totalIn() - totalOut() == endBal;
}

}  // namespace qblocks
