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
int main(int argc, const char *argv[]) {

    parselib_init(myQuitHandler);
    if (argc < 2)
        verbose = true;
    cout << "Starting monitor...\r"; cout.flush();

    blknum_t clientHeight;
    uint64_t cacheHeight;
    getLatestBlocks(cacheHeight, clientHeight);

    // Parse command line, allowing for command files
    CVisitor visitor;
    if (!visitor.opts.prepareArguments(argc, argv)) {
        return 0;
    }

    // while (!visitor.opts.commandList.empty())
    {
        SFString command = nextTokenClear(visitor.opts.commandList, '\n');
        if (!visitor.opts.parseArguments(command)) {
            return 0;
        }

        if (!fileExists("./config.toml")) {
            cerr << "The config.toml file was not found. Are you in the right folder? Quitting...\n";
            cerr.flush();
            return 0;
        }

        CToml toml("./config.toml");
        visitor.loadWatches(toml);

        const char* defaultFormat = "{ \"date\": \"[{DATE}]\", \"from\": \"[{FROM}]\", \"to\": \"[{TO}]\", \"value\": \"[{VALUE}]\" }";
        visitor.screenFmt          = cleanFmt(toml.getConfigStr("formats", "screen_fmt",  defaultFormat));
        visitor.opts.accounting_on = toml.getConfigBool("display", "accounting", false) || visitor.opts.accounting_on;
        visitor.opts.logs_on       = toml.getConfigBool("display", "logs", false) || visitor.opts.logs_on;
        visitor.opts.trace_on      = toml.getConfigBool("display", "trace", false) || visitor.opts.trace_on;
        visitor.opts.parse_on      = toml.getConfigBool("display", "parse", false) || visitor.opts.parse_on;
        visitor.opts.debugger_on   = toml.getConfigBool("display", "debug", false) || visitor.opts.debugger_on;
        visitor.opts.single_on     = toml.getConfigBool("display", "single", false) || visitor.opts.single_on;
        visitor.opts.kBlock        = visitor.opts.kBlock;

        // Showing the cache file (if told to...)
        SFString cacheFileName = "./cache/" + visitor.watches[0].address + ".acct.bin";
        if (fileExists(cacheFileName + ".lck")) {
            cout << usageStr("The cache lock file is present. The program is either already running or it did not end cleanly the\n"
                            "\tlast time it ran. Quit the already running program or, if it is not running, remove the lock\n"
                            "\tfile: " + cacheFileName + ".lck'. Quitting...")
                    .Substitute("\n", "\r\n");
            cout.flush();cerr.flush();//getchar();
            return 0;
        }

        // Import if there's anything to import
        if (visitor.hasImport()) {
            if (visitor.cache.Lock(cacheFileName, "a+", LOCK_WAIT)) {
                visitor.checkForImport();
                visitor.cache.Release();
            }
        }

        // Figure out which block to start on. Use earliest block from the watches. Note that
        // 'displayFromCache' may modify this to lastest visited block
        uint64_t blockNum = visitor.blockStats.minWatchBlock-1;
        if (visitor.opts.kBlock) {  // we're not starting at the beginning
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

            // TODO(tjayrush): allow for early quiting from debugger--trouble--with this on, and no cache, it
            // immediately quits because displayFromCache returns 'false' for more than one reason
        displayFromCache(cacheFileName, blockNum, &visitor);

        SFTime now = Now();
        cout << argv[0] << ": " << now.Format(FMT_JSON) << ": "
                << "{ "
                << cYellow << visitor.transStats.nDisplayed    << cOff << " displayed from cache; "
                << cYellow << visitor.transStats.nAccountedFor << cOff << " accounted for"
                << " }                        \r\n";

        if (visitor.opts.debugger_on) {
            // If we were debugging and we did nothing, let the user know
            if ((visitor.transStats.nDisplayed + visitor.transStats.nAccountedFor) == 0) {
                cout << "Nothing to do. Hit enter to quit...";
                cout.flush();
                getchar();
            }
        }
    }

    if (visitor.opts.debugger_on) {
        CBlock block;
        getBlock(block, cacheHeight);
        visitor.enterDebugger(block);
    }

    return 0;
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
