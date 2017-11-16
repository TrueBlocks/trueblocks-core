/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"
#include "acctcacheitem.h"

//-------------------------------------------------------------------------
extern bool     processMerge(COptions& options);
extern uint64_t processExtract(COptions& options);
extern uint64_t processRenumber(COptions& options);
extern SFString display(const SFString& msg, const COptions& opt, uint64_t nRecords, const CAcctCacheItem& item, bool pad);

//-------------------------------------------------------------------------
int main(int argc, const char *argv[]) {
    // Tell the system where the blocks are and which version to use
    etherlib_init("binary");

    CAcctCacheItem::registerClass();

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    while (!options.commandList.empty()) {

        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        // If this is a merge, do the merge and then quit...
        if (!processMerge(options))
            return 0;

        // Otherwise handle each mode (there may be more than one)
        while (!options.mode.empty()) {

            SFString mode = nextTokenClear(options.mode, '|');
            if (mode == "extract")
            {
                // returns the number of transactions extracted
                uint64_t n = processExtract(options);
                if (!n)
                    return usage("No records were extracted for contact: " + asStringU(options.extractID) + "\n");
                cerr << "\t" << asStringU(n) << " records were extracted into ./extract.bin\n";
                return 0;
            }

            if (mode == "renum")
            {
                // returns the number of transactions renumbered
                uint64_t n = processRenumber(options);
                if (!n)
                    return usage("No records were renumberd\n");
                cerr << "\t" << asStringU(n) << " records were renumbered into ./<fn>_renum.bin\n";
                return 0;
            }

            blknum_t latest = getLatestBlockFromCache();
            for (uint32_t fn=0 ; fn < options.filenames.getCount() ; fn++) {

                uint64_t nDups = 0, nReversals = 0, nFixed = 0, nRecords = 0, nTruncs = 0;
                CAcctCacheItem lastItem(0,0,0);
                CAcctCacheItemArray fixed;

                SFArchive archive(READING_ARCHIVE);  // we only ever read the current cache
                if (archive.Lock(options.filenames[fn], binaryReadOnly, LOCK_NOWAIT)) {

                    cout << toProper(mode)+"ing cache: " << options.filenames[fn] << "\n";
                    while (!archive.Eof()) {

                        CAcctCacheItem item;
                        archive >> item.which >> item.blockNum >> item.transIndex;
                        if (item.blockNum > 0) {
                            nRecords++;
                            bool isDup = (lastItem == item);
                            if (mode == "check") {
                                if (!isTestMode()) {
                                    cerr << "\tChecking block: " << asYellow(item.blockNum) << "\r";
                                    cerr.flush();
                                }
                                if (isDup) {
                                    cout << "\tDuplicate at record " << asYellow(nRecords)
                                            << ", current " << asYellow(item)
                                            << ", previous " << asYellow(lastItem) << "\n";
                                    nDups++;

                                } else if (item.blockNum < lastItem.blockNum)
                                {
                                    cout << "\tReversal at record " << asYellow(nRecords)
                                            << ", current " << asYellow(item)
                                            << ", previous " << asYellow(lastItem) << "\n";
                                    nReversals++;

                                } else if (item.blockNum > latest) {
                                    cout << "\tcur: " << asYellow(item) << " prev: " << asYellow(lastItem) << "\n";
                                    cout << display("\tBlock number larger than latestBlock (" + asStringU(latest) + ") found at record: ",
                                                    options,
                                                    nRecords,
                                                    item,
                                                    false) << ". Quitting...\n";
                                    return 0;
                                }
                                lastItem = item;

                            } else if (mode == "fix") {
                                if (isDup) {
                                    cout << "\tDuplicate removed at record " << asYellow(nRecords)
                                            << ", current " << asYellow(item)
                                            << ", previous " << asYellow(lastItem) << "\n";
                                    nFixed++;

                                } else if (item.blockNum < lastItem.blockNum)
                                {
                                    cout << "\tReversal skipped at record " << asYellow(nRecords)
                                            << ", current " << asYellow(item)
                                            << ", previous " << asYellow(lastItem) << "\n";
                                    nFixed++;

                                } else {
                                    if (!options.trunc || item.blockNum <= options.trunc) {

                                        fixed[fixed.getCount()] = item;
                                        lastItem = item;
                                        if (!isTestMode()) {
                                            cerr << "\tAccepted block: " << asYellow(item.blockNum) << "\r";
                                            cerr.flush();
                                        }

                                    } else {
                                        nTruncs++;
                                    }

                                }

                            } else if (mode == "list") {
                                if (!(nRecords % options.skip))
                                    cout << display("", options, nRecords, item, true) << "\n";
                            } else {
                                return usage("Unknown mode: " + mode + ". Quitting...\n");
                            }
                        }
                    }
                    archive.Release();

                } else {
                    cout << "Could not open file: " << options.filenames[fn] << "\n";
                }

                if (nDups) {
                    cout << cMagenta << "\tThe cache has " << nDups << " duplicates.\n" << cOff;
                } else if (nReversals) {
                    cout << cMagenta << "\tThe cache has " << nReversals << " reversals.\n" << cOff;
                } else if (mode == "check") {
                    cout << cMagenta << "\tNo problems found (" << lastItem.blockNum << ").               \n" << cOff;
                }

                blknum_t lastBlock = 0;
                if (nFixed || nTruncs) {
                    SFString backFile = options.filenames[fn]+".bak";
                    copyFile(options.filenames[fn], backFile);
                    qbSleep(1.0); // wait a second, just in case
                    if (!fileExists(backFile)) {
                        cerr << "Could not create backup file `" << backFile << ". Quitting...";
                        return 1;
                    }
#if 0
                    cout << "Would write to " << options.filenames[fn] << "\n";
                    cout << "backup " << backFile << ": " << fileExists(backFile) << "\n";
#else
                    removeFile(options.filenames[fn]);
                    SFArchive archive1(WRITING_ARCHIVE);
                    if (archive1.Lock(options.filenames[fn], "wb", LOCK_NOWAIT)) {
                        cout << "\tRe-writing " << asYellow(fixed.getCount())
                                << " of " << nRecords << " records to cache: "
                                << asYellow(options.filenames[fn]) << " (" << nTruncs << " truncated)\n";
                        for (uint32_t i=0 ; i < fixed.getCount() ; i++) {
                            archive1 << fixed[i].which << fixed[i].blockNum << fixed[i].transIndex;
                            lastBlock = fixed[i].blockNum;
                        }
                        archive1.Release();
                        // write the last block to file
                        CFilename lbFn(options.filenames[fn]);
                        stringToAsciiFile(lbFn.getPath() + "lastBlock.txt", asStringU(lastBlock) + "\r\n");
                    } else {
                        cerr << "Could not create corrected file `" << options.filenames[fn] << ". Quitting...";
                        return 1;
                    }
#endif
                    cout << cMagenta << "\tThe cache was repaired and a backup created.\n" << cOff;
                } else if (mode == "fix") {
                    cout << cMagenta << "\tThere was nothing to fix (" << lastItem.blockNum << ").\n" << cOff;
                    // write the last block to file
                    CFilename lbFn(options.filenames[fn]);
                    stringToAsciiFile(lbFn.getPath() + "lastBlock.txt", asStringU(lastItem.blockNum) + "\r\n");
                }
                cout << "\n";
            }
        }
    }

    return 0;
}

