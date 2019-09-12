/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//--------------------------------------------------------------------------------
void COptions::handle_status(ostream& os) {

    CIndexCache aid;
    if (contains(mode, "|index|")) {
        aid.type = aid.getRuntimeClass()->m_ClassName;
        aid.path = (isTestMode() ? "IndexPath" : getIndexPath(""));
        forEveryFileInFolder(getIndexPath(""), countFiles, &aid);
        status.caches.push_back(&aid);
        CItemCounter counter(this, start);
        loadTimestampArray(&counter.ts_array, counter.ts_cnt);
        counter.cachePtr = &aid;
        counter.indexArray = &aid.items;
        if (details) {
            forEveryFileInFolder(getIndexPath(""), noteIndex, &counter);
        } else {
//            forEveryFileInFolder(getCachePath("monitors/"), noteMonitor_light, &counter);
//            if (aid.metrics.size() == 0)
//                aid.valid_counts = true;
        }
    }

    CMonitorCache md;
    if (contains(mode, "|monitors|")) {
        SHOW_FIELD(CAccountWatch, "nodeBal");
        md.type = md.getRuntimeClass()->m_ClassName;
        md.path = (isTestMode() ? "CachePath" : getCachePath("monitors/"));
        forEveryFileInFolder(getCachePath("monitors/"), countFiles, &md);
        status.caches.push_back(&md);
        CItemCounter counter(this);
        counter.cachePtr = &md;
        counter.monitorArray = &md.items;
        if (details) {
            forEveryFileInFolder(getCachePath("monitors/"), noteMonitor, &counter);
        } else {
            forEveryFileInFolder(getCachePath("monitors/"), noteMonitor_light, &counter);
            if (md.addrs.size() == 0)
                md.valid_counts = true;
        }
    }

    CNameCache nc;
    if (contains(mode, "|names|")) {
        nc.type = nc.getRuntimeClass()->m_ClassName;
        nc.path = (isTestMode() ? "CachePath" : getCachePath("monitors/"));
        forEveryFileInFolder(getCachePath("monitors/"), countFiles, &nc);
        status.caches.push_back(&nc);
        CItemCounter counter(this);
        counter.cachePtr = &nc;
        counter.monitorArray = &nc.items;
        if (details) {
            forEveryFileInFolder(getCachePath("monitors/"), noteMonitor, &counter);
        } else {
            forEveryFileInFolder(getCachePath("monitors/"), noteMonitor_light, &counter);
            if (nc.addrs.size() == 0)
                nc.valid_counts = true;
        }
    }

    CAbiCache abi;
    if (contains(mode, "|abis|")) {
        abi.type = abi.getRuntimeClass()->m_ClassName;
        abi.path = (isTestMode() ? "CachePath" : getCachePath("abis/"));
        forEveryFileInFolder(getCachePath("abis/"), countFiles, &abi);
        status.caches.push_back(&abi);
        if (details) {
            CItemCounter counter(this);
            counter.cachePtr = &abi;
            counter.abiArray = &abi.items;
            forEveryFileInFolder(getCachePath("abis/"), noteABI, &counter);
        }
    }

    CChainCache cd_blocks;
    if (contains(mode, "|blocks|")) {
        cd_blocks.type = cd_blocks.getRuntimeClass()->m_ClassName;
        cd_blocks.path = (isTestMode() ? "BlockPath" : getCachePath("blocks/"));
        forEveryFileInFolder(getCachePath("blocks/"), countFiles, &cd_blocks);
        status.caches.push_back(&cd_blocks);
    }

    CChainCache cd_txs;
    if (contains(mode, "|txs|")) {
        cd_txs.type = cd_txs.getRuntimeClass()->m_ClassName;
        cd_txs.path = (isTestMode() ? "TxPath" : getCachePath("txs/"));
        forEveryFileInFolder(getCachePath("txs/"), countFiles, &cd_txs);
        status.caches.push_back(&cd_txs);
    }

    CChainCache cd_traces;
    if (contains(mode, "|traces|")) {
        cd_traces.type = cd_traces.getRuntimeClass()->m_ClassName;
        cd_traces.path = (isTestMode() ? "TracePath" : getCachePath("traces/"));
        forEveryFileInFolder(getCachePath("traces/"), countFiles, &cd_traces);
        status.caches.push_back(&cd_traces);
    }

    CSlurpCache slurps;
    if (contains(mode, "|slurps|")) {
        slurps.type = slurps.getRuntimeClass()->m_ClassName;
        slurps.path = (isTestMode() ? "SlurpPath" : getCachePath("slurps/"));
        forEveryFileInFolder(getCachePath("slurps/"), countFiles, &slurps);
        status.caches.push_back(&slurps);
        CItemCounter counter(this);
        counter.cachePtr = &slurps;
        counter.monitorArray = &slurps.items;
        if (details) {
            forEveryFileInFolder(getCachePath("slurps/"), noteMonitor, &counter);
        } else {
            forEveryFileInFolder(getCachePath("slurps/"), noteMonitor_light, &counter);
            if (slurps.addrs.size() == 0)
                slurps.valid_counts = true;
        }
    }

    CPriceCache pd;
    if (contains(mode, "|prices|")) {
        pd.type = pd.getRuntimeClass()->m_ClassName;
        pd.path = (isTestMode() ? "PricePath" : getCachePath("prices/"));
        forEveryFileInFolder(getCachePath("prices/"), countFiles, &pd);
        status.caches.push_back(&pd);
        if (details) {
            CItemCounter counter(this);
            counter.cachePtr = &pd;
            counter.priceArray = &pd.items;
            forEveryFileInFolder(getCachePath("prices/"), notePrice, &counter);
        }
    }

    os << exportPreamble(JSON1, expContext().fmtMap["header"], GETRUNTIME_CLASS(CStatus));
    os << status << endl;
    os << exportPostamble(JSON1, expContext().fmtMap["meta"]);

    return;
}

