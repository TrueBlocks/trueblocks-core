/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//--------------------------------------------------------------------------------
bool COptions::handle_status(ostream& os) {
    ENTER8("handle_status");

    CIndexCache aid;
    if (contains(mode, "|index|")) {
        LOG8("Index");
        aid.type = aid.getRuntimeClass()->m_ClassName;
        aid.path = (isTestMode() ? "IndexPath" : getIndexPath(""));
        forEveryFileInFolder(getIndexPath(""), countFiles, &aid);
        status.caches.push_back(&aid);
        CItemCounter counter(this, start, end);
        loadTimestampArray(&counter.ts_array, counter.ts_cnt);
        counter.cachePtr = &aid;
        counter.indexArray = &aid.items;
        if (details) {
            forEveryFileInFolder(getIndexPath(""), noteIndex, &counter);
        } else {
            //            forEveryFileInFolder(getCachePath("monitors/"), noteIndex_light, &counter);
            //            if (aid.metrics.size() == 0)
            //                aid.valid_counts = true;
        }
    }

    CMonitorCache md;
    if (contains(mode, "|monitors|")) {
        LOG8("Monitors");
        if (expContext().asEther) {
            SHOW_FIELD(CAccountWatch, "curEther");
            HIDE_FIELD(CAccountWatch, "curBalance");
        } else {
            HIDE_FIELD(CAccountWatch, "curEther");
            SHOW_FIELD(CAccountWatch, "curBalance");
        }
        md.type = md.getRuntimeClass()->m_ClassName;
        md.path = (isTestMode() ? "CachePath" : getCachePath("monitors/"));
        forEveryFileInFolder(getCachePath("monitors/"), countFiles, &md);
        status.caches.push_back(&md);
        CItemCounter counter(this, start, end);
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
        LOG8("Names");
        nc.type = nc.getRuntimeClass()->m_ClassName;
        nc.path = (isTestMode() ? "CachePath" : getCachePath("monitors/"));
        forEveryFileInFolder(getCachePath("monitors/"), countFiles, &nc);
        status.caches.push_back(&nc);
        CItemCounter counter(this, start, end);
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
        LOG8("abis");
        abi.type = abi.getRuntimeClass()->m_ClassName;
        abi.path = (isTestMode() ? "CachePath" : getCachePath("abis/"));
        forEveryFileInFolder(getCachePath("abis/"), countFiles, &abi);
        status.caches.push_back(&abi);
        if (details) {
            CItemCounter counter(this, start, end);
            counter.cachePtr = &abi;
            counter.abiArray = &abi.items;
            forEveryFileInFolder(getCachePath("abis/"), noteABI, &counter);
        }
    }

    CChainCache cd_blocks;
    if (contains(mode, "|blocks|") || contains(mode, "|data|")) {
        LOG8("Blocks");
        cd_blocks.type = cd_blocks.getRuntimeClass()->m_ClassName;
        cd_blocks.path = (isTestMode() ? "BlockPath" : getCachePath("blocks/"));
        forEveryFileInFolder(getCachePath("blocks/"), countFiles, &cd_blocks);
        status.caches.push_back(&cd_blocks);
    }

    CChainCache cd_txs;
    if (contains(mode, "|transactions|") || contains(mode, "|data|")) {
        LOG8("Transactions");
        cd_txs.type = cd_txs.getRuntimeClass()->m_ClassName;
        cd_txs.path = (isTestMode() ? "TxPath" : getCachePath("txs/"));
        forEveryFileInFolder(getCachePath("txs/"), countFiles, &cd_txs);
        status.caches.push_back(&cd_txs);
    }

    CChainCache cd_traces;
    if (contains(mode, "|traces|") || contains(mode, "|data|")) {
        LOG8("Traces");
        cd_traces.type = cd_traces.getRuntimeClass()->m_ClassName;
        cd_traces.path = (isTestMode() ? "TracePath" : getCachePath("traces/"));
        forEveryFileInFolder(getCachePath("traces/"), countFiles, &cd_traces);
        status.caches.push_back(&cd_traces);
    }

    CSlurpCache slurps;
    if (contains(mode, "|slurps|")) {
        LOG8("Slurps");
        slurps.type = slurps.getRuntimeClass()->m_ClassName;
        slurps.path = (isTestMode() ? "SlurpPath" : getCachePath("slurps/"));
        forEveryFileInFolder(getCachePath("slurps/"), countFiles, &slurps);
        status.caches.push_back(&slurps);
        CItemCounter counter(this, start, end);
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
        LOG8("Prices");
        pd.type = pd.getRuntimeClass()->m_ClassName;
        pd.path = (isTestMode() ? "PricePath" : getCachePath("prices/"));
        forEveryFileInFolder(getCachePath("prices/"), countFiles, &pd);
        status.caches.push_back(&pd);
        if (details) {
            CItemCounter counter(this, start, end);
            counter.cachePtr = &pd;
            counter.priceArray = &pd.items;
            forEveryFileInFolder(getCachePath("prices/"), notePrice, &counter);
        }
    }

    os << status;
    EXIT_NOMSG8(true);
}

