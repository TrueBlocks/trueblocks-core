/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//#define MAX_ROWS 50
//#define CLIENT 10480200
//#define N_BLOCKS 100
//#define THE_CMD "/Users/jrush/Development/trueblocks-core/bin/blaze scrape"
#define MAX_ROWS 1000000
#define CLIENT (client + 0)
#define N_BLOCKS (n_blocks + 0)
#define THE_CMD "blaze scrape"
//--------------------------------------------------------------------------
bool COptions::scrape_once(void) {
#if 0
    LOG_INFO(string_q(120, '-'));

    maxIndexRows = MAX_ROWS;  // not configurable really

    // Find the last visited block. (It's the later of ripe, staging, or finalized.)
    blknum_t unused1, ripe, staging, finalized, client;
    getLatestBlocks(unused1, ripe, staging, finalized, client);
    client = CLIENT;

    // The latest of finalized block, staging block, or ripe block is the last 'good' block. Start one past that...
    blknum_t lastVisit = max(ripe, max(staging, finalized));
    blknum_t startBlock = lastVisit + 1;

    // In some cases, the index may be ahead of tip. In that case, we doing nothing...
    if (startBlock > client) {
        LOG_INFO("The index (", startBlock, ") is caught up to the tip of the chain (", client, ").");
        return false;
    }

    if (isDockerMode()) {
        n_blocks = 100;
        n_block_procs = 5;
        n_addr_procs = 10;
    }

    const CToml* config = getGlobalConfig("blockScrape");
    n_blocks = config->getConfigInt("settings", "n_blocks", (n_blocks == NOPOS ? 2000 : n_blocks));
    n_block_procs = config->getConfigInt("settings", "n_block_procs", (n_block_procs == NOPOS ? 10 : n_block_procs));
    n_addr_procs = config->getConfigInt("settings", "n_addr_procs", (n_addr_procs == NOPOS ? 20 : n_addr_procs));

    // Docker will kill blaze if it uses too many resources...
    if (!isDockerMode()) {
        // ...so we only override on n_blocks to speed things up if not docker...
        if (startBlock < 450000) {
            n_blocks = max((blknum_t)4000, n_blocks);

        } else if (ddosRange(startBlock)) {
            // ...or slow things down...
            n_blocks = getGlobalConfig("blockScrape")->getConfigInt("settings", "n_blocks_fallback", 200);
        }
    }
    n_blocks = N_BLOCKS;

    // If a block is more than 28 blocks from the head we consider it 'ripe.' Once a block
    // goes ripe, we no longer ask the node about it. We try to move it to staging. Staging
    // means it's ready to be consolidated (finalized). 28 blocks is an arbitrarily chosen value,
    // but is a bit more than six minutes under normal operation ((14 * 28) / 60 == 6.5). If the
    // index is near the head of the chain and the difficulty level is high (the time bomb is
    // exploding), the time will extend, but the final nature is the same.
    blknum_t ripeBlock = client - 28;
    if (client < 28)
        ripeBlock = 0;

    // Now we want to do a final adjustment to n_blocks to make sure it doesn't go past the
    // last synced block at the client
    if ((startBlock + n_blocks) > client) {
        ASSERT(startBlock <= client);  // see above
        n_blocks = (client - startBlock);
    }

    string_q cmd = THE_CMD;
    // We're ready to scrape, so build the blaze command line...
    ostringstream os;
    os << cmd;
    os << " --startBlock " << startBlock << " --nBlocks " << n_blocks;
    if (n_block_procs != 20)
        os << " --nBlockProcs " << n_block_procs;
    if (n_addr_procs != 60)
        os << " --nAddrProcs " << n_addr_procs;

    // Report to the screen...
    string_q tmp = "cmd: " + substitute(os.str(), "/Users/jrush/Development/trueblocks-core/bin/", "");
    LOG_INFO(cGreen, tmp, cOff, " (", (client - startBlock), " blocks from head)");

    // ...and make the call to blaze.
    os << " --ripeBlock " << ripeBlock;
    // os << " 2>/dev/null 1>/dev/null ";
    if (system(os.str().c_str()) != 0) {
        // blaze will return non-zero if it fails. In this case, we need to remove files in the 'ripe'
        // folder because they're inconsistent (blaze's 'go' processes do not run in sequence). We
        // blindly clean all ripe files, which is a bit of overill, but it's easy and it works.
        // Next time we run, it will start over at the last staged block.
        cleanFolder(indexFolder_ripe);
        LOG_WARN("Blaze quit without finishing. Reprocessing...");
        return false;
    }

    LOG_INFO("\r", string_q(120, ' '), "\r");

    // Blaze succeeded, but the user may have started `acctScrape` since it was called. We don't want
    // to get incorrect results, so we return here knowing it will get cleaned up next time.
    if (isRunning("acctScrape")) {
        LOG_WARN("acctScrape is running. blockScrape will re-run in a moment...");
        return false;
    }

    // Blaze sucessfullly created individual files, one for each block between 'start' and 'start + n_blocks'.
    // Each contains a list of addresses that appear in that block. The unripe folder holds blocks that
    // are less than 28 blocks old. We do nothing further with them, but the query tool (acctScrape)
    // may use them if it wants to.

    // From this point until the end of this invocation, the processing must be able to stop abruptly
    // without resulting in corrupted data (control+c for example). This means we must process a single
    // file at a time in sequential order. Processing means moving files from ripe to staging and then
    // (if applicable) from the staging file into a finalized chunk. If we stop processing at any
    // point, we want to leave the data in a state where the next invocation of this program can either
    // clean up or pick up where it left off.

    // Next, we processes one file in the ripe folder at a time by appending its data to a temporary
    // staging file. We flush the data after each append and then remove the ripe file. If this process
    // is interrupted, we clean up both the ripe folder and the temporary staging file. This may be
    // over kill, but it's safer. In effect, we start over at the most recently successfully staged block
    // the next time through. The value `prevBlock` points to the last completed staging block. The last
    // block in the file is ../staged/{prevBlock}.txt. We're also processing time stamps (which can get
    // ahead of the scraper) at the same time.

    // Pick up the latest of zero, finalized, or staging...
    blknum_t p = (staging == NOPOS ? (finalized == NOPOS ? 0 : finalized) : staging);

    // Carries state we need to process the blocks
    CConsolidator cons(p);
    if (!cons.tmp_file.is_open()) {
        LOG_WARN("Could not open temporary staging file.");
        return false;
    }

    // Spin through 'ripe' files and process as we go. Note: it's okay to allow the timestamp file to get
    // ahead of the staged blocks. We only write when the block number is a new one not already in the file.
    if (!forEveryFileInFolder(indexFolder_ripe, visitCopyRipeToStage, &cons)) {
        // Something went wrong with copying the ripe blocks to staging. (i.e. the user hit control+c or we
        // encountered a non-sequential list of files). We clean up and start over the next time through.
        cleanFolder(indexFolder_unripe);
        cleanFolder(indexFolder_ripe);
        cons.tmp_file.close();
        ::remove(cons.tmp_fn.c_str());
        return false;
    }
    cons.tmp_file.close();

    // The stage now contains all non-consolidated records. Ripe should be empty. All files are closed.

    // Next, we try to pick off chunks of 500,000 records (maxIndexRows) if we can, consolidate them (write
    // them to a binary relational table), and re-write any unfinalized records back onto the stage. Again, if
    // anything goes wrong we need clean up and leave the data in a recoverable state.
    if (!finalize_chunks(&cons)) {
        cleanFolder(indexFolder_unripe);
        cleanFolder(indexFolder_ripe);
        ::remove(cons.tmp_fn.c_str());
    }
#endif

    // FIX_THIS_CODE
    freshenTimestamps(getLatestBlock_cache_ripe());
    LOG_INFO("Run complete.");

    return true;
}

