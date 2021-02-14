/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    nodeNotRequired();
    namesNotRequired();
    pinlib_init(defaultQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            continue;

        // Clean the monitor stage of previously unfinished scrapes
        cleanMonitorStage();

        if (options.visitTypes & VIS_FINAL)
            forEveryFileInFolder(indexFolder_blooms, visitFinalIndexFiles, &options);

        if (options.visitTypes & VIS_STAGING)
            forEveryFileInFolder(indexFolder_staging, visitStagingIndexFiles, &options);

        if (options.visitTypes & VIS_UNRIPE)
            forEveryFileInFolder(indexFolder_unripe, visitUnripeIndexFiles, &options);

        for (auto monitor : options.allMonitors) {
            monitor.moveToProduction();
            LOG4(monitor.address, " freshened to ", monitor.getLastVisited(true /* fresh */));
        }
    }

    if (options.stats.nFiles != options.stats.nSkipped) {
        ostringstream header;
        header << options.stats.Format(substitute(substitute(STR_DISPLAY_ACCTSCRAPESTATS, "\t", ","), "{", "{p:"));

        ostringstream data;
        data << options.stats.Format(substitute(STR_DISPLAY_ACCTSCRAPESTATS, "\t", ","));

        LOG4(header.str());
        LOG4(data.str());

        header << endl;
        data << endl;

        string_q statsFile = configPath("performance_scraper.csv");
        if (!fileExists(statsFile))
            stringToAsciiFile(statsFile, header.str());
        appendToAsciiFile(statsFile, data.str());
    }

    pinlib_cleanup();
    return 0;
}
