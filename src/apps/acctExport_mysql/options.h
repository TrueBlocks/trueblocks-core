#pragma once
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
#include "acctlib.h"
#include "tokenlib.h"
#ifndef NOWALLETLIB
#include "walletlib.h"
#endif
#include "options.h"
#include "sql_database.h"

//-----------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    bool               sql_on;
    bool               json;
    uint64_t           maxRows;
    CAccountWatchArray watches;
    string_q           transFmt;
    string_q           transExport;

public:
             COptions        (void);
            ~COptions        (void);

    bool     parseArguments  (string_q& command);
    void     Init            (void);
    bool     loadWatches     (const CToml& toml);
//    bool     loadBalances    (void);

    bool     connectDatabase (void);
    bool     displayFromCache(void);
};

//-----------------------------------------------------------------------
extern string_q promoteToFunc(const CTransaction *p);
extern bool     processMigrate(void);
extern bool     processConfigs(const COptions& options);
extern bool     processPrices(void);
extern void     comment(const char *msg);
