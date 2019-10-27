/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-------------------------------------------------------------------------
int main(int argc, const char *argv[]) {

    nodeNotRequired();
    acctlib_init(defaultQuitHandler);

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        blknum_t latest = getLastBlock_cache_final();
        string_q def = (options.data ? STR_DATA_DISPLAY : STR_DEFAULT_DISPLAY);

        // Handle the various modes (there may be more than one)
        CStringArray modes;
        explode(modes, options.mode, '|');
        for (auto mode: modes) {
            uint32_t nOpened = 0;
            for (size_t ac = 0 ; ac < options.monitors.size() ; ac++) {

                CAccountWatch *watch = &options.monitors[ac];

                options.stats = CStats(); // reset
                CAppearance_base lastItem;
                CAppearanceArray_base fixed;
                fixed.reserve(2000000); // just a guess, but makes adding new records very much faster

                CAppearanceArray_base items;
                if (!options.loadMonitorData(items, watch->address)) {
                    // do nothing

                } else {
                    nOpened++;
                    if (!options.data)
                        cerr << toProper(mode)+"ing cache: " << substitute(watch->name, getCachePath(""), "${CACHE}/") << "\n";
                    if (options.exportFmt == JSON1)
                        cout << "[";

                    for (auto item : items) {

                        if (shouldQuit())
                            break;

                        if (item.blk < options.end) {
                            options.stats.nRecords++;
                            bool isDup = (lastItem.blk == item.blk && lastItem.txid == item.txid);
                            if (mode == "check") {
                                if (!isTestMode()) {
                                    cerr << "\tChecking block: " << cYellow << item.blk << cOff << "\r";
                                    cerr.flush();
                                }
                                if (isDup) {
                                    cout << "\tDuplicate at record " << cYellow << options.stats.nRecords << cOff
                                            << ", current " << cYellow << item.blk << "\t" << item.txid << cOff
                                            << ", previous " << cYellow << lastItem.blk << "\t" << lastItem.txid << cOff << "\n";
                                    options.stats.nDups++;

                                } else if (item.blk < lastItem.blk)
                                {
                                    cout << "\tReversal at record " << cYellow << options.stats.nRecords << cOff
                                            << ", current " << cYellow << item.blk << "\t" << item.txid << cOff
                                            << ", previous " << cYellow << lastItem.blk << "\t" << lastItem.txid << cOff << "\n";
                                    options.stats.nReversals++;

                                } else if (item.blk > latest) {
                                    cout << "\tcur: " << cYellow << item.blk << "\t" << item.txid << cOff;
                                    cout << " prev: " << cYellow << lastItem.blk << "\t" << lastItem.txid << cOff << "\n";
                                    cout << "\tBlock number larger than latestBlock (" << latest
                                            << ") found at record: " << cYellow << options.stats.nRecords << cOff
                                            << " current: " << cYellow << item.blk << "\t" << item.txid << cOff << ". Your file is probably"
                                            << " currupted. Quitting...\n";
                                    return 0;
                                }
                                lastItem = item;

                            } else if (mode == "fix") {
                                if (isDup) {
                                    cout << "\tDuplicate removed at record " << cYellow << options.stats.nRecords << cOff
                                            << ", current " << cYellow << item.blk << "\t" << item.txid << cOff
                                            << ", previous " << cYellow << lastItem.blk << "\t" << lastItem.txid << cOff << "\n";
                                    options.stats.nFixed++;

                                } else if (item.blk < lastItem.blk)
                                {
                                    cout << "\tReversal skipped at record " << cYellow << options.stats.nRecords << cOff
                                            << ", current " << cYellow << item.blk << "\t" << item.txid << cOff
                                            << ", previous " << cYellow << lastItem.blk << "\t" << lastItem.txid << cOff << "\n";
                                    options.stats.nFixed++;

                                } else {
                                    if (!options.truncate || item.blk <= options.truncate) {

                                        fixed.push_back(item);
                                        lastItem = item;
                                        if (!isTestMode()) {
                                            cerr << "\tAccepted block: " << cYellow << item.blk << cOff << "\r";
                                            cerr.flush();
                                        }

                                    } else {
                                        options.stats.nTruncs++;
                                    }

                                }

                            } else if (mode == "list") {
                                if (!(options.stats.nRecords % options.skip)) {
                                    static bool first = true;
                                    if (options.exportFmt == JSON1) {
                                        if (!first)
                                            cout << ",";
                                        cout << "{ ";
                                        cout << "\"bn\": " << item.blk << ",";
                                        cout << "\"tx_id\": " << item.txid;
                                        cout << "}\n";
                                        first = false;
                                    } else if (options.exportFmt == CSV1) {
                                        cout << (isApiMode() ? ("\"" + watch->address + "\",") : "");
                                        cout << item.blk << "," << item.txid << endl;
                                    } else {
                                        cout << (isApiMode() ? watch->address + "\t" : "") << item.blk << "\t" << item.txid << endl;
                                    }
                                }

                            } else {
                                return options.usage("Unknown mode: " + mode + ". Quitting...\n");
                            }
                        }
                    }

                    if (options.exportFmt == JSON1)
                        cout << "]";
                }

                if (nOpened == 0)
                    return 0;

                if (options.stats.nDups) {
                    cout << cMagenta << "\tThe cache has " << options.stats.nDups << " duplicates.\n" << cOff;
                } else if (options.stats.nReversals) {
                    cout << cMagenta << "\tThe cache has " << options.stats.nReversals << " reversals.\n" << cOff;
                } else if (mode == "check") {
                    cout << cMagenta << "\tNo problems found (n: " << options.stats.nRecords
                            << " / bn: " << lastItem.blk << ").               \n" << cOff;
                }

                blknum_t lastBlock = 0;
                string_q lbFileName = getMonitorLast(watch->address);
                string_q contents;
                asciiFileToString(lbFileName, contents);
                blknum_t prevLastBlock = str_2_Uint(contents);
                if (options.stats.nFixed || options.stats.nTruncs) {
                    string_q backFile = watch->name+".bak";
                    copyFile(watch->name, backFile);
                    usleep(500000); // wait a second, just in case
                    if (!fileExists(backFile)) {
                        cerr << "Could not create backup file `" << backFile << ". Quitting...";
                        return 1;
                    }
                    cout << "\tRe-writing " << cYellow << fixed.size() << cOff
                            << " of " << options.stats.nRecords << " records to cache: "
                            << cYellow << watch->name << cOff << " (" << options.stats.nTruncs << " truncated)\n";

                    if (fileExists(watch->name))
                        ::remove(watch->name.c_str());
                    CArchive txCache2(WRITING_ARCHIVE);
                    if (txCache2.Lock(watch->name, modeWriteCreate, LOCK_NOWAIT)) {
                        for (size_t i=0 ; i < fixed.size() ; i++) {
                            txCache2 << fixed[i].blk << fixed[i].txid;
                            lastBlock = fixed[i].blk;
                        }
                        txCache2.Release();
                        // write the last block to file
                        if (lastBlock > prevLastBlock || options.stats.nTruncs) {
                            CAccountWatch monitor;
                            monitor.address = watch->address;
                            monitor.writeLastBlock(lastBlock);
                        }
                    } else {
                        cerr << "Could not create corrected file `" << watch->name << ". Quitting...";
                        return 1;
                    }
                    cout << cMagenta << "\tThe cache was repaired and a backup created.\n" << cOff;

                } else if (mode == "fix") {
                    if (!isApiMode())
                        cout << cMagenta << "\tThere was nothing to fix (" << lastItem.blk << ").\n" << cOff;
                    // write the last block to file
                    if (lastItem.blk > prevLastBlock || options.stats.nTruncs) {
                        CAccountWatch monitor;
                        monitor.address = watch->address;
                        monitor.writeLastBlock(lastItem.blk);
                    }
                }
            }
        }
    }

    if (fileExists("./merged.bin"))
        ::remove("./merged.bin");

    acctlib_cleanup();

    return 0;
}

//-------------------------------------------------------------------------
int sortByBlock(const void *v1, const void *v2) {
    const CAppearance_base *c1 = (const CAppearance_base *)v1;
    const CAppearance_base *c2 = (const CAppearance_base *)v2;
    if ( c1->blk > c2->blk) return  1;
    if ( c1->blk < c2->blk) return -1;
    return (int)((int32_t)c1->txid - (int32_t)c2->txid);
}

//-------------------------------------------------------------------------
const char *STR_DEFAULT_DISPLAY = "blockNum: [{r:10:BLOCKNUM}] transID: [{r:7:TRANSINDEX}]\\n";

//-------------------------------------------------------------------------
const char *STR_DATA_DISPLAY = "[{z:9:BLOCKNUM}]\t[{z:5:TRANSINDEX}]\\n";
