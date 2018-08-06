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
#include <algorithm>
#include "etherlib.h"
#include "webapi.h"
#include "account.h"
#include "options.h"

//---------------------------------------------------------------------------------------------------
class CSlurperApp {
private:
    CWebAPI api;

public:
    CAccount theAccount;

    CSlurperApp(void) { }
    ~CSlurperApp(void) { }

    bool Initialize(COptions& options, string_q& message);
    bool Slurp(COptions& options, string_q& message);
    bool Filter(COptions& options, string_q& message);
    bool Display(COptions& options, string_q& message);

private:
    void buildDisplayStrings(COptions& options);
    string_q getFormatString(COptions& options, const string_q& name, bool ignoreBlank);
};

//---------------------------------------------------------------------------------------------------
extern void findBlockRange(const string_q& contents, size_t& minBlock, size_t& maxBlock);
extern void clearAbis(void);
extern bool loadABI(CAbi& abi, const string_q& addr);