//-------------------------------------------------------------------------
int sortTheCache(const void *v1, const void *v2) {
    const CAcctCacheItem *c1 = (const CAcctCacheItem*)v1;
    const CAcctCacheItem *c2 = (const CAcctCacheItem*)v2;
    int ret = (int)(c1->blockNum - c2->blockNum);
    if (ret)
        return ret;
    ret = (int)(c1->transIndex - c2->transIndex);
    if (ret)
        return ret;
    return (int)(c1->which - c2->which);
}

//-------------------------------------------------------------------------
bool processMerge(COptions& options) {

    if (!options.isMerge)
        return true;

    cerr << cYellow << "You've asked to merge the following cache files:\n" << cOff;
    for (uint32_t i = 0 ; i < options.filenames.getCount() ; i++)
        cerr << "\t" << options.filenames[i] << "\n";
    cerr << iMagenta << "\tThe result will be placed in the file ./merged.bin.\n" << cOff;
    cerr << "\tContinue? (y=yes) > ";
    int ch = getchar();
    if (ch != 'y' && ch != 'Y') {
        cerr << "Merge request ignored.\n";
        return false;  // return false so we don't continue
    }

    bool renumber=false;
    cerr << iMagenta << "\tDo you want to renumber the contracts?\n" << cOff;
    cerr << "\tContinue? (y=yes) > ";
    ch = getchar();
    if (ch != 'y' && ch != 'Y') {
        cerr << "Not renumbering.\n";
    } else {
        renumber = true;
    }

    cerr << "Caches are being merged.\n";
    CAcctCacheItemArray merged;
    for (uint32_t i = 0 ; i < options.filenames.getCount() ; i++) {
        cerr << "\tMerging file: " << options.filenames[i] << "\n";
        SFArchive cache(READING_ARCHIVE);
        if (!cache.Lock(options.filenames[i], binaryReadOnly, LOCK_NOWAIT))
            return usage("Could not open file: " + options.filenames[i] + ". Quitting.");
        while (!cache.Eof()) {
            uint32_t which = 0; uint64_t blockNum = 0, transID = 0;
            cache >> which >> blockNum >> transID;
            merged[merged.getCount()] = CAcctCacheItem(blockNum, transID, renumber ? (int32_t)i : (int32_t)which);
        }
        cache.Release();
    }
    merged.Sort(sortTheCache);

    SFArchive mergeFile(WRITING_ARCHIVE);
    if (!mergeFile.Lock("./merged.bin", binaryWriteCreate, LOCK_WAIT))
        return usage("Could not open merge file: ./merged.bin. Quitting.");
    for (uint32_t i = 0 ; i < merged.getCount() ; i++)
        mergeFile << merged[i].which << merged[i].blockNum << merged[i].transIndex;
    mergeFile.Release();

    // We must fix the file to removed duplicates
    if (!options.mode.Contains("fix"))
        options.mode += "|fix";
    options.mode.ReplaceAll("||", "|");

    // We are only interested now in the merged file
    options.filenames.Clear();
    options.filenames[options.filenames.getCount()] = "./merged.bin";

    return true;
}

