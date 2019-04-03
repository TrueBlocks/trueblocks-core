/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//-----------------------------------------------------------------------
int main(int argc, const char *argv[]) {
    nodeNotRequired();
    acctlib_init(defaultQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        options.showBanner();
        if (!options.useBlooms)
            options.scrapeCnt = 10000000;  // TODO(tjayrush): Not right

        if (options.useBlooms)
            forEveryBloomFile(visitBloomFilters, &options, options.startScrape, options.scrapeCnt);
        else
            forEveryFileInFolder(indexFolder_sorted_v2, visitIndexFiles, &options);

    }
    options.finalReport();
    acctlib_cleanup();

    return 0;
}

//-----------------------------------------------------------------------
void COptions::showBanner(void) const {
    if (isTestMode())
        return;
    cerr << bBlack << Now().Format(FMT_JSON) << cOff;
    cerr << ": Monitoring " << cYellow << getTransCachePath(primary.address) << cOff;
    cerr << " (start: " << cTeal << startScrape << cOff;
    cerr << " end: " + cTeal << (startScrape + scrapeCnt) << cOff;
    cerr << (!useBlooms ? "" : (" n: " + cTeal + uint_2_Str(scrapeCnt))) << cOff << ")           \n";
}
