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
#include "debug.h"

//EXISTING_CODE
//EXISTING_CODE

//-----------------------------------------------------------------------
#define cleanFmt(str) ((str).Substitute("\\n\\\n", "\\n").Substitute("\n", "").Substitute("\\n", "\r\n").Substitute("\\t", "\t"))

//-----------------------------------------------------------------------
int main(int argc, const char *argv[]) {

    parselib_init("binary");

    registerQuitHandler(myQuitHandler);
    if (argc < 2)
        verbose = true;

    SFUint32 topOfChain = getLatestBloomFromCache();

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

        CToml toml("./config.toml");
        visitor.loadWatches(toml);

        const char* defaultFormat = "{ \"date\": \"[{DATE}]\", \"from\": \"[{FROM}]\", \"to\": \"[{TO}]\", \"value\": \"[{VALUE}]\" }";
        visitor.screenFmt  = cleanFmt(toml.getConfigStr("formats", "screen_fmt",  defaultFormat));
        visitor.verboseFmt = cleanFmt(toml.getConfigStr("formats", "verbose_fmt", ""));
        visitor.opts.accounting_on = toml.getConfigBool("display", "accounting", false) || visitor.opts.accounting_on;
        visitor.opts.logs_on       = toml.getConfigBool("display", "logs", false) || visitor.opts.logs_on;
        visitor.opts.trace_on      = toml.getConfigBool("display", "trace", false) || visitor.opts.trace_on;
        visitor.opts.parse_on      = toml.getConfigBool("display", "parse", false) || visitor.opts.parse_on;
        visitor.opts.bloom_on      = toml.getConfigBool("display", "bloom", false) || visitor.opts.bloom_on;
        visitor.opts.debugger_on   = toml.getConfigBool("display", "debug", false) || visitor.opts.debugger_on;
        visitor.opts.single_on     = toml.getConfigBool("display", "single", false) || visitor.opts.single_on;
        visitor.opts.kBlock        = visitor.opts.kBlock;
        visitor.opts.mode          = visitor.opts.mode;

        // Showing the cache file (if told to...)
        SFString cacheFileName = "./cache/" + visitor.watches[0].address + ".acct.bin";
        if (fileExists(cacheFileName + ".lck")) {
            cout << usageStr("The cache lock file is present. The program is either already running or it did not end cleanly the\n"
                            "\tlast time it ran. Quit the already running program or, if it is not running, remove the lock\n"
                            "\tfile: " + cacheFileName + ".lck'. Quitting...")
                    .Substitute("\n", "\r\n");
            cout.flush();cerr.flush();getchar();
            return false;
        }

        // Figure out which block to start on. Use earliest block from the watches. Note that
        // 'displayFromCache' may modify this to lastest visited block
        SFUint32 blockNum = visitor.minWatchBlock-1;
        if (visitor.opts.kBlock) {
            blockNum = visitor.opts.kBlock;
            for (uint32_t i = 0 ; i < visitor.watches.getCount() ; i++) {
                visitor.watches[i].qbis.endBal = getBalance(visitor.watches[i].address, blockNum, false);
            }
        }

        if (visitor.opts.debugger_on) {
            removeFile("./cache/debug");
            initscr();
            raw();
            keypad(stdscr, true);
            noecho();
            refresh();
            atexit(myOnExitHandler);
            cout << "Starting balances:\r\n"; cout.flush();
        }

        // Display the cache (if the user tells us to...)
        if (!visitor.opts.debugger_on && !verbose) verbose = 1;
        if (visitor.opts.mode.Contains("showCache")) {
            // TODO(tjayrush): allow for early quiting from debugger--trouble--with this on, and no cache, it
            // immediately quits because displayFromCache returns 'false' for more than one reason
            if (!displayFromCache(cacheFileName, blockNum, &visitor))
                visitor.opts.mode = ""; // do not continue
        }

        // Freshening the cache (if the user tells us to...)
        if (visitor.opts.mode.Contains("freshen")) {

            SFUint32 lastVisit  = toLongU(asciiFileToString("./cache/lastBlock.txt"));
            blockNum = max(blockNum, lastVisit) + 1;

            visitor.endBlock       = min(topOfChain, visitor.maxWatchBlock);
            visitor.startBlock     = min(blockNum,   visitor.endBlock);
            visitor.nBlocksToVisit = visitor.endBlock - visitor.startBlock;
            cerr << "Freshening from "
                << visitor.startBlock << " to "
                << visitor.endBlock << " ("
                << visitor.nBlocksToVisit << " blocks)\r\n";
            cerr.flush();
            // The cache may have been opened for reading during displayFromCache, so we
            // close it here, so we can open it back up as append only
            if (visitor.cache.isOpen())
                visitor.cache.Release();
            visitor.cache.m_isReading = false;
            visitor.cache.m_archiveSchema = NO_SCHEMA;
            visitor.cache.m_writeDeleted = true;
            if (visitor.cache.Lock(cacheFileName, "a+", LOCK_WAIT)) {
                forEveryBloomFile(updateCacheUsingBlooms, &visitor, visitor.startBlock, visitor.nBlocksToVisit);
                visitor.cache.Release();
            }
            if (visitor.nFreshened) {
                timestamp_t tsOut = toTimeStamp(Now());
                SFString endMsg = dateFromTimeStamp(tsOut).Format(FMT_JSON) + " (" + asString(topOfChain) + ")";
                visitor.interumReport(visitor.endBlock, visitor.last_ts, endMsg);
                cout << "\r\n";
            }
        }

        SFTime now = Now();
        cout << getprogname() << ": " << now.Format(FMT_JSON) << ": "
                << "{ "
                << cYellow << visitor.nDisplayed    << cOff << " displayed from cache; "
                << cYellow << visitor.nFreshened    << cOff << " written to cache; "
                << cYellow << visitor.nAccountedFor << cOff << " accounted for"
                << " }\r\n";

        if (visitor.opts.debugger_on && visitor.nProcessed() == 0) {
            cout << "Nothing to do. Hit enter to quit...";
            cout.flush();
            getchar();
        }
    }

    if (visitor.opts.debugger_on) {
        CBlock block;
        getBlock(block, topOfChain);
        visitor.enterDebugger(block);
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
