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
SFUint32 maxWid = 3000;
//-----------------------------------------------------------------------
void displayTransaction(uint32_t which, const CTransaction *theTrans, void *data) {

    CVisitor *visitor = reinterpret_cast<CVisitor *>(data);
    ASSERT(!visitor->watches[which].disabled);

    const CTransaction *promoted = promoteToFunc(theTrans);
    if (!promoted)
        promoted = theTrans;  // revert to the original

    SFString contractName = visitor->watches[which].name;
    SFString transType = (SFString(promoted->getRuntimeClass()->m_ClassName).substr(1));

    //----------------------------------
    bool wantsEvents = visitor->screenFmt.Contains("[{EVENTS}]");
    SFString format = visitor->screenFmt.Substitute("[{EVENTS}]","");
    if (visitor->toScreen) {

        visitor->setColors(theTrans, visitor->watches[which].color);

        SFString func = promoted->function;
        SFString fmt = format.Substitute("[{FUNCTION}]", func).Substitute("[{FUNC}]", toProper(nextTokenClear(func,'|')));
        fmt.ReplaceAll("[{CONTRACT}]", contractName);
        fmt.ReplaceAll("[{CONTRACT3}]", contractName.Left(3));
        fmt.ReplaceAll("[{TYPE}]", transType);
        fmt.Replace("[{P}]", (promoted == theTrans?"":"\t\tparsed: %[{w:130:PARSED}]#\n"));

        SFString head;
        head += ((promoted->isInternalTx ? promoted->to : contractName) + "::" +
                 transType + (promoted->isInternalTx ? " (logs from " + contractName + ")" : ""));
        fmt.Replace("[{HEAD}]", head);
        fmt.Replace("[{SEP}\n]", SFString('-', maxWid) + "\n");

        SFString transStr = promoted->Format(fmt).Substitute("\t"," ");

        cout << (visitor->useColor ? visitor->color : "");
        cout << transStr.Substitute("#", visitor->color).Substitute("@", visitor->hiColor).Substitute("%", visitor->hiColor2);
        cout << (visitor->useColor ? cOff : "");
    }

    //----------------------------------
    SFString evtList;
    if (visitor->showLogs || wantsEvents) {
        for (int i = 0 ; i < theTrans->receipt.logs.getCount() ; i++) {

            // Try to promote it. If we can't promote it, revert to the original.
            const CLogEntry *l = &theTrans->receipt.logs[i];
            const CLogEntry *promotedLog = promoteToEvent(l);
            if (!promotedLog)
                promotedLog = l;

            // Display it.
            SFString eventType = (SFString(promotedLog->getRuntimeClass()->m_ClassName).substr(1));
            SFString evtStr = promotedLog->toJson1().Left(maxWid);
            if (visitor->showLogs) {
                cout << (visitor->useColor ? iYellow : "");
                cout << "  " << padLeft(asString(i),2) << ". " << padRight(eventType.Left(15),15) << " " << evtStr;
                cout << (visitor->useColor ? cOff : "");
                cout << "\n";
            }
            evtList += eventType + ",";

            // If it was promoted, delete it
            if (promotedLog != l)
                delete promotedLog;
        }
    }
    if (wantsEvents)
        cout << iYellow << "[" << Strip(evtList, ',') << "]              ";

    //----------------------------------
    if (visitor->notify) {

        // Display a Mac notification if we're on Mac
        SFString from = promoted->Format("[{FROM}]").Left(5)+"...";
        SFString to = promoted->Format("[{TO}]").Left(5)+"...";
        SFString cmd = "osascript -e 'display notification \"❤️" + transType + ": " +
                        from + "==>" + to + "\" with title \"" + contractName + "\"'";
        doCommand(cmd);
    }

extern void showColoredTrace(CVisitor *vis, const SFHash& hash);
    if (promoted->isInternalTx)
        showColoredTrace(visitor, promoted->hash);

    // If the transaction was promoted, clear that up
    if (theTrans != promoted)
        delete promoted;

    if (visitor->useColor)
        cout << cOff;
    cout << "\n";
    cout.flush();
}
//EXISTING_CODE