//--------------------------------------------------------------------------
bool COptions::finalize_chunks(CConsolidator* cons) {
    // 'oldStage' contains staged but not yet consolidated records. 'tempStage' contains
    // newly scraped ripe blocks. 'newStage' is empty. This temporary file is where we will put
    // the records from these two files. We need to process in a way that will allow for interruption.

    // ...we want to work with a temporary file so if anything goes wrong, we can recover...
    string_q tmpFile = getIndexPath("temp.txt");
    string_q oldStage = getLastFileInFolder(indexFolder_staging, false);
    string_q tempStage = cons->tmp_fn;
    string_q newStage = indexFolder_staging + padNum9(cons->prevBlock) + ".txt";

    if (oldStage == newStage) {
        blknum_t curSize = fileSize(newStage) / 59;
        LOG_INFO(bBlue, "Consolidation not ready...", cOff);
        LOG_INFO(cYellow, "  No new blocks. Have ", curSize, " records of ", maxIndexRows, ". Need ",
                 (maxIndexRows - curSize), " more.", cOff);
        return true;
    }

    if (oldStage != tempStage) {
        if (!appendFile(tmpFile, oldStage)) {
            // oldStage is still valid. Caller will clean up the rest
            LOG_ERR("Could not append oldStage to temp.fil.");
            return false;
        }
    }

    // ...next we append the new ripe records if we can...
    if (!appendFile(tmpFile, tempStage)) {
        // oldStage is still valid. Caller will clean up the rest
        ::remove(tmpFile.c_str());
        LOG_ERR("Could not append tempStage to temp.fil.");
        return false;
    }

    // ...finally, we move the temp file to the new stage without allowing user to hit control+c
    lockSection(true);
    ::rename(tmpFile.c_str(), newStage.c_str());
    if (oldStage != tempStage && oldStage != newStage)
        ::remove(oldStage.c_str());
    ::remove(tempStage.c_str());
    lockSection(false);

    // We are now in a valid state with all records in the properly named newStage
    blknum_t curSize = fileSize(newStage) / 59;

    // Did user hit control+c?
    if (shouldQuit())
        return true;

    // If we don't have enough records to consolidate, tell the user and return...
    if (curSize <= maxIndexRows) {
        LOG_INFO(" ");
        LOG_INFO(bBlue, "Consolidation not ready...", cOff);
        LOG_INFO(cYellow, "  Have ", curSize, " records of ", maxIndexRows, ". Need ", (maxIndexRows - curSize),
                 " more.", cOff);
        return true;
    }

    // Process until curSize is less than maxIndexRows. This may mean more than one pass
    size_t pass = 0;
    while (curSize > maxIndexRows && !shouldQuit()) {
        lockSection(true);

        CStringArray lines;
        lines.reserve(curSize + 100);
        asciiFileToLines(newStage, lines);

        LOG_INFO(" ");
        LOG_INFO(bBlue, "Consolidation pass ", pass++, cOff);
        LOG_INFO(cWhite, "  Starting search at record ", (maxIndexRows - 1), " of ", lines.size(), cOff);
        if (verbose > 2) {
            LOG_INFO(cGreen, "\t", (maxIndexRows - 1), ": ", lines[maxIndexRows - 1], cOff);
            LOG_INFO(cGreen, "\t", (maxIndexRows), ": ", lines[maxIndexRows], cOff);
        }

        size_t where = 0;
        string_q prev;
        for (uint64_t record = (maxIndexRows - 1); record < lines.size() && where == 0; record++) {
            CStringArray pParts;
            explode(pParts, lines[record], '\t');
            if (verbose > 2 && (record == lines.size() - 2)) {
                LOG_INFO(bBlue, "\t", record, ": ", pParts[0], " -- ", pParts[1], " -- ", pParts[2], cOff);
            }
            if (prev != pParts[1]) {
                if (!prev.empty())
                    where = record - 1;
                prev = pParts[1];
            }
        }
        if (where == 0) {
            LOG_INFO("The weird edge case");
            where = lines.size() - 1;
        }

        LOG_INFO(cWhite, "  Found a break at line ", where, " extracting records 0 to ", where, " (inclusive) of ",
                 lines.size(), cOff);
        if (verbose > 2) {
            LOG_INFO(cGreen, "\t", 0, ": ", lines[0], cOff);
            LOG_INFO(cGreen, "\t", 1, ": ", lines[1], cOff);
            LOG_INFO(bBlue, "\t", where, ": ", lines[where], cOff);
            LOG_INFO(bTeal, "\t", (where + 1), ": ", lines[where + 1], cOff);
        }

        CStringArray consolidatedLines;
        consolidatedLines.reserve(lines.size());
        for (uint64_t record = 0; record <= where; record++) {
            if (countOf(lines[record], '\t') != 2) {
                LOG_WARN("Found a record with less than two tabs. Quitting...");
                LOG_WARN("preceeding line:\t[", ((record > 0) ? lines[record - 1] : "N/A"), "]");
                LOG_WARN("offending line:\t[", lines[record], "]");
                LOG_WARN("following line:\t[", ((record < where) ? lines[record + 1] : "N/A"), "]");
                return false;
            }
            consolidatedLines.push_back(lines[record]);
        }

        CStringArray p1;
        explode(p1, consolidatedLines[0], '\t');
        CStringArray p2;
        explode(p2, consolidatedLines[consolidatedLines.size() - 1], '\t');

        sort(consolidatedLines.begin(), consolidatedLines.end());
        string_q binFile = indexFolder_finalized + p1[1] + "-" + p2[1] + ".bin";
        writeIndexAsBinary(binFile, consolidatedLines);

        if (pin) {
            CPinnedItem pinRecord;
            pinChunk(p1[1] + "-" + p2[1], pinRecord);
        }

        if (publish) {
            publishManifest(cout);
        }
        LOG_INFO(cWhite, "  Wrote ", consolidatedLines.size(), " records to ",
                 substitute(binFile, indexFolder_finalized, "$FINAL/"), cOff);

        where += 1;
        CStringArray remainingLines;
        remainingLines.reserve(maxIndexRows + 100);

        if (verbose > 2) {
            LOG_INFO(cWhite, "  Extracting records ", where, " to ", lines.size(), " of ", lines.size(), cOff);
            LOG_INFO(cGreen, "\t", where, ": ", lines[where], cOff);
            LOG_INFO(cGreen, "\t", (where + 1), ": ", lines[where + 1], cOff);
            LOG_INFO(bBlue, "\t", (where - 1), ": ", lines[where - 1], cOff);
            LOG_INFO(bTeal, "\t", (where), ": ", lines[where], cOff);
        }

        for (uint64_t record = where; record < lines.size(); record++) {
            remainingLines.push_back(lines[record]);
        }

        ::remove(newStage.c_str());
        linesToAsciiFile(newStage, remainingLines, true);
        LOG_INFO(cWhite, "  Wrote ", remainingLines.size(), " records to ",
                 substitute(newStage, indexFolder_staging, "$STAGING/"), cOff);

        curSize = fileSize(newStage) / 59;
        lockSection(false);
    }
    return true;
}