//-------------------------------------------------------------------------
uint64_t processExtract(COptions& options) {
    cerr << "Transactions for contract " << options.extractID << " are being extracted.\n";
    CAcctCacheItemArray extract;
    for (uint32_t i = 0 ; i < options.filenames.getCount() ; i++) {
        cerr << "\tExtracting from file: " << options.filenames[i] << "\n";
        SFArchive cache(READING_ARCHIVE);
        if (!cache.Lock(options.filenames[i], binaryReadOnly, LOCK_NOWAIT))
            return (uint64_t)usage("Could not open file: " + options.filenames[i] + ". Quitting.");
        while (!cache.Eof()) {
            uint32_t which = 0; uint64_t blockNum = 0, transID = 0;
            cache >> which >> blockNum >> transID;
            if (which == options.extractID)
                extract[extract.getCount()] = CAcctCacheItem(blockNum, transID, (int32_t)which);
        }
        cache.Release();
    }

    SFArchive extractFile(WRITING_ARCHIVE);
    if (!extractFile.Lock("./extract.bin", binaryWriteCreate, LOCK_WAIT))
        return (uint64_t)usage("Could not open extract file: ./extract.bin. Quitting.");
    for (uint32_t i = 0 ; i < extract.getCount() ; i++)
        extractFile << extract[i].which << extract[i].blockNum << extract[i].transIndex;
    extractFile.Release();

    return extract.getCount();
}

//-------------------------------------------------------------------------
uint64_t processRenumber(COptions& options) {
    cerr << "Transactions are being renumbered.\n";
    uint64_t totalRenumered = 0;
    for (uint32_t i = 0 ; i < options.filenames.getCount() ; i++) {
        SFString fn = options.filenames[i];
        SFString rn = options.filenames[i].Substitute(".bin","_renum.bin");
        cerr << "\tRenumbering file " << fn << " to " << rn << "\n";

        SFArchive cache(READING_ARCHIVE);
        if (!cache.Lock(fn, binaryReadOnly, LOCK_NOWAIT))
            return (uint64_t)usage("Could not open file: " + fn + ". Quitting.");
        SFArchive renum(WRITING_ARCHIVE);
        if (!renum.Lock(rn, binaryWriteCreate, LOCK_WAIT))
            return (uint64_t)usage("Could not open file: " + rn + ". Quitting.");

        while (!cache.Eof()) {
            uint32_t which = 0; uint64_t blockNum = 0, transID = 0;
            cache >> which >> blockNum >> transID;
            for (uint32_t jj = 0 ; jj < options.renums.getCount() ; jj += 2) {
                if (which == options.renums[jj]) {
                    which = (uint32_t)options.renums[jj+1];
                    totalRenumered++;
                }
            }
            renum << which << blockNum << transID;
        }
        cache.Release();
        renum.Release();
    }
    return totalRenumered;
}

//--------------------------------------------------------------------------------
SFString display(const SFString& msg, const COptions& opt, uint64_t nRecords, const CAcctCacheItem& item, bool pad) {
    CStringExportContext ctx;
    if (opt.blockOnly) {
        ctx << cYellow << (pad ? padNum9(item.blockNum) : asStringU(item.blockNum)) << cOff;

    } else {
        if (!msg.empty()) {
            ctx << msg << cYellow << (pad ? padNum9(nRecords) : asStringU(nRecords)) << cOff << " ";
        }
        ctx << "blockNum: " << cYellow << (pad ? padNum9(item.blockNum) : asStringU(item.blockNum)) << cOff;
        ctx << " transID: "  << cYellow << (pad ? padNum5(item.transIndex) : asStringU(item.transIndex)) << cOff;
        ctx << " which: "    << cYellow << (pad ? padNum5((uint64_t)item.which) : asString(item.which)) << cOff;
    }
    return ctx.str;
}
