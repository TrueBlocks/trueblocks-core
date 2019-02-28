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
    char txid  [6];   // less than 100000 + \n
};

//---------------------------------------------------------------
int findFunc(const void *v1, const void *v2) {
    const CIndexRecord *t1 = (const CIndexRecord *)v1;
    const CIndexRecord *t2 = (const CIndexRecord *)v2;
    return strncmp(t1->addr, t2->addr, 42);
}

//---------------------------------------------------------------
bool visitIndexFiles(const string_q& path, void *data) {

    COptions *options = reinterpret_cast<COptions*>(data);
    if (endsWith(path, "/")) {
        forEveryFileInFolder(path + "*", visitIndexFiles, data);

    } else {

        if (contains(path, "/0") && contains(path, "-") && endsWith(path, ".txt")) {

            blknum_t late;
            blknum_t bn = bnFromPath(path, late);
            if (verbose) cerr << path << ": " << bn << " to " << late << endl;

            uint64_t size = fileSize(path);
            uint64_t nRecords = size / sizeof(CIndexRecord);
            options->addrStats.nSeen += nRecords;

            if (bn < options->firstBlock) {
                // We may be too early...
                options->addrStats.nSkipped += nRecords;
                options->addrStats.nHit = fileSize(options->txCache.getFilename()) / (sizeof(uint64_t)*2);
                return true;

            } else if (bn >= (options->firstBlock + options->nBlocks)) {
                // If we're too late....don't continue...
                options->addrStats.nSkipped += nRecords;
                options->writeLastBlock(bn);
                return false;
            }

            cerr << options->name << " bn: " << bn << *options << "\r";
            cerr.flush();

            CMemMapFile blockFile(path, CMemMapFile::WholeFile, CMemMapFile::RandomAccess);
            CIndexRecord *records = (CIndexRecord *)(blockFile.getData());  // NOLINT

            for (size_t ac = 0 ; ac < options->monitors.size() && !shouldQuit() ; ac++) {

                CIndexRecord t;
                const CAccountWatch *acct = &options->monitors[ac];
                strncpy(t.addr, acct->address.c_str(), 42);

                CIndexRecord *found = reinterpret_cast<CIndexRecord*>(bsearch(&t, records, nRecords, sizeof(CIndexRecord), findFunc));
                if (found) {
                    // back up to the first record
                    bool done = false;
                    while (found > records && !done) {
                        --found;
                        if (strncmp(found->addr, acct->address.c_str(), 42)) {
                            done = true;
                            found++;
                        }
                    }

                    // Figure out where we are and how much of the file remains (pointer arithmatic here)
                    uint64_t pRecords = (uint64_t)records;
                    uint64_t pFound   = (uint64_t)found;
                    uint64_t remains  = (pFound - pRecords) / sizeof(CIndexRecord);

                    // now we scan until we hit non-matching record
                    done = false;
                    for (uint64_t i = 0 ; i < remains && !done && !shouldQuit() ; i++) {

                        char bl[10]; bzero(bl, sizeof(bl)); strncpy(bl, found[i].block, 9);
                        char tx[ 6]; bzero(tx, sizeof(tx)); strncpy(tx, found[i].txid,  5);
                        char ad[43]; bzero(ad, sizeof(ad)); strncpy(ad, found[i].addr, 42);
                        if (!strncmp(acct->address.c_str(), found[i].addr, 42)) {

                            // write to the screen (or redirected file)
                            //cout << bl << "\t" << tx << endl;

                            // write to the cache (probably don't need the screen print above)
                            CAcctCacheItem item(str_2_Uint(bl), str_2_Uint(tx));
                            lockSection(true);
                            // We found something...write it to the cache...
                            options->txCache << item.blockNum << item.transIndex;
                            options->txCache.flush();
                            options->writeLastBlock(item.blockNum);
                            lockSection(false);

// TODO(tjayrush)
// stats issue
// the send to api issue
// write blocks issue
// what is the 'blockCounted = false' issue?
// is there a options->blkStats.nHit++ or options->transStats.nHit++ issue?

                            options->addrStats.nHit++;
                            static int rep = 0;
                            if (!(++rep%3813)) {
                                cerr << "    At block #";
                                cerr << item.blockNum;
                                cerr << " we've searched ";
                                cerr << options->addrStats.nSeen;
                                cerr << " records and found ";
                                cerr << options->addrStats.nHit;
                                cerr << " hits" << string_q(80, ' ') << endl;
                                cerr << options->name << " bn: " << item.blockNum << *options << "\r";
                                cerr.flush();
                            }
                            if (!(++rep%1329)) {
                                cerr << options->name << " bn: " << item.blockNum << *options << "\r";
                                cerr.flush();
                            }


                        } else {
                            cerr << options->name << " bn: " << bn << *options << "\r";
                            cerr.flush();
                        }

                        // are we done?
                        if (strncmp(ad, acct->address.c_str(), 42) > 0)
                            done = true;
                    }

                } else {
                    // silently ignore files where the address was not found
                }
            }
            blockFile.close();
            options->writeLastBlock(late);

        } else {
            // silently skip case where file name does not end with .txt
        }
    }
    return !shouldQuit();
}