//-----------------------------------------------------------------------
int main(int argc, const char *argv[]) {

    parselib_init("binary");
    if (argc < 2)
        verbose = 0x1;

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    // while (!options.commandList.empty())
    {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        CToml toml;
        toml.setFilename("./config.toml");
        toml.readFile("./config.toml");

        CVisitor visitor;

        blknum_t minWatchBlock = UINT32_MAX;
        uint64_t nWatches = toml.getConfigInt("watches", "nWatches", 0);
        for (uint32_t i = 0 ; i < nWatches ; i++) {
            CAccountWatch watch;
            if (!watch.getWatch(toml, i, false))
                return usage("Invalid watch parameters for watch " + asString(i) + ". Quitting...");
            visitor.watches[visitor.watches.getCount()] = watch;
            minWatchBlock = min(minWatchBlock, watch.firstBlock);
        }
        if (!visitor.watches.getCount())
            return usage("You must specify at least one address to watch in the config file.\n");
        visitor.watches[visitor.watches.getCount()] = CAccountWatch(1, "Others", "Ext Accts", 0, bRed);

        const char* defaultFormat = "";
        visitor.screenFmt = toml.getConfigStr("formats", "screen_fmt", defaultFormat)
                                    .Substitute("\\n\\\n", "\\n")
                                    .Substitute("\n", "")
                                    .Substitute("\\n", "\n")
                                    .Substitute("\\t", "\t");
        visitor.useColor = toml.getConfigBool("display", "use_color", true) && !options.nocolor;
        if (!visitor.useColor)
            colorsOff();
        visitor.toScreen = toml.getConfigBool("display", "to_screen", true);
        visitor.notify = toml.getConfigBool("display", "notify", false);
        visitor.showLogs = toml.getConfigBool("display", "show_logs", false);
        visitor.breakPt = options.bp;
        if (verbose & 0x4)
            visitor.showLogs = true;
        visitor.accounting = toml.getConfigBool("display", "accounting", verbose&0x8);
        visitor.debug = toml.getConfigBool("display", "debug", verbose&0x10);
        maxWid = toml.getConfigInt("display", "max_width", 3000);

        // Showing the cache file (if told to...)
        SFString cacheFileName = "./cache/" + visitor.watches[0].address + ".acct.bin";
        if (fileExists(cacheFileName + ".lck"))
            return usage("Already running. Either quit other instance or remove the lock file: '" + cacheFileName + ".lck'. Quitting...");

        // Figure out which block to start on. Note, if 'master' override is not present, use earliest
        // block from the watches. Not that 'displayCache' may modify this to lastest visited block
        SFUint32 blockNum = max(minWatchBlock, toml.getConfigInt("master", "firstBlock", minWatchBlock));
        if (options.start) {
            blockNum = options.start;
            for (uint32_t i = 0 ; i < visitor.watches.getCount() ; i++) {
                visitor.watches[i].qbis.endBal = getBalance(visitor.watches[i].address, blockNum, false);
            }
        }

        // Display the cache (if the user tells us to...)
        if (!visitor.debug && !verbose) verbose = 1;
        visitor.cacheOnly = !options.mode.Contains("freshen");
        if (options.mode.Contains("showCache"))
            displayCache(cacheFileName, blockNum, &visitor);

        // Freshening the cache (if the user tells us to...)
        if (options.mode.Contains("freshen")) {

            SFUint32 lastVisit = toLongU(asciiFileToString("./cache/lastBlock.txt"));
            blockNum = max(blockNum, lastVisit) + 1;

            SFUint32 lastBlock = getClientLatestBlk();
            SFUint32 nBlocks = (blockNum >= lastBlock ? 0 : lastBlock - blockNum);

            visitor.stats = CStats(blockNum, lastBlock);
            if (verbose)
                visitor.stats.initReport();

            // The cache may have been opened for reading during displayCache, so we
            // close it here, so we can open it back up as append only
            if (visitor.cache.isOpen())
                visitor.cache.Release();
            visitor.cache.m_isReading = false;
            visitor.cache.m_archiveSchema = NO_SCHEMA;
            visitor.cache.m_writeDeleted = true;
            if (visitor.cache.Lock(cacheFileName, "a+", LOCK_WAIT)) {
                forEveryNonEmptyBlockOnDisc(buildCache, &visitor, blockNum, nBlocks);
                visitor.cache.Release();
            }
            visitor.stats.interumReport(lastBlock, visitor.stats.last_ts);
            cout << "\n";
        }
    }

    return 0;
}

