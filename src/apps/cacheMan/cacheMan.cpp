/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "options.h"

//-------------------------------------------------------------------------
int main(int argc, const char *argv[]) {

    etherlib_init();

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    while (!options.commandList.empty()) {

        string_q command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        blknum_t latest = getLatestBlockFromCache();
        string_q def = (options.asData ? STR_DATA_DISPLAY : STR_DEFAULT_DISPLAY);
        string_q fmtStr = getGlobalConfig()->getDisplayStr(options.asData, def);

        // Handle the various modes (there may be more than one)
        while (!options.mode.empty()) {

            string_q mode = nextTokenClear(options.mode, '|');
            for (uint32_t fn = 0 ; fn < options.filenames.size() ; fn++) {

                options.stats = CStats(); // reset
                CAcctCacheItem lastItem(0,0);
                CAcctCacheItemArray fixed;
                fixed.reserve(2000000); // just a guess, but makes adding new records very much faster

                // Read from the current cache
                CArchive txCache(READING_ARCHIVE);
                if (txCache.Lock(options.filenames[fn], binaryReadOnly, LOCK_NOWAIT)) {

                    if (!options.asData)
                        cout << toProper(mode)+"ing cache: " << options.filenames[fn] << "\n";
                    while (!txCache.Eof()) {

                        CAcctCacheItem item;
                        txCache >> item.blockNum >> item.transIndex;
                        if (item.blockNum > 0 && item.blockNum < options.maxBlock) {
                            options.stats.nRecords++;
                            bool isDup = (lastItem == item);
                            if (mode == "check") {
                                if (!isTestMode()) {
                                    cerr << "\tChecking block: " << cYellow << item.blockNum << cOff << "\r";
                                    cerr.flush();
                                }
                                if (isDup) {
                                    cout << "\tDuplicate at record " << cYellow << options.stats.nRecords << cOff
                                            << ", current " << cYellow << item << cOff
                                            << ", previous " << cYellow << lastItem << cOff << "\n";
                                    options.stats.nDups++;

                                } else if (item.blockNum < lastItem.blockNum)
                                {
                                    cout << "\tReversal at record " << cYellow << options.stats.nRecords << cOff
                                            << ", current " << cYellow << item << cOff
                                            << ", previous " << cYellow << lastItem << cOff << "\n";
                                    options.stats.nReversals++;

                                } else if (item.blockNum > latest) {
                                    cout << "\tcur: " << cYellow << item << cOff << " prev: " << cYellow << lastItem << cOff << "\n";
                                    cout << "\tBlock number larger than latestBlock (" << latest
                                            << ") found at record: " << cYellow << options.stats.nRecords << cOff
                                            << " current: " << cYellow << item << cOff << ". Your file is probably"
                                            << " currupted. Quitting...\n";
                                    return 0;
                                }
                                lastItem = item;

                            } else if (mode == "fix") {
                                if (isDup) {
                                    cout << "\tDuplicate removed at record " << cYellow << options.stats.nRecords << cOff
                                            << ", current " << cYellow << item << cOff
                                            << ", previous " << cYellow << lastItem << cOff << "\n";
                                    options.stats.nFixed++;

                                } else if (item.blockNum < lastItem.blockNum)
                                {
                                    cout << "\tReversal skipped at record " << cYellow << options.stats.nRecords << cOff
                                            << ", current " << cYellow << item << cOff
                                            << ", previous " << cYellow << lastItem << cOff << "\n";
                                    options.stats.nFixed++;

                                } else {
                                    if (!options.trunc || item.blockNum <= options.trunc) {

                                        fixed.push_back(item);
                                        lastItem = item;
                                        if (!isTestMode()) {
                                            cerr << "\tAccepted block: " << cYellow << item.blockNum << cOff << "\r";
                                            cerr.flush();
                                        }

                                    } else {
                                        options.stats.nTruncs++;
                                    }

                                }

                            } else if (mode == "list") {
                                if (!(options.stats.nRecords % options.skip)) {
                                    cout << item.Format(fmtStr);
                                }

                            } else {
                                return usage("Unknown mode: " + mode + ". Quitting...\n");
                            }
                        }
                    }
                    txCache.Release();

                } else {
                    cout << "Could not open file: " << options.filenames[fn] << "\n";
                }

                if (options.stats.nDups) {
                    cout << cMagenta << "\tThe cache has " << options.stats.nDups << " duplicates.\n" << cOff;
                } else if (options.stats.nReversals) {
                    cout << cMagenta << "\tThe cache has " << options.stats.nReversals << " reversals.\n" << cOff;
                } else if (mode == "check") {
                    cout << cMagenta << "\tNo problems found (n: " << options.stats.nRecords
                            << " / bn: " << lastItem.blockNum << ").               \n" << cOff;
                }

                blknum_t lastBlock = 0;
                CFilename lbFn(options.filenames[fn]);
                string_q lbFileName = lbFn.getPath() + "lastBlock.txt";
                uint32_t prevLastBlock = (uint32_t)str_2_Uint(asciiFileToString(lbFileName));
                if (options.stats.nFixed || options.stats.nTruncs) {
                    if (!isTestMode()) {
                        string_q backFile = options.filenames[fn]+".bak";
                        copyFile(options.filenames[fn], backFile);
                        usleep(1000000); // wait a second, just in case
                        if (!fileExists(backFile)) {
                            cerr << "Could not create backup file `" << backFile << ". Quitting...";
                            return 1;
                        }
                    }
                    cout << "\tRe-writing " << cYellow << fixed.size() << cOff
                            << " of " << options.stats.nRecords << " records to cache: "
                            << cYellow << options.filenames[fn] << cOff << " (" << options.stats.nTruncs << " truncated)\n";

                    if (!isTestMode()) {
                        remove(options.filenames[fn].c_str());
                        CArchive txCache2(WRITING_ARCHIVE);
                        if (txCache2.Lock(options.filenames[fn], "wb", LOCK_NOWAIT)) {
                            for (uint32_t i=0 ; i < fixed.size() ; i++) {
                                txCache2 << fixed[i].blockNum << fixed[i].transIndex;
                                lastBlock = fixed[i].blockNum;
                            }
                            txCache2.Release();
                            // write the last block to file
                            if (lastBlock > prevLastBlock || options.stats.nTruncs)
                                writeLastBlock(lastBlock);
                        } else {
                            cerr << "Could not create corrected file `" << options.filenames[fn] << ". Quitting...";
                            return 1;
                        }
                    }
                    cout << cMagenta << "\tThe cache was repaired and a backup created.\n" << cOff;

                } else if (mode == "fix") {
                    cout << cMagenta << "\tThere was nothing to fix (" << lastItem.blockNum << ").\n" << cOff;
                    // write the last block to file
                    if (!isTestMode()) {
                        if (lastItem.blockNum > prevLastBlock || options.stats.nTruncs)
                            writeLastBlock(lastItem.blockNum);
                    }
                }
            }
        }
    }

    if (isTestMode())
        ::remove("./merged.bin");

    return 0;
}

//-------------------------------------------------------------------------
int sortByBlock(const void *v1, const void *v2) {
    const CAcctCacheItem *c1 = (const CAcctCacheItem *)v1;
    const CAcctCacheItem *c2 = (const CAcctCacheItem *)v2;
    if ( c1->blockNum   > c2->blockNum   ) return  1;
    if ( c1->blockNum   < c2->blockNum   ) return -1;
    return (int)((int64_t)c1->transIndex - (int64_t)c2->transIndex);
}

//-------------------------------------------------------------------------
void writeLastBlock(blknum_t bn) {
    if (isTestMode())
        return;
    stringToAsciiFile("./cache/lastBlock.txt", uint_2_Str(bn) + "\n");
}

//-------------------------------------------------------------------------
const char *STR_DEFAULT_DISPLAY = "blockNum: [{r:10:BLOCKNUM}] transID: [{r:7:TRANSINDEX}]\\n";

//-------------------------------------------------------------------------
const char *STR_DATA_DISPLAY = "[{BLOCKNUM}]\t[{TRANSINDEX}]\\n";
