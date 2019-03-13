/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "acctlib.h"
#include "options.h"

//----------------------------------------------------------------
struct CIndexRecord {
public:
    char addr  [43];  // '0x' + 40 chars + \t
    char block [10];  // less than 1,000,000,000 + \t
    char txid  [6];   // less than 100,000 + \n
};

//---------------------------------------------------------------
int findFunc(const void *v1, const void *v2) {
    const CIndexRecord *t1 = (const CIndexRecord *)v1;
    const CIndexRecord *t2 = (const CIndexRecord *)v2;
    return strncmp(t1->addr, t2->addr, 42);
}

//---------------------------------------------------------------
bool visitIndexFiles(const string_q& path, void *data) {

    if (endsWith(path, "/")) {
        // Descend into subfolders if there are any
        return forEveryFileInFolder(path + "*", visitIndexFiles, data);

    } else {

        // The context that we're working with...
        COptions *options = reinterpret_cast<COptions*>(data);

        // Silently skip unknown files (such as shell scripts for example). Index filenames take
        // the form of start-end.txt (where `end` is one more that the largest block in the file).
        if (!endsWith(path, ".txt") || !contains(path, "-"))
            return !shouldQuit();

        // Each record in the file is fixed width. So we can know the number of records if we
        // know the file size and the size of the records (see above)
        uint64_t size = fileSize(path);
        uint64_t nRecords = size / sizeof(CIndexRecord);

        // Figure out the range of the record contained in this file (Note: index filenames references one more
        // than the last block contained in the file, so we subtract one).
        blknum_t lastBlockInFile;
        blknum_t firstBlockInFile = bnFromPath(path, lastBlockInFile);
        if (lastBlockInFile > 0)
            lastBlockInFile--;

        options->addrStats.nSeen += nRecords;
        if (options->startScrape > lastBlockInFile) {
            // If the file is too early for this scrape, record some stats, then silently proceed if not told to quit
            options->addrStats.nSkipped += nRecords;
            // Pick up the number of already found records (do this only once)
            if (options->addrStats.nHit == 0) {
                for (auto monitor : options->monitors) {
                    string_q fn = getTransCachePath(monitor.address);
                    if (fileExists(fn))
                        options->addrStats.nHit += fileSize(fn) / (sizeof(uint64_t) * 2);
                }
            }
            return !shouldQuit();
        }

        // We're scraping the file...
        cerr << " bn: " << firstBlockInFile << *options << "\r";
        cerr.flush();

        // Let's try to open the file...
        CMemMapFile blockFile(path, CMemMapFile::WholeFile, CMemMapFile::RandomAccess);
        if (!blockFile.is_open()) {
            // TODO(tjayrush): Should report an error here
            return !shouldQuit();
        }

        if (contains(path, "007341667-007345394"))
            printf("");

        // ...and see if we can get a pointer to its data...
        CIndexRecord *records = (CIndexRecord *)(blockFile.getData());  // NOLINT
        if (!records) {
            // TODO(tjayrush): Should report an error here
            blockFile.close();
            return !shouldQuit();
        }

//        CIndexRecord *end = records + (sizeof(CIndexRecord) * nRecords);

        // ...and now we can search through it
        for (size_t ac = 0 ; ac < options->monitors.size() && !shouldQuit() ; ac++) {
            CAccountWatch *acct = &options->monitors[ac];
            if (!acct->openCacheFile1()) {
                cerr << "Could not open transaction cache file " << getTransCachePath(acct->address) << ". Quitting...";
                return false;
            }

            CIndexRecord search;
            strncpy(search.addr, acct->address.c_str(), 42);

            CIndexRecord *found = reinterpret_cast<CIndexRecord*>(bsearch(&search, records, nRecords, sizeof(CIndexRecord), findFunc));
            if (found) {
                // We found at least one record with this address. Now, we need to back up to the first location of
                // this record so we can write these records. We walk backwards (making sure not to go past the start
                // of the data array) and find the first record with this address.
                bool done = false;
                while (found > records && !done) {
                    --found;
                    if (strncmp(found->addr, acct->address.c_str(), 42)) {
                        done = true;
                        found++;
                    }
                }

                // We know where to start and we know how much remains in the file, so now we spin through the
                // found records writing them to an array for later writing to the hard drive. We want to use
                // an array (with plenty of space) because we want to write the entire memory in one operation.
                CAcctCacheItemArray items;
                items.reserve(300000);

                // Figure out where we are in the file and how much of the file remains (pointer arithmatic)
                done = false;
                uint64_t recordID = ((uint64_t)found - (uint64_t)records) / sizeof(CIndexRecord);
                for (uint64_t i = recordID ; i < nRecords && !done && !shouldQuit() ; i++) {

                    char ad[43]; bzero(ad, sizeof(ad)); strncpy(ad, records[i].addr, 42);
                    char bl[10]; bzero(bl, sizeof(bl)); strncpy(bl, records[i].block, 9);
                    char tx[ 6]; bzero(tx, sizeof(tx)); strncpy(tx, records[i].txid,  5);
                    if (!strncmp(ad, acct->address.c_str(), 42)) {

                        options->addrStats.nHit++;

                        // We found a hit. Save it into an array for later writing to the hard drive
                        CAcctCacheItem item(str_2_Uint(bl), str_2_Uint(tx));
                        items.push_back(item);

                        // TODO(tjayrush)
                        // stats issue
                        // the send to api issue
                        // write blocks issue
                        // what is the 'blockCounted = false' issue?
                        // is there a options->blkStats.nHit++ or options->transStats.nHit++ issue?

                        static int rep = 0;
                        if (!(++rep%3813)) {
                            cerr << "    At block #";
                            cerr << item.blockNum;
                            cerr << " we've searched ";
                            cerr << options->addrStats.nSeen;
                            cerr << " records and found ";
                            cerr << options->addrStats.nHit;
                            cerr << " hits" << string_q(80, ' ') << endl;
                            cerr << " bn: " << item.blockNum << *options << "\r";
                            cerr.flush();
                        }

                        if (!(++rep%1329)) {
                            cerr << " bn: " << bl << *options << "\r";
                            cerr.flush();
                        }

                    } else {
                        cerr << " bn: " << bl << *options << "\r";
                        cerr.flush();

                    }

                    // Are we done looking?
                    if (strncmp(ad, acct->address.c_str(), 42) != 0)
                        done = true;
                }

                // If we found any records, we want to write them to the monitor's cache and record the starting point
                // for the next scrape of this address
                if (items.size()) {
                    lockSection(true);
                    acct->writeAnArray(items);
                    acct->writeLastBlock(lastBlockInFile + 1); // where to start our next search
                    lockSection(false);
                }

            } else {
                acct->writeLastBlock(lastBlockInFile + 1); // where to start our next search

            }

        } // end of `for` loop over monitors

        // We're done with this index file, so we can close it
        blockFile.close();
    }

    return !shouldQuit();
}
