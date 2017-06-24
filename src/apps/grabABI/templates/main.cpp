/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"
#include "parselib.h"

//EXISTING_CODE
//-----------------------------------------------------------------------
bool displayTransaction(uint32_t which, const CTransaction *theTrans, void *data) {

    CVisitor *visitor = reinterpret_cast<CVisitor *>(data);
    ASSERT(!visitor->watches[which].disabled);
    visitor->nDisplayed++;
    visitor->lastWhich = which;
    visitor->lastTrans = theTrans;

    const CTransaction *promoted = promoteToFunc(theTrans);
    if (!promoted)
        promoted = theTrans;  // revert to the original

    SFString contractName = visitor->watches[which].name;
    SFString transType = (SFString(promoted->getRuntimeClass()->m_ClassName).substr(1));

    //----------------------------------
    bool wantsEvents = visitor->screenFmt.Contains("[{EVENTS}]");
    bool wantsParse = visitor->screenFmt.Contains("[{PARSE}]");
    SFString format  = visitor->screenFmt.Substitute("[{EVENTS}]","").Substitute("[{PARSE}]","");
    if (!format.empty()) {

        visitor->setColors(theTrans, visitor->watches[which].color);

        SFString func = promoted->function;
        SFString fmt = format.Substitute("[{FUNCTION}]", func).Substitute("[{FUNC}]", toProper(nextTokenClear(func,'|')));
        fmt.ReplaceAll("[{CONTRACT}]", contractName);
        fmt.ReplaceAll("[{CONTRACT3}]", contractName.Left(3));
        fmt.ReplaceAll("[{TYPE}]", transType);
        fmt.ReplaceAll("[{TYPE20}]", padRight(transType.Left(15),15) + (theTrans->isInternalTx ? "" : "`"));
        fmt.Replace("[{P}]", (promoted == theTrans?"":"\t\tparsed: %[{w:130:PARSED}]#\r\n"));

        SFString head;
        head += ((promoted->isInternalTx ? promoted->to : contractName) + "::" +
                 transType + (promoted->isInternalTx ? " (logs from " + contractName + ")" : ""));
        fmt.Replace("[{HEAD}]", head);
        fmt.Replace("[{SEP}\r\n]", SFString('-', 180) + "\r\n");

        SFString transStr = promoted->Format(fmt).Substitute("\t"," ");

        SFString c1 = visitor->color, c2 = visitor->hiColor, c3 = visitor->hiColor2, c4 = cOff;
        if (theTrans->isError) {
            c1 = c2 = c3 = c4 = biBlack;
        } else if (theTrans->isInternalTx) {
            c1 = c2 = c3 = c4 = cRed;
        }
        cout << c1 << transStr.Substitute("#", c1).Substitute("@", c2).Substitute("%", c3).Substitute("`", c4) << cOff;
    }

    //----------------------------------
    if (visitor->parse_on || wantsParse) {
        SFString parsed = promoted->Format("\r\n[{PARSED}]\r\n")
        .Substitute(" ", "").Substitute(",", ", ").Substitute("{", "{ ").Substitute("}", " }");
        cout << iTeal << Strip(parsed, ',');
    }

    //----------------------------------
    SFString evtList;
    if (visitor->logs_on || wantsEvents) {
        if (visitor->logs_on)
            cout << "\r\n";
        for (int i = 0 ; i < theTrans->receipt.logs.getCount() ; i++) {

            // Try to promote it. If we can't promote it, revert to the original.
            const CLogEntry *l = &theTrans->receipt.logs[i];
            const CLogEntry *promotedLog = promoteToEvent(l);
            if (!promotedLog)
                promotedLog = l;

            // Display it.
            SFString eventType = (SFString(promotedLog->getRuntimeClass()->m_ClassName).substr(1));
            SFString evtStr = promotedLog->toJson1();
            if (visitor->logs_on) {
                cout << iYellow << "  " << padLeft(asString(i),2) << ". " << padRight(eventType.Left(15),15) << " " << evtStr << cOff << "\r\n";
            }
            evtList += eventType + ",";

            // If it was promoted, delete it
            if (promotedLog != l)
                delete promotedLog;
        }
    }
    if (wantsEvents)
        cout << iYellow << "[" << Strip(evtList, ',') << "]";

    //----------------------------------
    if (visitor->notify) {

        // Display a Mac notification if we're on Mac
        SFString from = promoted->Format("[{FROM}]").Left(5)+"...";
        SFString to = promoted->Format("[{TO}]").Left(5)+"...";
        SFString cmd = "osascript -e 'display notification \"❤️" + transType + ": " +
                        from + "==>" + to + "\" with title \"" + contractName + "\"'";
        doCommand(cmd);
    }

    // TODO(tjrayrush): when should we show the accounting traces?
    if (visitor->trace_on || visitor->autoTrace) {
        visitor->showColoredTrace(promoted->hash, theTrans->isError);
        if (visitor->bloom_on && promoted->receipt.logsBloom != 0) {
            showColoredBloom(promoted->receipt.logsBloom, "Tx bloom:", "");
            cout << "\r\n";
            for (int t=0;t<visitor->watches.getCount()-1;t++) {
                SFBloom b = makeBloom(visitor->watches[t].address);
                showColoredBloom(b,visitor->watches[t].color + padRight(visitor->watches[t].name.Left(9),9) + cOff, (isBloomHit(b, promoted->receipt.logsBloom) ? greenCheck : redX));
                cout << "\r\n";
            }
        }
    }

    // If the transaction was promoted, clear that up
    if (theTrans != promoted)
        delete promoted;

    cout << cOff;
    cout << "\r\n";
    cout.flush();
    return true;
}
//EXISTING_CODE