//---------------------------------------------------------------------------
bool countFiles(const string_q& path, void* data) {
    CCache* counter = reinterpret_cast<CCache*>(data);
    if (endsWith(path, '/')) {
        if (!isTestMode() && !contains(path, "monitors/staging"))
            counter->noteFolder(path);
        return forEveryFileInFolder(path + "*", countFiles, data);

    } else if (endsWith(path, ".bin") || endsWith(path, ".json")) {
        if (!isTestMode())
            counter->noteFile(path);
        counter->valid_counts = true;
    }
    return !shouldQuit();
}

//---------------------------------------------------------------------------
bool noteMonitor_light(const string_q& path, void* data) {
    if (contains(path, "/staging"))
        return !shouldQuit();

    if (endsWith(path, '/')) {
        return forEveryFileInFolder(path + "*", noteMonitor_light, data);

    } else if (endsWith(path, "acct.bin") || endsWith(path, ".json")) {
        CItemCounter* counter = reinterpret_cast<CItemCounter*>(data);
        ASSERT(counter->options);
        CMonitorCache* ptr = (CMonitorCache*)counter->cachePtr;  // NOLINT
        if (isTestMode()) {
            if (ptr->addrs.size() < 3)
                ptr->addrs.push_back("--address--");
        } else {
            ptr->addrs.push_back(substitute(
                substitute(substitute(substitute(path, counter->cachePtr->path, ""), ".acct", ""), ".bin", ""), ".json",
                ""));
        }
    }
    return !shouldQuit();
}

//---------------------------------------------------------------------------
bool noteMonitor(const string_q& path, void* data) {
    if (contains(path, "/staging"))
        return !shouldQuit();

    if (endsWith(path, '/')) {
        return forEveryFileInFolder(path + "*", noteMonitor, data);

    } else if (endsWith(path, "acct.bin") || endsWith(path, ".json")) {
        LOG4("Processing: ", path);

        CItemCounter* counter = reinterpret_cast<CItemCounter*>(data);
        ASSERT(counter->options);
        CMonitorCacheItem mdi;
        mdi.type = mdi.getRuntimeClass()->m_ClassName;
        mdi.address =
            substitute(substitute(substitute(substitute(path, counter->cachePtr->path, ""), ".acct", ""), ".bin", ""),
                       ".json", "");
        if (isTestMode()) {
            mdi.address = "---address---";
            mdi.curBalance = 10000;
        } else {
            mdi.curBalance = (isNodeRunning() ? getBalanceAt(mdi.address) : str_2_BigUint(uint_2_Str(NOPOS)));
        }
        CAccountName item;
        string_q customStr = getGlobalConfig("ethNames")->getConfigJson("custom", "list", "");
        while (item.parseJson3(customStr)) {
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

        if (!isTestMode() && endsWith(path, ".acct.bin")) {
            CArchive archive(READING_ARCHIVE);
            if (archive.Lock(path, modeReadOnly, LOCK_NOWAIT)) {
                uint32_t first = 0,
                         last = 0;  // the data on file is stored as uint32_t. Make sure to read the right thing
                archive.Seek(0, SEEK_SET);
                archive.Read(first);
                archive.Seek(-1 * (long)(2 * sizeof(uint32_t)), SEEK_END);  // NOLINT
                archive.Read(last);
                archive.Release();
                mdi.firstAppearance = first;
                mdi.latestAppearance = last;
            } else {
                mdi.firstAppearance = NOPOS;
                mdi.latestAppearance = NOPOS;
            }
            mdi.nRecords = fileSize(path) / sizeof(CAppearance_base);
            mdi.sizeInBytes = fileSize(path);
        } else {
            mdi.firstAppearance = NOPOS;
            mdi.latestAppearance = NOPOS;
            mdi.nRecords = NOPOS;
            mdi.sizeInBytes = NOPOS;
        }
        counter->monitorArray->push_back(mdi);
        if (isTestMode())
            return false;
    }
    return !shouldQuit();
}

//---------------------------------------------------------------------------
bool noteIndex(const string_q& path, void* data) {
    if (endsWith(path, '/')) {
        return forEveryFileInFolder(path + "*", noteIndex, data);

    } else {
        if (!isTestMode()) {
            cerr << path << "\r";
            cerr.flush();
        }

        CItemCounter* counter = reinterpret_cast<CItemCounter*>(data);

        timestamp_t unused;
        blknum_t last = NOPOS;
        blknum_t first = bnFromPath(path, last, unused);
        if (last < counter->scanRange.first)
            return true;
        if (first > counter->scanRange.second)
            return !contains(path, "finalized");

        ASSERT(counter->options);
        CIndexCacheItem aci;
        aci.type = aci.getRuntimeClass()->m_ClassName;
        //        aci.path = substitute(path, getIndexPath(""), "${INDEX}/");
        //        string_q fn = substitute(path, getIndexPath(""), "");
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
            CStringArray parts;
            explode(parts, path, '/');
            blknum_t num = str_2_Uint(nextTokenClear(parts[parts.size() - 1], '-'));
            if (contains(path, "blooms")) {
                aci.hash = counter->options->bloomHashes[num].hash;
            } else {
                ASSERT(contains(path, "finalized"));
                aci.hash = counter->options->indexHashes[num].hash;
            }
            size_t len = aci.hash.length();
            if (len)
                aci.hash = aci.hash.substr(0, 4) + "..." + aci.hash.substr(len - 4, len);
        } else {
            aci.firstAppearance = (uint32_t)str_2_Uint(fn);
            aci.latestAppearance = (uint32_t)str_2_Uint(fn);
        }
        if (counter->ts_array) {
            if (aci.firstAppearance < counter->ts_cnt && aci.latestAppearance < counter->ts_cnt) {
                aci.firstTs = (timestamp_t)counter->ts_array[(aci.firstAppearance * 2) + 1];
                aci.lastestTs = (timestamp_t)counter->ts_array[(aci.latestAppearance * 2) + 1];
            } else {
                aci.firstTs = (timestamp_t)0;
                aci.lastestTs = (timestamp_t)0;
            }
        }
        getIndexMetrics(path, aci.nAppearances, aci.nAddresses);
        aci.sizeInBytes = (uint32_t)fileSize(path);
        // aci.metrics.push_back(aci.firstAppearance);
        // a/ci.metrics.push_back(aci.latestAppearance);
        // aci.metrics.push_back(aci.nAppearances);
        // aci.metrics.push_back(aci.nAddresses);
        // aci.metrics.push_back(aci.sizeInBytes);
        counter->indexArray->push_back(aci);
    }
    return !shouldQuit();
}