//---------------------------------------------------------------------------
bool countFiles(const string_q& path, void *data) {

    if (isTestMode())
        return false;

    CCache *counter = (CCache *)data;
    if (endsWith(path, '/')) {
        if (!contains(path, "monitors/staging"))
            counter->noteFolder(path);
        return forEveryFileInFolder(path + "*", countFiles, data);

    } else if (endsWith(path, ".bin") || endsWith(path, ".json")) {

        counter->noteFile(path);
        counter->valid_counts = true;

    }
    return !shouldQuit();
}

//---------------------------------------------------------------------------
bool noteMonitor_light(const string_q& path, void *data) {

    if (isTestMode())
        return false;

    if (contains(path, "/staging"))
        return !shouldQuit();

    if (endsWith(path, '/')) {
        return forEveryFileInFolder(path + "*", noteMonitor_light, data);

    } else if (endsWith(path, "acct.bin") || endsWith(path, ".json")) {
        CItemCounter *counter = (CItemCounter*)data;
        ASSERT(counter->options);
        ((CMonitorCache*)counter->cachePtr)->addrs.push_back(substitute(substitute(substitute(substitute(path, counter->cachePtr->path, ""),".acct", ""),".bin", ""), ".json", ""));
    }
    return !shouldQuit();
}

//---------------------------------------------------------------------------
bool noteMonitor(const string_q& path, void *data) {

    if (isTestMode())
        return false;

    if (contains(path, "/staging"))
        return !shouldQuit();

    if (endsWith(path, '/')) {
        return forEveryFileInFolder(path + "*", noteMonitor, data);

    } else if (endsWith(path, "acct.bin") || endsWith(path, ".json")) {
        CItemCounter *counter = (CItemCounter*)data;
        ASSERT(counter->options);
        CMonitorCacheItem mdi;
        mdi.type = mdi.getRuntimeClass()->m_ClassName;
        mdi.address = substitute(substitute(substitute(substitute(path, counter->cachePtr->path, ""),".acct", ""),".bin", ""), ".json", "");
        mdi.nodeBal = getBalanceAt(mdi.address); // doesn't need the balance provider since it latest block
        CAccountName item;
        string_q customStr = getGlobalConfig("getAccounts")->getConfigJson("custom", "list", "");
        while (item.parseJson3(customStr)) {
            unpreserveSpaces(item.name);
            if (mdi.address == item.address) {
                mdi.group = item.group;
                mdi.name = item.name;
                break;
            }
            item = CAccountName();
        }
        if (mdi.name.empty()) {
            ASSERT(prefunds.size() == 8893);  // This is a known value
            uint32_t cnt = 0;
            for (auto prefund : counter->options->prefundWeiMap) {
                address_t addr = prefund.first;
                if (mdi.address == addr) {
                    mdi.group = "80-Prefund";
                    mdi.name = "Prefund_" + padNum4(cnt);
                    mdi.source = "Genesis";
                    break;
                }
                cnt++;
            }
        }

        if (mdi.name.empty())
            counter->options->getNamedAccount(mdi, mdi.address);

        if (endsWith(path, ".acct.bin")) {
            mdi.firstAppearance = 1001001;
            mdi.latestAppearance = 8101001;
            mdi.nRecords = fileSize(path) / sizeof(CAppearance_base);
            mdi.sizeInBytes = fileSize(path);
        } else {
            mdi.firstAppearance = NOPOS;
            mdi.latestAppearance = NOPOS;
            mdi.nRecords = NOPOS;
            mdi.sizeInBytes = NOPOS;
        }
        counter->monitorArray->push_back(mdi);
    }
    return !shouldQuit();
}

