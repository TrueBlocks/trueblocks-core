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

    // Tell the system where the blocks are and which version to use
    etherlib_init("binary");

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    while (!options.commandList.empty()) {

        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        blknum_t latest = getLatestBlockFromCache();
        SFString def = (options.asData ? STR_TERSE_DISPLAY : STR_DEFAULT_DISPLAY);
        SFString fmtStr = getGlobalConfig()->getDisplayStr(options.asData, def);

        // If we are merging and we succeed, we will use the merged file, otherwise we quit. Do this first.
        if (!processMerge(options))
            return 0;

        // If we extract and succeed, we use the extract file for the rest of the modes
        if (!processExtract(options))
            return 0;

        // Handle the various modes (there may be more than one)
        while (!options.mode.empty()) {

            SFString mode = nextTokenClear(options.mode, '|');
            for (uint32_t fn = 0 ; fn < options.filenames.getCount() ; fn++) {

                CAcctCacheItem lastItem(0,0,0);
                CAcctCacheItemArray fixed;

                SFArchive archive(READING_ARCHIVE);  // read from the current cache
                if (archive.Lock(options.filenames[fn], binaryReadOnly, LOCK_NOWAIT)) {

                    cout << toProper(mode)+"ing cache: " << options.filenames[fn] << "\n";
                    while (!archive.Eof()) {

                        CAcctCacheItem item;
                        archive >> item.which >> item.blockNum >> item.transIndex;
                        if (item.blockNum > 0) {
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

                                        fixed[fixed.getCount()] = item;
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

                            } else if (!options.isMerge){
                                return usage("Unknown mode: " + mode + ". Quitting...\n");
                            }
                        }
                    }
                    archive.Release();

                } else {
                    cout << "Could not open file: " << options.filenames[fn] << "\n";
                }

                if (options.stats.nDups) {
                    cout << cMagenta << "\tThe cache has " << options.stats.nDups << " duplicates.\n" << cOff;
                } else if (options.stats.nReversals) {
                    cout << cMagenta << "\tThe cache has " << options.stats.nReversals << " reversals.\n" << cOff;
                } else if (mode == "check") {
                    cout << cMagenta << "\tNo problems found (" << lastItem.blockNum << ").               \n" << cOff;
                }

                blknum_t lastBlock = 0;
                if (options.stats.nFixed || options.stats.nTruncs) {
                    SFString backFile = options.filenames[fn]+".bak";
                    copyFile(options.filenames[fn], backFile);
                    qbSleep(1.0); // wait a second, just in case
                    if (!fileExists(backFile)) {
                        cerr << "Could not create backup file `" << backFile << ". Quitting...";
                        return 1;
                    }
                    cout << "\tRe-writing " << cYellow << fixed.getCount() << cOff
                            << " of " << options.stats.nRecords << " records to cache: "
                            << cYellow << options.filenames[fn] << cOff << " (" << options.stats.nTruncs << " truncated)\n";

                    if (!isTestMode()) {
                        removeFile(options.filenames[fn]);
                        SFArchive archive1(WRITING_ARCHIVE);
                        if (archive1.Lock(options.filenames[fn], "wb", LOCK_NOWAIT)) {
                            for (uint32_t i=0 ; i < fixed.getCount() ; i++) {
                                archive1 << fixed[i].which << fixed[i].blockNum << fixed[i].transIndex;
                                lastBlock = fixed[i].blockNum;
                            }
                            archive1.Release();
                            // write the last block to file
                            CFilename lbFn(options.filenames[fn]);
                            if (!isTestMode())
                                stringToAsciiFile(lbFn.getPath() + "lastBlock.txt", asStringU(lastBlock) + "\r\n");
                        } else {
                            cerr << "Could not create corrected file `" << options.filenames[fn] << ". Quitting...";
                            return 1;
                        }
                    } else {
                    }
                    cout << cMagenta << "\tThe cache was repaired and a backup created.\n" << cOff;

                } else if (mode == "fix") {
                    cout << cMagenta << "\tThere was nothing to fix (" << lastItem.blockNum << ").\n" << cOff;
                    // write the last block to file
                    CFilename lbFn(options.filenames[fn]);
                    if (!isTestMode())
                        stringToAsciiFile(lbFn.getPath() + "lastBlock.txt", asStringU(lastItem.blockNum) + "\r\n");
                }
                cout << "\n";
            }
        }
    }

    if (options.wantsStats) {
        cerr << "Stats option is not implemented. Quitting...\n";
    }

    return 0;
}

//-------------------------------------------------------------------------
int sortByBlock(const void *v1, const void *v2) {
    const CAcctCacheItem *c1 = (const CAcctCacheItem *)v1;
    const CAcctCacheItem *c2 = (const CAcctCacheItem *)v2;
    if ( c1->blockNum   > c2->blockNum   ) return  1;
    if ( c1->blockNum   < c2->blockNum   ) return -1;
    if ( c1->transIndex > c2->transIndex ) return  1;
    if ( c1->transIndex < c2->transIndex ) return -1;
    return (c1->which - c2->which);
}

//-------------------------------------------------------------------------
int sortByAccount(const void *v1, const void *v2) {
    const CAcctCacheItem *c1 = (const CAcctCacheItem *)v1;
    const CAcctCacheItem *c2 = (const CAcctCacheItem *)v2;
    if ( c1->which      > c2->which      ) return  1;
    if ( c1->which      < c2->which      ) return -1;
    if ( c1->blockNum   > c2->blockNum   ) return  1;
    if ( c1->blockNum   < c2->blockNum   ) return -1;
    if ( c1->transIndex > c2->transIndex ) return  1;
    if ( c1->transIndex < c2->transIndex ) return -1;
    return 0;
}

//-------------------------------------------------------------------------
const char *STR_DEFAULT_DISPLAY = "blockNum: [{BLOCKNUM}] transID: [{TRANSINDEX}] which: [{WHICH}]\\n";

//-------------------------------------------------------------------------
const char *STR_TERSE_DISPLAY = "[{BLOCKNUM}]|[{TRANSINDEX}]|[{WHICH}]\\n";
