/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "acctlib.h"
#include "options.h"

//---------------------------------------------------------------
bool COptions::visitAsciiFile(const string_q& path, void *data) {

    uint64_t size_of_file = fileSize(path);
    uint64_t nRecords = size_of_file / sizeof(CIndexRecord);

    CMemMapFile *blockFile = NULL;
    CIndexRecord *records = NULL;

    for (size_t ac = 0 ; ac < monitors.size() && !shouldQuit() ; ac++) {
        CAccountWatch *acct = &monitors[ac];
        if (!acct->openCacheFile1()) {
            cerr << "Could not open transaction cache file " << getMonitorPath(acct->address) << ". Quitting...";
            return false;
        }

        CAcctCacheItemArray items;
        items.reserve(300000);

        if (!blockFile) {
            blockFile = new CMemMapFile(path, CMemMapFile::WholeFile, CMemMapFile::RandomAccess);
            if (!blockFile) {
                cerr << "Could not open index file " << path << ". Quitting...";
                return false;
            }

            if (!blockFile->is_open()) {
                // TODO(tjayrush): Should report an error here
                return !shouldQuit();
            }

            records = (CIndexRecord *)(blockFile->getData());  // NOLINT
            if (!records) {
                // TODO(tjayrush): Should report an error here
                blockFile->close();
                delete blockFile;
                blockFile = NULL;
                return !shouldQuit();
            }
        }

        CIndexRecord search;
        size_t sz = sizeof(search);
        bzero(&search, sz);
        strncpy(search.addr, acct->address.c_str(), 42);

        CIndexRecord *found = reinterpret_cast<CIndexRecord*>(bsearch(&search, records, nRecords, sizeof(CIndexRecord), findFunc));
        if (found) {
            bool done = false;
            while (found > records && !done) {
                --found;
                if (strncmp(found->addr, acct->address.c_str(), 42)) {
                    done = true;
                    found++;
                }
            }

            done = false;
            uint64_t recordID = ((uint64_t)found - (uint64_t)records) / sizeof(CIndexRecord);
            for (uint64_t i = recordID ; i < nRecords && !done && !shouldQuit() ; i++) {

                char ad[43]; bzero(ad, sizeof(ad)); strncpy(ad, records[i].addr, 42);
                char bl[10]; bzero(bl, sizeof(bl)); strncpy(bl, records[i].block, 9);
                char tx[ 6]; bzero(tx, sizeof(tx)); strncpy(tx, records[i].txid,  5);
                if (!strncmp(ad, acct->address.c_str(), 42)) {

                    addrStats.nHit++;

                    CAcctCacheItem item(str_2_Uint(bl), str_2_Uint(tx));
                    items.push_back(item);

                    // TODO(tjayrush)
                    // stats issue
                    // the send to api issue
                    // write blocks issue
                    // what is the 'blockCounted = false' issue?
                    // is there a blkStats.nHit++ or transStats.nHit++ issue?

                    //                            static int rep = 0;
                    //                            if (!(++rep%3813)) {
                    //                                cerr << "    At block #";
                    //                                cerr << item.blockNum;
                    //                                cerr << " we've searched ";
                    //                                cerr << addrStats.nSeen;
                    //                                cerr << " records and found ";
                    //                                cerr << addrStats.nHit;
                    //                                cerr << " hits" << string_q(80, ' ') << endl;
                    //                                cerr << " bn: " << item.blockNum << *options << "\r";
                    //                                cerr.flush();
                    //                            }
                    //
                    //                            if (!(++rep%1329)) {
                    //                                cerr << " bn: " << bl << *options << "\r";
                    //                                cerr.flush();
                    //                            }
                    //
                    //                        } else {
                    //                            cerr << " bn: " << bl << *options << "\r";
                    //                            cerr.flush();

                }

                if (strncmp(ad, acct->address.c_str(), 42) != 0)
                    done = true;
            }

            if (items.size()) {
                lockSection(true);
                acct->writeAnArray(items);
                acct->writeLastBlock(lastBlockInFile + 1);
                lockSection(false);
            }

        } else {
            acct->writeLastBlock(lastBlockInFile + 1);

        }
    }

    if (blockFile) {
        blockFile->close();
        delete blockFile;
        blockFile = NULL;
    }

    return !shouldQuit();
}

