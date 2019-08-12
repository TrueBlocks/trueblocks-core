/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

#if 0
//----------------------------------------------------------------------------------
string_q relativePath(const string_q& path, const string_q& relTo, const string_q& rep="./") {
    string_q rel = (relTo.empty() ? getCWD() : relTo);
    return substitute(path, rel, rep);
}
#endif
//-------------------------------------------------------------------------
int main(int argc, const char *argv[]) {
    nodeNotRequired();
    acctlib_init(defaultQuitHandler);

#if 0
    CStatus::registerClass();
    CCache::registerClass();
    CAddressCache::registerClass();
    CAbiCache::registerClass();
    CChainCache::registerClass();
    CMonitorCache::registerClass();
    CMonitorCacheItem::registerClass();
    CSlurpCache::registerClass();
    CPriceCache::registerClass();
    CPriceCacheItem::registerClass();

    HIDE_FIELD(CAccountWatch, "statement");
    HIDE_FIELD(CAccountWatch, "stateHistory");
    HIDE_FIELD(CAccountWatch, "nodeBal");

    CCachePtrArray caches;

    CAddressCache aid;
    aid.type = aid.getRuntimeClass()->m_ClassName;
    aid.nFiles = 100;
    aid.nFolders = 5;
    aid.path = relativePath(getIndexPath(""), getIndexPath(""), "${INDEX_PATH}");
    caches.push_back(&aid);

    CAbiCache abi;
    abi.type = abi.getRuntimeClass()->m_ClassName;
    abi.nFiles = 100;
    abi.nFolders = 5;
    abi.path = relativePath(getCachePath("abis/"), getCachePath(""), "${CACHE_PATH}");
    caches.push_back(&abi);

    CChainCache cd_blocks;
    cd_blocks.type = cd_blocks.getRuntimeClass()->m_ClassName;
    cd_blocks.nFiles = 100;
    cd_blocks.nFolders = 5;
    cd_blocks.path = relativePath(getCachePath("blocks/"), getCachePath(""), "${CACHE_PATH}");
    caches.push_back(&cd_blocks);

    CChainCache cd_txs;
    cd_txs.type = cd_txs.getRuntimeClass()->m_ClassName;
    cd_txs.nFiles = 100;
    cd_txs.nFolders = 5;
    cd_txs.path = relativePath(getCachePath("txs/"), getCachePath(""), "${CACHE_PATH}");
    caches.push_back(&cd_txs);

    CChainCache cd_traces;
    cd_traces.type = cd_traces.getRuntimeClass()->m_ClassName;
    cd_traces.nFiles = 100;
    cd_traces.nFolders = 5;
    cd_traces.path = relativePath(getCachePath("traces/"), getCachePath(""), "${CACHE_PATH}");;
    caches.push_back(&cd_traces);

    CMonitorCache md;
    md.type = md.getRuntimeClass()->m_ClassName;
    md.nFiles = 100;
    md.nFolders = 5;
    md.path = relativePath(getCachePath("monitors/"), getCachePath(""), "${CACHE_PATH}");;
    caches.push_back(&md);

    CMonitorCacheItem mdi1;
    mdi1.type = mdi1.getRuntimeClass()->m_ClassName;
    mdi1.address = "0x1234567812345678123456781234567812345678";
    mdi1.name = "My Account 1";
    mdi1.firstAppearance = 1001001;
    mdi1.lastAppearance = 8101001;
    mdi1.nRecords = 12;
    mdi1.sizeInBytes = 30010;
    md.monitors.push_back(mdi1);

    CSlurpCache slurp;
    slurp.type = slurp.getRuntimeClass()->m_ClassName;
    slurp.nFiles = 100;
    slurp.nFolders = 5;
    slurp.path = relativePath(getCachePath("slurps/"), getCachePath(""), "${CACHE_PATH}");
    caches.push_back(&slurp);

    CMonitorCacheItem mdi2;
    mdi2.type = mdi2.getRuntimeClass()->m_ClassName;
    mdi2.address = "0x1234567812345678123456781234567812345678";
    mdi2.name = "My Account 1";
    mdi2.firstAppearance = 1001001;
    mdi2.lastAppearance = 8101001;
    mdi2.nRecords = 10102;
    mdi2.sizeInBytes = 1111010;
    slurp.monitors.push_back(mdi2);

    CPriceCache pd;
    pd.type = pd.getRuntimeClass()->m_ClassName;
    pd.nFiles = 100;
    pd.nFolders = 5;
    pd.path = relativePath(getCachePath("prices/"), getCachePath(""), "${CACHE_PATH}");
    caches.push_back(&pd);

    CPriceCacheItem pdi1;
    pdi1.type = pdi1.getRuntimeClass()->m_ClassName;
    pdi1.pair = "ETH-USD";
    pdi1.range = "0010100-8001001";
    pdi1.nRecords = 10000;
    pdi1.sizeInBytes = 1010111;
    pd.pairs.push_back(pdi1);

//    CPriceCacheItem pdi2;
//    pdi2.type = pdi2.getRuntimeClass()->m_ClassName;
//    pdi2.pair = "ETH-DAI";
//    pdi2.range = "0220100-8001001";
//    pdi1.nRecords = 20000;
//    pdi1.sizeInBytes = 2010111;
//    pd.pairs.push_back(pdi2);
//
//    CPriceCacheItem pdi3;
//    pdi3.type = pdi3.getRuntimeClass()->m_ClassName;
//    pdi3.pair = "ETH-DAI";
//    pdi3.range = "0550100-8001001";
//    pdi1.nRecords = 30000;
//    pdi1.sizeInBytes = 3010111;
//    pd.pairs.push_back(pdi3);

    bool first = true;
    ostringstream os;
    os << exportPreamble(JSON1, expContext().fmtMap["header"], GETRUNTIME_CLASS(CCache));
    for (auto cachePtr : caches) {
        if (!first)
            os << ",";
        os << *cachePtr << endl;
        first = false;
    }
    os << exportPostamble(JSON1, expContext().fmtMap["meta"]);

    CStatus status;
    status.client_version = "Parity-Ethereum/""/v2.3.4-beta-0e95db11d-20190220/x86_64-macos/rustc1.32.0";
    status.trueblocks_version = "GHC-TrueBlocks/""/0.5.8-alpha-d0f43c27-20190728";
    status.rpc_provider = "http:/""/localhost:8545";
    status.api_provider = "http:/""/localhost:80";
    status.balance_provider = "http:/""/wildmolasses:80";
    status.cache_location = "/Volumes/Samsung_T7/testing/";
    status.index_location = "~/.quickBlocks/cache/addr_index/";
    status.host = "Thomass-iMac.lan (jrush)";
    status.is_scraping = true;
    status.bytes_on_disk_client = 2511053015;
    status.bytes_on_disk_trueblocks = 1401501501;
    status.caches = "++HOLDER++";


    ostringstream os2;
    os2 << status << endl;
    cout << substitute(os2.str(), "\"++HOLDER++\"", os.str());

#else
    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        blknum_t latest = getLastBlock_cache_final();
        string_q def = (options.asData ? STR_DATA_DISPLAY : STR_DEFAULT_DISPLAY);
        string_q fmtStr = getGlobalConfig("cacheMan")->getDisplayStr(options.asData, def);

        // Handle the various modes (there may be more than one)
        CStringArray modes;
        explode(modes, options.mode, '|');
        for (auto mode: modes) {
            for (size_t ac = 0 ; ac < options.monitors.size() ; ac++) {

                CAccountWatch *watch = &options.monitors[ac];

                options.stats = CStats(); // reset
                CAppearance_base lastItem;
                CAppearanceArray_base fixed;
                fixed.reserve(2000000); // just a guess, but makes adding new records very much faster

extern bool loadMonitorData(CAppearanceArray_base& items, const address_t& addr);
                CAppearanceArray_base items;
                if (loadMonitorData(items, watch->address)) {

                    if (!options.asData)
                        cerr << toProper(mode)+"ing cache: " << watch->name << "\n";
                    if (options.exportFmt == JSON1)
                        cout << "[";

                    for (auto item : items) {

                        if (item.blk < options.maxBlock) {
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
                                    if (!options.trunc || item.blk <= options.trunc) {

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
                                    } else {
                                        cout << (!getEnvStr("API_MODE").empty() ? watch->address + "\t" : "") << item.blk << "\t" << item.txid << endl; // Format(fmtStr);
                                    }
                                }

                            } else {
                                return options.usage("Unknown mode: " + mode + ". Quitting...\n");
                            }
                        }
                    }

                    if (options.exportFmt == JSON1)
                        cout << "]";

                } else {
                    cout << "Could not open file: " << watch->name << "\n";
                }

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
                    if (!isTestMode()) {
                        string_q backFile = watch->name+".bak";
                        copyFile(watch->name, backFile);
                        usleep(1000000); // wait a second, just in case
                        if (!fileExists(backFile)) {
                            cerr << "Could not create backup file `" << backFile << ". Quitting...";
                            return 1;
                        }
                    }
                    cout << "\tRe-writing " << cYellow << fixed.size() << cOff
                            << " of " << options.stats.nRecords << " records to cache: "
                            << cYellow << watch->name << cOff << " (" << options.stats.nTruncs << " truncated)\n";

                    if (!isTestMode()) {
                        remove(watch->name.c_str());
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
                    }
                    cout << cMagenta << "\tThe cache was repaired and a backup created.\n" << cOff;

                } else if (mode == "fix") {
                    if (!options.api_mode)
                        cout << cMagenta << "\tThere was nothing to fix (" << lastItem.blk << ").\n" << cOff;
                    // write the last block to file
                    if (!isTestMode()) {
                        if (lastItem.blk > prevLastBlock || options.stats.nTruncs) {
                            CAccountWatch monitor;
                            monitor.address = watch->address;
                            monitor.writeLastBlock(lastItem.blk);
                        }
                    }
                }
            }
        }
    }

    if (isTestMode())
        ::remove("./merged.bin");
#endif

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