//--------------------------------------------------------------------------
bool visitCopyRipeToStage(const string_q& path, void* data) {
    if (endsWith(path, '/')) {
        return forEveryFileInFolder(path + "*", visitCopyRipeToStage, data);

    } else {
        blknum_t e_unused;
        timestamp_t ts;
        blknum_t bn = bnFromPath(path, e_unused, ts);

        // If we're not one behind, we have a problem
        CConsolidator* con = reinterpret_cast<CConsolidator*>(data);
        if ((con->prevBlock + 1) != bn) {
            // For some reason, we're missing a file. Quit and try again next time
            LOG_WARN("Current file (", path, ") does not sequentially follow previous file ", con->prevBlock, ".");
            return false;
        }

        ifstream inputStream(path, ios::in);
        if (!inputStream.is_open()) {
            // Something went wrong, try again next time
            LOG_WARN("Could not open input stream ", path);
            return false;
        }

        lockSection(true);
        con->tmp_file << inputStream.rdbuf();
        con->tmp_file.flush();
        inputStream.close();
        ::remove(path.c_str());
        con->prevBlock = bn;
        lockSection(false);
    }

    return !shouldQuit();
}

//--------------------------------------------------------------------------
bool appendFile(const string_q& toFile, const string_q& fromFile) {
    ofstream output;
    output.open(toFile, ios::out | ios::app);
    if (!output.is_open())
        return false;

    ifstream input(fromFile, ios::in);
    if (!input.is_open()) {
        output.close();
        return false;
    }

    output << input.rdbuf();
    output.flush();
    input.close();

    return true;
}

