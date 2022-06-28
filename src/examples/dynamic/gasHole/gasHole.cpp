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
#include "gasHole.h"

//-----------------------------------------------------------------------
class CTestTraverser : public CTraverser {
  public:
    // wei_t totalSpent;
    // wei_t totalError;
    CTestTraverser(void) : CTraverser("testing") {
        // totalSpent = 0;
        // totalError = 0;
    }
};

//-----------------------------------------------------------------------
bool header(CTraverser* trav, void* data) {
    CFunction::registerClass();
    CParameter::registerClass();
    loadNames();
    cout << tab_2_Csv(str_2_Header(substitute(STR_OUTPUT_FORMAT, "date", "tx_date"))) << endl;
    return true;
}

//-----------------------------------------------------------------------
bool display(CTraverser* trav, void* data) {
    double spotPrice = getPriceInUsd(trav->trans.blockNumber);
    double gasCostEther = str_2_Double(trav->trans.Format("[{ETHERGASCOST}]"));
    double gasCostUsd = (gasCostEther * spotPrice);

    CAccountName acct;
    findName(trav->trans.Format("[{FROM}]"), acct);
    if (acct.name.empty()) {
        acct.name = "anon";
    }

    string_q fmt = tab_2_Csv(STR_OUTPUT_FORMAT);
    fmt = substitute(fmt, "[{spotPrice}]", double_2_Str(spotPrice, 2));
    fmt = substitute(fmt, "[{usdGasCost}]", double_2_Str(gasCostUsd, 2));
    fmt = substitute(fmt, "[{name}]", acct.name);
    if (isZeroAddr(trav->trans.to)) {
        fmt = substitute(fmt, "[{compressedTx}]", "Contract Deployment");
    }
    cout << substitute(trav->trans.Format(fmt), " UTC", "") << endl;
    return true;
}

//-----------------------------------------------------------------------
extern "C" CTraverser* makeTraverser(uint32_t wanted) {
    if (getVersionNum() < wanted) {
        LOG_ERR("Cannot load traverser from older versions: ", getVersionNum());
        LOG_ERR("You need to re-install TrueBlocks and rebuild this traverser.");
        return nullptr;
    }
    CTestTraverser* trav = new CTestTraverser;
    trav->preFunc = header;
    trav->displayFunc = display;
    return trav;
}

//-----------------------------------------------------------------------
const char* STR_OUTPUT_FORMAT =
    "[{blockNumber}]\t"
    "[{transactionIndex}]\t"
    "[{timestamp}]\t"
    "[{date}]\t"
    "[{from}]\t"
    "[{gasUsed}]\t"
    "[{gasPrice}]\t"
    "[{gasCost}]\t"
    "[{etherGasCost}]\t"
    "[{spotPrice}]\t"
    "[{usdGasCost}]\t"
    "[{hash}]\t"
    "[{name}]\t"
    "[{function}]\t"
    "[{compressedTx}]";

//-----------------------------------------------------------------------
string_q str_2_Header(const string_q& strIn) {
    return substitute(substitute(strIn, "[{", ""), "}]", "");
}

//-----------------------------------------------------------------------
string_q tab_2_Csv(const string_q& strIn) {
    return "\"" + substitute(strIn, "\t", "\",\"") + "\"";
}
