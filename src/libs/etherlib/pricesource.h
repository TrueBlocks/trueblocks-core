#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
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
