#pragma once
/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
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

class COptions : public COptionsBase {
public:
    bool prettyPrint;
    bool rerun;
    bool incomeOnly;
    bool expenseOnly;
    COptionsBlockList blocks;
    time_q firstDate;
    string_q funcFilter;
    int errFilt;
    bool reverseSort;
    time_q lastDate;
    bool openFile;
    string_q addr;
    size_t maxTransactions;
    size_t pageSize;
    string_q exportFormat;
    string_q name;
    bool cache;
    size_t acct_id;

    COptions(void);
    ~COptions(void);

    string_q postProcess(const string_q& which, const string_q& str) const override;
    bool parseArguments(string_q& command) override;
    void Init(void) override;
};
