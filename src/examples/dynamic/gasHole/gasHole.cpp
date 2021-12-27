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
    cout << "blockNum"
         << "\t";
    cout << "transactionId"
         << "\t";
    cout << "date"
         << "\t";
    cout << "gasCostEther"
         << "\t";
    cout << "totalSpent"
         << "\t";
    cout << "totalErr"
         << "\t";
    cout << "percentage" << endl;
    return true;
}

//-----------------------------------------------------------------------
bool display(CTraverser* trav, void* data) {
    CTestTraverser* tt = (CTestTraverser*)trav;

    cerr << string_q(120, ' ') << "\r";
    if (tt->curMonitor->address == tt->trans.from) {
        wei_t spent = tt->trans.gasPrice * tt->trans.receipt.gasUsed;
        tt->totalSpent += spent;
        tt->totalError += tt->trans.isError ? spent : 0;

        string_q val = wei_2_Ether(spent, 18);
        string_q tot = wei_2_Ether(tt->totalSpent, 18);
        string_q err = wei_2_Ether(tt->totalError, 18);
        double pct = str_2_Double(err) / str_2_Double(tot);

        cout << trav->app->blk << "\t";
        cout << trav->app->txid << "\t";
        cout << ts_2_Date(trav->trans.timestamp) << "\t";
        cout << val << "\t";
        cout << tot << "\t";
        cout << err << "\t";
        cout << double_2_Str(pct, 5) << endl;
    } else {
        cerr << "No gas spent in block " << tt->trans.blockNumber << "\r";
        cerr.flush();
    }

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