//-----------------------------------------------------------------------
#include <ncurses.h>
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
    exit(1);
}
inline void myOnExitHandler(void) { myQuitHandler(1); }

//-----------------------------------------------------------------------
int main(int argc, const char *argv[]) {

    registerQuitHandler(myQuitHandler);

    parselib_init("binary");
    if (argc < 2)
        verbose = 0x1;

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv)) {
        cout.flush();
        cerr.flush();
        //getchar();
        return false;
    }

    // while (!options.commandList.empty())
    {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command)) {
            cout.flush();
            cerr.flush();
            //getchar();
            return false;
        }

        CToml toml;
        toml.setFilename("./config.toml");
        toml.readFile("./config.toml");

        CVisitor visitor;

        blknum_t minWatchBlock = UINT32_MAX;
        uint64_t nWatches = toml.getConfigInt("watches", "nWatches", 0);
        for (uint32_t i = 0 ; i < nWatches ; i++) {
            CAccountWatch watch;
            if (!watch.getWatch(toml, i, false)) {
                cout << usageStr("Invalid watch parameters for watch " + asString(i) + ". Quitting...")
                            .Substitute("\n", "\r\n");
                cout.flush();
                cerr.flush();
                //getchar();
                return false;
            }
            visitor.watches[visitor.watches.getCount()] = watch;
            minWatchBlock = min(minWatchBlock, watch.firstBlock);
        }
        if (!visitor.watches.getCount()) {
            cout << usageStr("You must specify at least one address to watch in the config file.\r\n")
                        .Substitute("\n", "\r\n");
            cout.flush();
            cerr.flush();
            //getchar();
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
        visitor.accounting_on = toml.getConfigBool("display", "accounting", false) || options.accounting_on;
        visitor.logs_on       = toml.getConfigBool("display", "logs", false) || options.logs_on;
        visitor.trace_on      = toml.getConfigBool("display", "trace", false) || options.trace_on;
        visitor.parse_on      = toml.getConfigBool("display", "parse", false) || options.parse_on;
        visitor.bloom_on      = toml.getConfigBool("display", "bloom", false) || options.bloom_on;
        visitor.debugger_on   = toml.getConfigBool("display", "debug", false) || options.debugger_on;
        visitor.single_on     = toml.getConfigBool("display", "single", false) || options.single_on;

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
        if (options.kBlock) {
            blockNum = options.kBlock;
            for (uint32_t i = 0 ; i < visitor.watches.getCount() ; i++) {
                visitor.watches[i].qbis.endBal = getBalance(visitor.watches[i].address, blockNum, false);
            }
        }

        if (visitor.debugger_on) {
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
        visitor.cacheOnly = !options.mode.Contains("freshen");
        if (options.mode.Contains("showCache")) {
            // TODO(tjayrush): allow for early quiting from debugger--trouble--with this on, and no cache, it immediately quits
            // because displayFromCache returns 'false' for more than one reason
            if (!displayFromCache(cacheFileName, blockNum, &visitor))
                options.mode = ""; // do not continue
        }

        // Freshening the cache (if the user tells us to...)
        if (options.mode.Contains("freshen")) {

            SFUint32 lastVisit = toLongU(asciiFileToString("./cache/lastBlock.txt"));
            blockNum = max(blockNum, lastVisit) + 1;
            SFUint32 lastBlock = getClientLatestBlk();
            SFUint32 nBlocks = (blockNum >= lastBlock ? 0 : lastBlock - blockNum);

            visitor.nBlocksToVisit= lastBlock-blockNum;
            visitor.startBlock = blockNum;
            visitor.endBlock = lastBlock;
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
            SFString endMsg = dateFromTimeStamp(tsOut).Format(FMT_JSON) + " (" + asString(lastBlock) + ")";
            visitor.interumReport(lastBlock, visitor.last_ts, endMsg);
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
