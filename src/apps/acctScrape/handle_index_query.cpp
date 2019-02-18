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

extern void writeLastBlock(blknum_t bn);
//---------------------------------------------------------------
bool visitIndexFiles(const string_q& path, void *data) {

    COptions *options = reinterpret_cast<COptions*>(data);
    if (endsWith(path, "/")) {
        forEveryFileInFolder(path + "*", visitIndexFiles, data);

    } else {

        if (endsWith(path, ".txt")) {

            blknum_t late;
            blknum_t bn = bnFromPath(path, late);
//            cout << "Searching from block " << bn << " to " << late << endl;
            uint64_t size = fileSize(path);
            uint64_t nRecords = size / sizeof(CIndexRecord);
            options->addrStats.nSeen += nRecords;

            if (bn < options->firstBlock) {
                // We may be too early...
                options->addrStats.nSkipped += nRecords;
                return true;

            } else if (bn >= (options->firstBlock + options->nBlocks)) {
                // If we're too late....don't contiue...
                options->addrStats.nSkipped += nRecords;
                writeLastBlock(bn);
                return false;
            }

//            if (options->debugging) { cerr << "Opening index file " << path << "\n"; }
            cerr << options->name << " bn: " << bn << *options << "\r";
            cerr.flush();

            CMemMapFile blockFile(path, CMemMapFile::WholeFile, CMemMapFile::RandomAccess);
            CIndexRecord *records = (CIndexRecord *)(blockFile.getData());  // NOLINT

            for (size_t ac = 0 ; ac < options->monitors.size() && !shouldQuit() ; ac++) {
                const CAccountWatch *acct = &options->monitors[ac];
                CIndexRecord t;
                strncpy(t.addr, acct->address.c_str(), 42);
                CIndexRecord *found = reinterpret_cast<CIndexRecord*>(bsearch(&t, records, nRecords, sizeof(CIndexRecord), findFunc));
                if (found) {
                    bool done = false;
                    while (found > records && !done) {
                        --found;
                        if (strncmp(found->addr, acct->address.c_str(), 42)) {
                            done = true;
                            found++;
                        }
                    }
                    uint64_t pRecords = (uint64_t)records;
                    uint64_t pFound   = (uint64_t)found;
                    uint64_t remains  = (pFound - pRecords) / sizeof(CIndexRecord);
                    done = false;
                    for (uint64_t i = 0 ; i < remains && !done && !shouldQuit() ; i++) {
                        char bl[10];
                        bzero(bl, 10);
                        strncpy(bl, found[i].block, 9);
                        char tx[6];
                        bzero(tx, 6);
                        strncpy(tx, found[i].txid, 5);
                        char ad[43];
                        bzero(ad, 43);
                        strncpy(ad, found[i].addr, 42);
                        if (!strncmp(acct->address.c_str(), found[i].addr, 42)) {
                            cout << bl << "\t" << tx << endl;
//                            CAcctCacheItem item(str_2_Uint(bl), str_2_Uint(tx));
//                            lockSection(true);
//                            // We found something...write it to the cache...
//                            options->txCache << item.blockNum << item.transIndex;
//                            options->txCache.flush();
//                            writeLastBlock(item.blockNum);
//                            lockSection(false);
                            options->addrStats.nHit++;
                            static int rep = 0;
                            if (!(++rep%9801)) {
                                cerr << "    At block #";
                                cerr << bn;
                                cerr << " we've searched ";
                                cerr << options->addrStats.nSeen;
                                cerr << " records and found ";
                                cerr << options->addrStats.nHit;
                                cerr << " hits" << string_q(80, ' ') << endl;
//                                cerr << "        " << acct->displayName(false,true,true,8)
//                                    << ": blk: " << bl << " tx: " << tx
//                                    << " (" << options->blkStats.nSeen << " of " << options->nBlocks << ") "
//                                    << "                    " << endl;
                            }
                        } else {
//                            if (verbose) {
                                cerr << options->name << " bn: " << bn << *options << "\r";
                                cerr.flush();
//                            }
                        }
                        if (strncmp(ad, acct->address.c_str(), 42) > 0)
                            done = true;
                    }
                }
            }
            blockFile.close();
            writeLastBlock(late);

//            lockSection(true);
//            for (auto item : items) {
//                // We found something...write it to the cache...
//                options->txCache << item.blockNum << item.transIndex;
//                options->txCache.flush();
//                writeLastBlock(item.blockNum);
#if 0
//TODO
                //
                // stats issue
                // api issue
                // write blocks issue
                // what is the 'blockCounted = false' issue?
                // is there a options->blkStats.nHit++ or options->transStats.nHit++ issue?
                // can we remove the oneBlock issue
                //
                // Send the data to an api if we have one
                if (!acct->api_spec.uri.empty()) {
                    if (trans->traces.size() == 0)
                        getTraces(((CTransaction*)trans)->traces, trans->hash);
                        acct->abi_spec.articulateTransaction((CTransaction*)trans);
                        if (!trans->articulatedTx.message.empty())
                            SHOW_FIELD(CFunction, "message");
                        ((CAccountWatch*)acct)->api_spec.sendData(trans->Format());
                        HIDE_FIELD(CFunction, "message");
                        cout << "\n";
                        cout.flush();
                    }
                    // Also, we optionally write blocks if we're told to do so
                    if (options->writeBlocks) {
                        string_q fn = getBinaryFilename(block.blockNumber);
                        if (!fileExists(fn)) {
                            CBlock *pBlock = (CBlock*)&block;
                            pBlock->finalized = isBlockFinal(block.timestamp, options->lastTimestamp, (60 * 4));
                            writeBlockToBinary(block, fn);
                        }
                    }
#endif
//            }
//            lockSection(false);

        } else {
            // silently skip case where file name does not end with .bin
        }
    }
    return !shouldQuit();
}
