/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//-----------------------------------------------------------------------------------
int main(int argc, const char* argv[]) {

    SFString message;

    COptions options;
    if (options.prepareArguments(argc, argv)) {

        if (options.parseArguments(options.commandList)) {

            CPriceQuoteArray quotes;
            if (loadPriceData(quotes, options.freshen, message) && quotes.getCount()) {

                SFString fmt = "";
                if (isTesting || options.dispLevel < 2)
                    fmt = "{ \"date\": \"[{DATE}]\", \"price\": \"[{CLOSE}]\" }";

                cout << "[\n";
                uint32_t step = (uint32_t)options.freq / 5;
                for (uint32_t i = 0 ; i < quotes.getCount() ; i = i + step) {
                    if (i > 0)
                        cout << ",\n";
                    cout << quotes[i].Format(fmt);
                }
                cout << "\n]\n";

            } else {
                return usage(message);

            }
        }
    }

    return 0;
}
