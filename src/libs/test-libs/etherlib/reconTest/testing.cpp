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
#include <string>
#include <algorithm>
#include "etherlib.h"
#include "testing.h"

//------------------------------------------------------------------------
class CThisTest : public testing::Test {
  public:
    CThisTest(void) : Test() {
    }
    virtual void SetUp(void) {
    }
    virtual void TearDown(void) {
    }
};

//------------------------------------------------------------------------
TEST_F(CThisTest, TestTest_1) {
    cout << "Running " << testName << "\n";

    ASSERT_EQ("test", true, true);

    verbose = 5;
    CTransaction trans;
    getTransaction(trans, 8856290, 62);
    getReceipt(trans.receipt, trans.hash);
    cout << trans << endl;

    CReconciliation prev;
    prev.endBal = str_2_BigInt("5000000000000000000");
    prev.blockNumber = 8856285;

    CReconciliation rec;
    cout << rec << endl;
    rec = CReconciliation(trans.blockNumber, trans.transactionIndex, trans.timestamp);
    CAccountName acct;
    acct.address = "0xf503017d7baf7fbc0fff7492b751025c6a78179b";
    rec.reconcileEth(prev, 8856295, &trans, acct);
    cout << rec << endl;

    rec.blockNumber = 1;
    rec.transactionIndex = 1;
    rec.timestamp = 1;
    rec.assetAddr = "address";
    rec.assetSymbol = "symbol";
    rec.decimals = 1;
    rec.prevBlk = 1;
    rec.prevBlkBal = 1;
    rec.begBal = 1;
    rec.amountIn = 1;
    rec.amountOut = 1;
    rec.internalIn = 1;
    rec.internalOut = 1;
    rec.selfDestructIn = 1;
    rec.selfDestructOut = 1;
    rec.minerBaseRewardIn = 1;
    rec.minerNephewRewardIn = 1;
    rec.minerTxFeeIn = 1;
    rec.minerUncleRewardIn = 1;
    rec.prefundIn = 1;
    rec.gasCostOut = 1;
    rec.endBal = 5;
    rec.spotPrice = 1;
    rec.reconciliationType = "regular";
    cout << rec << endl;

    rec.blockNumber = 100;
    rec.transactionIndex = 100;
    rec.timestamp = 100;
    rec.assetAddr = "new";
    rec.assetSymbol = "what";
    rec.decimals = 100;
    rec.prevBlk = 100;
    rec.prevBlkBal = 100;
    rec.spotPrice = 100;
    rec += rec;
    cout << rec << endl;

    return true;
}
}

//------------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    setenv("API_MODE", "true", true);
    etherlib_init(quickQuitHandler);
    verbose = 5;
    LOAD_TEST(TestTest_1);
    return testing::RUN_ALL_TESTS();
}
