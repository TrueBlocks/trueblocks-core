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
#include "pricequote.h"

namespace qblocks {

    typedef char* (*PRICEPARSEFUNC)(CPriceQuote& quote, char *p);
    extern char *parsePoloniex(CPriceQuote& quote, char *p);
    extern const char* STR_PRICE_URL;

    class CPriceSource {
    public:
        SFString url;
        SFString pair;
        PRICEPARSEFUNC func;
        CPriceSource(const SFString& u=STR_PRICE_URL,
                     const SFString& p="USDT_ETH",
                     PRICEPARSEFUNC f=parsePoloniex)
            : url(u), pair(p), func(f) {}
        SFString getDatabasePath(void) const;
    };

    extern bool loadPriceData(const CPriceSource& source, CPriceQuoteArray& quotes, bool freshen, SFString& message, uint64_t step = 1);

}  // namespace qblocks
