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
#include "etherlib.h"

//-----------------------------------------------------------------------------
class COptions : public CBlockOptions {
public:
// BEG_CODE_DECLARE
// END_CODE_DECLARE

    CBlockMap items;
    CNameValueArray requests;

    COptions(void);
    ~COptions(void);

    string_q postProcess(const string_q& which, const string_q& str) const override;
    bool parseArguments(string_q& command) override;
    void Init(void) override;

    void applyFilter(void);
    string_q listSpecials(format_t fmt) const;
};

extern bool lookupDate(const COptions *options, CBlock& block, const timestamp_t& ts);
extern bool showSpecials(CNameValue& pair, void *data);
