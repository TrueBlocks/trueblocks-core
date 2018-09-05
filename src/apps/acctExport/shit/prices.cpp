/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
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
