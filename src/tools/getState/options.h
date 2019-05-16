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

//-----------------------------------------------------------------------------
typedef enum {
    ST_NONE = 0,
    ST_BALANCE = (1<<1), ST_NONCE = (1<<2), ST_CODE = (1<<3), ST_STORAGE = (1<<4),
    ST_DEPLOYED = (1<<5), ST_ACCTTYPE = (1<<6),
    ST_SOME = (ST_BALANCE|ST_NONCE|ST_CODE|ST_ACCTTYPE),
    ST_ALL = (ST_BALANCE|ST_NONCE|ST_CODE|ST_STORAGE|ST_DEPLOYED|ST_ACCTTYPE)
} ethstate_t;

//-----------------------------------------------------------------------------
class COptions : public CHistoryOptions {
public:
    CEthStateMap items;
    CEthState *item;
    ethstate_t mode;
    biguint_t prevBal;
    bool exclude_zero;
    bool changes;
    wei_t deminimus;

    COptions(void);
    ~COptions(void);

    string_q postProcess(const string_q& which, const string_q& str) const override;
    bool parseArguments(string_q& command) override;
    void Init(void) override;
};

//-----------------------------------------------------------------------------
extern bool visitBlock(uint64_t num, void *data);