//---------------------------------------------------------------------------
bool noteABI(const string_q& path, void* data) {
    if (contains(path, "/staging"))
        return !shouldQuit();

    if (endsWith(path, '/')) {
        return forEveryFileInFolder(path + "*", noteABI, data);

    } else if (endsWith(path, ".bin") || endsWith(path, ".json")) {
        CItemCounter* counter = reinterpret_cast<CItemCounter*>(data);
        ASSERT(counter->options);

        CAbiCacheItem abii;
        abii.type = abii.getRuntimeClass()->m_ClassName;
        abii.address =
            substitute(substitute(substitute(substitute(path, counter->cachePtr->path, ""), ".acct", ""), ".bin", ""),
                       ".json", "");
        if (isTestMode())
            abii.address = "---address---";
        CAccountName n;
        counter->options->getNamedAccount(n, abii.address);
        abii.name = n.name;
        if (isTestMode()) {
            abii.nFunctions = abii.nEvents = abii.nOther = abii.sizeInBytes = 36963;
        } else {
            CAbi abi;
            abi.loadAbiFromFile(path, false);
            sort(abi.interfaces.begin(), abi.interfaces.end());
            abii.nFunctions = abi.nFunctions();
            abii.nEvents = abi.nEvents();
            abii.nOther = abi.nOther();
            abii.sizeInBytes = fileSize(path);
        }
        counter->abiArray->push_back(abii);
        if (isTestMode())
            return false;
    }
    return !shouldQuit();
}

//---------------------------------------------------------------------------
bool notePrice(const string_q& path, void* data) {
    if (endsWith(path, '/')) {
        return forEveryFileInFolder(path + "*", notePrice, data);

    } else if (endsWith(path, ".bin")) {
        CItemCounter* counter = reinterpret_cast<CItemCounter*>(data);
        ASSERT(counter->options);

        CPriceCacheItem price;
        price.type = price.getRuntimeClass()->m_ClassName;
        if (isTestMode()) {
            price.pair = "---pair---";
            price.sizeInBytes = price.nRecords = 36910963;
        } else {
            price.pair = substitute(
                substitute(substitute(substitute(path, counter->cachePtr->path, ""), ".acct", ""), ".bin", ""), ".json",
                "");
            price.sizeInBytes = fileSize(path);
            price.nRecords = fileSize(path) / sizeof(CPriceQuote);
        }
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
            addrMap[nextTokenClear(line, '\t')] = true;
        nAddresses = (uint32_t)addrMap.size();
        return;
    }

    CArchive archive(READING_ARCHIVE);
    if (!archive.Lock(path, modeReadOnly, LOCK_NOWAIT))
        return;

    CHeaderRecord_base header;
    bzero(&header, sizeof(header));
    // size_t nRead =
    archive.Read(&header, sizeof(header), 1);
    // if (false) { //nRead != sizeof(header)) {
    //    cerr << "Could not read file: " << path << endl;
    //    return;
    //}
    ASSERT(header.magic == MAGIC_NUMBER);
    // ASSERT(bytes_2_Hash(h->hash) == versionHash);
    nAddresses = header.nAddrs;
    nAppearences = header.nRows;
}
