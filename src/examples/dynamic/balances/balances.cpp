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
#include "acctlib.h"

//-----------------------------------------------------------------------
class CTestTraverser : public CTraverser {
  public:
    wei_t totalSpent;
    wei_t totalError;
    CTestTraverser(void) : CTraverser("testing") {
        totalSpent = 0;
        totalError = 0;
    }
};

//-----------------------------------------------------------------------
bool header(CTraverser* trav, void* data) {
    cout << "date,";
    cout << "balanceEth,";
    cout << "balanceUSD,";
    cout << "priceUSD" << endl;
    return true;
}

const wei_t oneEther = str_2_Wei("1000000000000000000");
//-----------------------------------------------------------------------
bool display(CTraverser* trav, void* data) {
    CTestTraverser* tt = (CTestTraverser*)trav;

    wei_t balance = getBalanceAt(tt->curMonitor->address, tt->app->blk);
    cout << trav->trans.Format("[{DATE}]") << ",";
    cout << wei_2_Ether(balance, 18) << ",";
    cout << wei_2_Dollars(trav->trans.timestamp, balance, 18) << ",";
    cout << wei_2_Dollars(trav->trans.timestamp, oneEther, 18) << endl;

    return true;
}

//-----------------------------------------------------------------------
extern "C" CTraverser* makeTraverser(void) {
    if (getVersionNum() < getVersionNum(0, 18, 0)) {
        LOG_ERR("Cannot load traverser from older versions: ", getVersionNum());
        LOG_ERR("Perhaps you need to re-install TrueBlocks.");
        return nullptr;
    }
    CTestTraverser* trav = new CTestTraverser;
    trav->preFunc = header;
    trav->displayFunc = display;
    return trav;
}