//--------------------------------------------------------------------------
CConsolidator::CConsolidator(blknum_t p) {
    prevBlock = p;
    tmp_fn = indexFolder_staging + "000000000-temp.txt";
    tmp_file.open(tmp_fn, ios::out | ios::trunc);
}

#if 0
//------------------------------------------------------------------------------------------------
bool COptions::start_scraper(void) {
    ENTER("handle_" + mode);
    nodeRequired();

    bool daemonMode = false;

    if (contains(tool_flags, "restart")) {
    } else if (contains(tool_flags, "pause")) {
    } else if (contains(tool_flags, "quit")) {
    } else {
        CStringArray optList;
        explode(optList, tool_flags, ' ');

        // Clean up the tool flags and pass them on to the blockScrape program
        tool_flags = "";  // reset tool_flag
        for (auto opt : optList) {
            if (opt == "--daemon") {
                daemonMode = true;

            } else if (!opt.empty()) {
                if (!startsWith(opt, "-") && !isNumeral(opt)) {
                    cerr << "Invalid options '" << opt << "' to blockScrape." << endl;
                    EXIT_NOMSG(false);
                }

                if (!contains(opt, "start") && !contains(opt, "end")) {
                    tool_flags += (opt + " ");
                }
            }
        }

        cerr << cYellow << "Scraper is starting with " << tool_flags << "..." << cOff << endl;
    }

    timestamp_t userSleep = (scrapeSleep == 0 ? 500000 : scrapeSleep * 1000000);

    // Run forever...unless told to pause or stop (shouldQuit is true if control+C was hit.
    bool waitFileExists = fileExists(waitFile);
    size_t nRuns = 0;
    size_t maxRuns = (isTestMode() ? 1 : UINT64_MAX);
    while (nRuns++ < maxRuns && !shouldQuit()) {
        if (waitFileExists) {
            if (!wasPaused)
                cerr << cYellow << "\tScraper paused..." << cOff << endl;
            wasPaused = true;
            usleep(max(useconds_t(5), scrapeSleep) * 1000000);  // sleep for at least five seconds

        } else {
            timestamp_t startTs = date_2_Ts(Now());
            if (wasPaused)
                cerr << cYellow << "\tScraper restarted..." << cOff << endl;
            wasPaused = false;
            ostringstream os;
            os << "blockScrape " << tool_flags;
            LOG_CALL(os.str());
            // clang-format off
            if (system(os.str().c_str())) {}  // Don't remove cruft. Silences compiler warnings
            // clang-format on

            if (isTestMode()) {
                // Do nothing related in --daemon mode while testing

            } else {
                CMonitorArray monitors;
                if (daemonMode) {
                    // Catch the monitors addresses up to the scraper if in --deamon mode
                    forEveryFileInFolder(getMonitorPath("") + "*", visitMonitor, &monitors);

                    if (!freshen_internal2(monitors, freshen_flags))
                        EXIT_FAIL("'chifra " + mode + "' returns false");

                    for (auto monitor : monitors) {
                        //#error
                        if (true) {  // monitor.needsRefresh) {
                            static size_t nThings = 0;
                            ostringstream os1;
                            os1 << "acctExport " << monitor.address << " --freshen";  // << " >/dev/null";
                            LOG_INFO("Calling: ", os1.str(), string_q(40, ' '), "\r");
                            // clang-format off
                            if (system(os1.str().c_str())) {}  // Don't remove cruft. Silences compiler warnings
                            // clang-format on
                            if (shouldQuit())
                                continue;
                            if (!(nThings % 10))
                                usleep(125000);  // stay responsive to cntrl+C
                        }
                    }
                }
                timestamp_t now = max(startTs, date_2_Ts(Now()));  // not less than
                timestamp_t timeSpent = (now - startTs) * 1000000;
                timestamp_t sleepSecs = timeSpent > userSleep ? 0 : userSleep - timeSpent;
                // LOG_INFO("startTs: ", startTs, " now: ", now, " timeSpent: ", timeSpent, " userSleep: ", userSleep, "
                // sleepSecs: ", sleepSecs, string_q(60, ' '));
                if (daemonMode)
                    LOG_INFO(cYellow, "Finished freshening ", monitors.size(), " monitored addresses. Sleeping for ",
                             (sleepSecs / 1000000), " seconds", string_q(40, ' '), cOff);
                usleep(useconds_t(sleepSecs));  // stay responsive to cntrl+C
            }
        }

        waitFileExists = fileExists(waitFile);
        if (waitFileExists) {
            if (asciiFileToString(waitFile) == "quit") {
                ::remove(waitFile.c_str());
                cerr << cYellow << "\tScraper quitting..." << cOff << endl;
                EXIT_NOMSG(true);
            }
        }
    }
    EXIT_NOMSG(true);
}

