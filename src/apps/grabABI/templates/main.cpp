/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include <ncurses.h>
#include "etherlib.h"
#include "parselib.h"
#include "processing.h"

//EXISTING_CODE
//EXISTING_CODE

//-----------------------------------------------------------------------
int main(int argc, const char *argv[]) {

    parselib_init("binary");

    registerQuitHandler(myQuitHandler);
    if (argc < 2)
        verbose = true;

    // Parse command line, allowing for command files
    CVisitor visitor;
    if (!visitor.opts.prepareArguments(argc, argv)) {
        return false;
    }

    // while (!visitor.opts.commandList.empty())
    {
        SFString command = nextTokenClear(visitor.opts.commandList, '\n');
        if (!visitor.opts.parseArguments(command)) {
            return false;
        }

        CToml toml;
        toml.setFilename("./config.toml");
        toml.readFile("./config.toml");

        blknum_t minWatchBlock = UINT32_MAX;
        uint64_t nWatches = toml.getConfigInt("watches", "nWatches", 0);
        for (uint32_t i = 0 ; i < nWatches ; i++) {
            CAccountWatch watch;
            if (!watch.getWatch(toml, i, false)) {
                cout << usageStr("Invalid watch parameters for watch " + asString(i) + ". Quitting...")
                            .Substitute("\n", "\r\n");
                return false;
            }
            visitor.watches[visitor.watches.getCount()] = watch;
            minWatchBlock = min(minWatchBlock, watch.firstBlock);
        }
        if (!visitor.watches.getCount()) {
            cout << usageStr("You must specify at least one address to watch in the config file.\r\n")
                        .Substitute("\n", "\r\n");
            return false;
        }
        visitor.watches[visitor.watches.getCount()] = CAccountWatch(1, "Others", "Ext Accts", 0, bRed);

        const char* defaultFormat = "";
        visitor.screenFmt = toml.getConfigStr("formats", "screen_fmt", defaultFormat)
                                    .Substitute("\\n\\\n", "\\n")
                                    .Substitute("\n", "")
                                    .Substitute("\\n", "\r\n")
                                    .Substitute("\\t", "\t");

        visitor.notify        = toml.getConfigBool("display", "notify", false);
        visitor.accounting_on = toml.getConfigBool("display", "accounting", false) || visitor.opts.accounting_on;
        visitor.logs_on       = toml.getConfigBool("display", "logs", false) || visitor.opts.logs_on;
        visitor.trace_on      = toml.getConfigBool("display", "trace", false) || visitor.opts.trace_on;
        visitor.parse_on      = toml.getConfigBool("display", "parse", false) || visitor.opts.parse_on;
        visitor.bloom_on      = toml.getConfigBool("display", "bloom", false) || visitor.opts.bloom_on;
        visitor.debugger_on   = toml.getConfigBool("display", "debug", false) || visitor.opts.debugger_on;
        visitor.single_on     = toml.getConfigBool("display", "single", false) || visitor.opts.single_on;
        visitor.kBlock        = visitor.opts.kBlock;

        // Showing the cache file (if told to...)
        SFString cacheFileName = "./cache/" + visitor.watches[0].address + ".acct.bin";
        if (fileExists(cacheFileName + ".lck")) {
            cout << usageStr("Already running. Either quit other instance or remove the lock file: '"
                             + cacheFileName + ".lck'. Quitting...")
                    .Substitute("\n", "\r\n");
            cout.flush();cerr.flush();getchar();
            return false;
        }

        // Figure out which block to start on. Note, if an 'override' is not present, use earliest
        // block from the watches. Note that 'displayFromCache' may modify this to lastest visited block
        SFUint32 blockNum = max(minWatchBlock-1, toml.getConfigInt("override", "firstBlock", minWatchBlock-1));
        if (visitor.kBlock) {
            blockNum = visitor.kBlock;
            for (uint32_t i = 0 ; i < visitor.watches.getCount() ; i++) {
                visitor.watches[i].qbis.endBal = getBalance(visitor.watches[i].address, blockNum, false);
            }
        }

        if (visitor.debugger_on) {
            removeFile("./cache/debug");
            initscr();
            raw();
            keypad(stdscr, TRUE);
            noecho();
            refresh();
            atexit(myOnExitHandler);
            cout << "Starting balances:\r\n"; cout.flush();
        }

        // Display the cache (if the user tells us to...)
        if (!visitor.debugger_on && !verbose) verbose = 1;
        visitor.cacheOnly = !visitor.opts.mode.Contains("freshen");
        if (visitor.opts.mode.Contains("showCache")) {
            // TODO(tjayrush): allow for early quiting from debugger--trouble--with this on, and no cache, it immediately quits
            // because displayFromCache returns 'false' for more than one reason
            if (!displayFromCache(cacheFileName, blockNum, &visitor))
                visitor.opts.mode = ""; // do not continue
        }

        // Freshening the cache (if the user tells us to...)
        if (visitor.opts.mode.Contains("freshen")) {

            SFUint32 lastVisit = toLongU(asciiFileToString("./cache/lastBlock.txt"));
            blockNum = max(blockNum, lastVisit) + 1;
            SFUint32 topOfChain = getClientLatestBlk();
            SFUint32 nBlocks = (blockNum >= topOfChain ? 0 : topOfChain - blockNum);

            visitor.nBlocksToVisit= topOfChain - blockNum;
            visitor.startBlock = blockNum;
            visitor.endBlock = topOfChain;
            if (verbose)
                visitor.initReport();

            // The cache may have been opened for reading during displayFromCache, so we
            // close it here, so we can open it back up as append only
            if (visitor.cache.isOpen())
                visitor.cache.Release();
            visitor.cache.m_isReading = false;
            visitor.cache.m_archiveSchema = NO_SCHEMA;
            visitor.cache.m_writeDeleted = true;
            if (visitor.cache.Lock(cacheFileName, "a+", LOCK_WAIT)) {
                forEveryNonEmptyBlockOnDisc(updateCache, &visitor, blockNum, nBlocks);
                visitor.cache.Release();
            }
            timestamp_t tsOut = toTimeStamp(Now());
            SFString endMsg = dateFromTimeStamp(tsOut).Format(FMT_JSON) + " (" + asString(topOfChain) + ")";
            visitor.interumReport(topOfChain, visitor.last_ts, endMsg);
            cout << "\r\n";
        }

        SFTime now = Now();
        cout << getprogname() << ": " << now.Format(FMT_JSON) << ": "
                << "{ "
                << cYellow << visitor.nDisplayed    << cOff << " displayed from cache; "
                << cYellow << visitor.nFreshened    << cOff << " written to cache; "
                << cYellow << visitor.nAccountedFor << cOff << " accounted for"
                << " }\r\n";

        if (visitor.debugger_on && visitor.nProcessed() == 0) {
            cout << "Nothing to do. Hit enter to quit...";
            cout.flush();
            getchar();
        }
    }

    return false;
}

//-----------------------------------------------------------------------
void myQuitHandler(int s) {
    if (!isendwin())
        endwin();
    if (s != 1)
        cout << "Caught signal " << s << "\n";
    SFString list = manageRemoveList();
    while (!list.empty()) {
        SFString file = nextTokenClear(list, '|');
        cout << "Removing file: " << file << "\n"; cout.flush();
        removeFile(file);
    }
    removeFile("./cache/debug");
    exit(1);
}

//EXISTING_CODE
//EXISTING_CODE