//---------------------------------------------------------------------------
bool noteIndex(const string_q& path, void *data) {

    if (isTestMode())
        return false;

    if (endsWith(path, '/')) {
        return forEveryFileInFolder(path + "*", noteIndex, data);

    } else {
        CItemCounter *counter = (CItemCounter*)data;
        timestamp_t unused;
        blknum_t ll = 0;
        blknum_t ff = bnFromPath(path, ll, unused);
        if (ff < counter->skipTo)
            return true;
        cerr << path << "\r"; cerr.flush();
        ASSERT(counter->options);
        CIndexCacheItem aci;
        aci.type = aci.getRuntimeClass()->m_ClassName;
        aci.path = substitute(path, counter->cachePtr->path, "");
        string_q fn = aci.path;
        replace(fn, "blooms/", "");
        replace(fn, "finalized/", "");
        replace(fn, "staging/", "");
        replace(fn, "unripe/", "");
        replace(fn, "ripe/", "");
        if (contains(path, "finalized") || contains(path, "blooms")) {
            uint64_t tmp;
            aci.firstAppearance = (uint32_t)bnFromPath(fn, tmp, unused);
            aci.latestAppearance = (uint32_t)tmp;
        } else {
            aci.firstAppearance = (uint32_t)str_2_Uint(fn);
            aci.latestAppearance = (uint32_t)str_2_Uint(fn);
        }
        if (counter->ts_array) {
            if (aci.firstAppearance < counter->ts_cnt &&
                aci.latestAppearance < counter->ts_cnt) {
                aci.firstTs = (timestamp_t)counter->ts_array[(aci.firstAppearance*2)+1];
                aci.lastestTs = (timestamp_t)counter->ts_array[(aci.latestAppearance*2)+1];
            } else {
                aci.firstTs = (timestamp_t)0;
                aci.lastestTs = (timestamp_t)0;
            }
        }
        getIndexMetrics(path, aci.nAppearances, aci.nAddresses);
        aci.sizeInBytes = (uint32_t)fileSize(path);
        //aci.metrics.push_back(aci.firstAppearance);
        //a/ci.metrics.push_back(aci.latestAppearance);
        //aci.metrics.push_back(aci.nAppearances);
        //aci.metrics.push_back(aci.nAddresses);
        //aci.metrics.push_back(aci.sizeInBytes);
        counter->indexArray->push_back(aci);
    }
    return !shouldQuit();
}

//---------------------------------------------------------------------------
bool noteABI(const string_q& path, void *data) {

    if (isTestMode())
        return false;

    if (contains(path, "/staging"))
        return !shouldQuit();

    if (endsWith(path, '/')) {
        return forEveryFileInFolder(path + "*", noteABI, data);

    } else if (endsWith(path, ".bin") || endsWith(path, ".json")) {

        CItemCounter *counter = (CItemCounter*)data;
        ASSERT(counter->options);

        CAbiCacheItem abii;
        abii.type = abii.getRuntimeClass()->m_ClassName;
        abii.address = substitute(substitute(substitute(substitute(path, counter->cachePtr->path, ""),".acct", ""),".bin", ""), ".json", "");
        CAccountName n;
        counter->options->getNamedAccount(n, abii.address);
        abii.name = n.name;
        CAbi abi;
        abi.loadAbiFromFile(path, false);
        abii.nFunctions = abi.nFunctions();
        abii.nEvents = abi.nEvents();
        abii.nOther = abi.nOther();
        abii.sizeInBytes = fileSize(path);
        counter->abiArray->push_back(abii);

    }
    return !shouldQuit();
}

//---------------------------------------------------------------------------
bool notePrice(const string_q& path, void *data) {

    if (isTestMode())
        return false;

    if (endsWith(path, '/')) {
        return forEveryFileInFolder(path + "*", notePrice, data);

    } else if (endsWith(path, ".bin")) {

        CItemCounter *counter = (CItemCounter*)data;
        ASSERT(counter->options);

        CPriceCacheItem price;
        price.type = price.getRuntimeClass()->m_ClassName;
        price.pair = substitute(substitute(substitute(substitute(path, counter->cachePtr->path, ""),".acct", ""),".bin", ""), ".json", "");
        price.sizeInBytes = fileSize(path);
        price.nRecords = fileSize(path) / sizeof(CPriceQuote);
        counter->priceArray->push_back(price);
    }
    return !shouldQuit();
}

//---------------------------------------------------------------------------
void getIndexMetrics(const string_q& path, uint32_t& nAppearences, uint32_t& nAddresses) {

    nAppearences = nAddresses = (uint32_t)-1;
    if (contains(path, "blooms")) {
        return;
    }

    if (endsWith(path, ".txt")) {
        nAppearences = (uint32_t)fileSize(path) / (uint32_t)59;
        CStringArray lines;
        asciiFileToLines(path, lines);
        typedef map<address_t, bool> CAddressMap;
        CAddressMap addrMap;
        for (auto line : lines)
            addrMap[nextTokenClear(line,'\t')] = true;
        nAddresses = (uint32_t)addrMap.size();
        return;
    }

    CArchive archive(READING_ARCHIVE);
    if (!archive.Lock(path, modeReadOnly, LOCK_NOWAIT))
        return;

    CHeaderRecord_base header;
    bzero(&header, sizeof(header));
    //size_t nRead =
    archive.Read(&header, sizeof(header), 1);
    //if (false) { //nRead != sizeof(header)) {
    //    cerr << "Could not read file: " << path << endl;
    //    return;
    //}
    ASSERT(header.magic == MAGIC_NUMBER);
    //ASSERT(bytes_2_Hash(h->hash) == versionHash);
    nAddresses = header.nAddrs;
    nAppearences = header.nRows;
}
