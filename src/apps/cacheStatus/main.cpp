/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-------------------------------------------------------------------------
int main(int argc, const char *argv[]) {
    acctlib_init(defaultQuitHandler);

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

    acctlib_cleanup();

    return 0;
}