//------------------------------------------------------------------------------------------------
bool freshen_internal2(CMonitorArray& fa, const string_q& freshen_flags) {
    ENTER("freshen_internal");
    nodeNotRequired();

    ostringstream base;
    base << "acctScrape " << freshen_flags << " [ADDRS] ;";

    size_t cnt = 0, cnt2 = 0;
    string_q tenAddresses;
    for (auto f : fa) {
        bool needsUpdate = true;
        if (needsUpdate) {
            LOG4(cTeal, "Needs update ", f.address, string_q(80, ' '), cOff);
            tenAddresses += (f.address + " ");
            if (!(++cnt % 10)) {  // we don't want to do too many addrs at a time
                tenAddresses += "|";
                cnt = 0;
            }
        } else {
            LOG4(cTeal, "Scraping addresses ", f.address, " ", cnt2, " of ", fa.size(), string_q(80, ' '), cOff, "\r");
        }
        cnt2++;
    }

    // Process them until we're done
    uint64_t cur = 0;
    while (!tenAddresses.empty()) {
        string_q thisFive = nextTokenClear(tenAddresses, '|');
        string_q cmd = substitute(base.str(), "[ADDRS]", thisFive);
        LOG_CALL(cmd);
        // clang-format off
        uint64_t n = countOf(thisFive, ' ');
        if (fa.size() > 1)
            LOG_INFO(cTeal, "Scraping addresses ", cur, "-", (cur+n-1), " of ", fa.size(), string_q(80, ' '), cOff);
        cur += n;
        if (system(cmd.c_str())) {}  // Don't remove cruft. Silences compiler warnings
        // clang-format on
        if (!tenAddresses.empty())
            usleep(250000);  // this sleep is here so that chifra remains responsive to Cntl+C. Do not remove
    }

    for (CMonitor& f : fa)
        f.needsRefresh = (f.cntBefore != f.getRecordCount());

    EXIT_NOMSG(true);
}
#endif

//---------------------------------------------------------------------------
bool visitMonitor(const string_q& path, void* data) {
    if (!endsWith(path, ".acct.bin"))  // we only want to process monitor files
        return true;

    CMonitor m;
    m.address = substitute(substitute(path, getMonitorPath(""), ""), ".acct.bin", "");
    if (isAddress(m.address)) {
        m.cntBefore = m.getRecordCount();
        m.needsRefresh = false;
        CMonitorArray* array = (CMonitorArray*)data;  // NOLINT
        array->push_back(m);
        //        LOG_INFO(cTeal, "Loading addresses ", m.address, " ", array->size(), string_q(80, ' '), cOff, "\r");
    }

    return true;
}
