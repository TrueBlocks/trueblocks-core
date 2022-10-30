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
bigint_t CReconciliation::totalIn_internal(void) const {
    return amountIn + internalIn + selfDestructIn + prefundIn + minerBaseRewardIn + minerNephewRewardIn + minerTxFeeIn +
           minerUncleRewardIn;
}

//-----------------------------------------------------------------------
bigint_t CReconciliation::totalOut_internal(void) const {
    return amountOut + internalOut + selfDestructOut + gasCostOut;
}

//-----------------------------------------------------------------------
bigint_t CReconciliation::totalOutLessGas_internal(void) const {
    return amountOut + internalOut + selfDestructOut;
}

//---------------------------------------------------------------------------
bigint_t CReconciliation::begBalDiff_internal(void) const {
    return blockNumber == 0 ? 0 : begBal - prevBal;
}

//---------------------------------------------------------------------------
bigint_t CReconciliation::endBalCalc_internal(void) const {
    return begBal + amountNet_internal();
}

//---------------------------------------------------------------------------
bigint_t CReconciliation::endBalDiff_internal(void) const {
    return endBalCalc_internal() - endBal;
}

//---------------------------------------------------------------------------
bool CReconciliation::reconciled_internal(void) const {
    return (endBalDiff_internal() == 0 && begBalDiff_internal() == 0);
}

//---------------------------------------------------------------------------
bigint_t CReconciliation::amountNet_internal(void) const {
    return totalIn_internal() - totalOut_internal();
}

}  // namespace qblocks
