/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "options.h"
#include "sql_database.h"

//---------------------------------------------------------------------------------------------------
bool processPrices(void) {

    CPriceQuote::registerClass();

    CMySqlServer sqlServer;
    string_q host = getGlobalConfig()->getConfigStr("mysql", "host", "");
    string_q user = getGlobalConfig()->getConfigStr("mysql", "user", "");
    string_q pw   = getGlobalConfig()->getConfigStr("mysql", "pw",   "");
    sqlServer.setConnection(host, user, pw, true);
    sqlServer.useDatabase("tokenomics_local");

    CPriceQuoteArray quotes;
    string_q message;
    CPriceSource source;
    if (!loadPriceData(source, quotes, false, message, 1)) {
        cerr << "Cannot load price data. Quitting.\n";
        exit(0);
    }

    for (uint32_t i = 0 ; i < quotes.size() ; i++) { // five min increments
        sqlServer.insertRows("eth_price_usd", "timeStamp, exchangeRate", quotes[i].Format("([{TIMESTAMP},][{CLOSE}])"), "timeStamp");
        cout << i << "\t" << quotes[i].timestamp << "\r";
        cout.flush();
    }

    return true;
}
