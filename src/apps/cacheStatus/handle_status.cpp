/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

inline string_q pathName(const string_q& str, const string_q& path = "") {
    return (isTestMode() ? str + "Path" : (path.empty() ? getCachePath(str + "/") : path));
}
//--------------------------------------------------------------------------------
bool COptions::handle_status(ostream& os) {
    ENTER("handle_status");

    CIndexCache index;
    if (contains(mode, "|index|")) {
        LOG8("Reporting on index");
        if (!index.readBinaryCache("index", details)) {
            LOG8("Regenerating cache");
            index.type = index.getRuntimeClass()->m_ClassName;
            expContext().types[index.type] = index.getRuntimeClass();
            index.path = pathName("index", getIndexPath(""));
            forEveryFileInFolder(getIndexPath(""), countFiles, &index);
            LOG8("Counted files");
            CItemCounter counter(this, start, end);
            loadTimestampFile(&counter.ts_array, counter.ts_cnt);
            index.path = pathName("index", indexFolder_finalized);
            counter.cachePtr = &index;
            counter.indexArray = &index.items;
            LOG8("Loaded timestamps");
            if (details) {
                counter.fileRange.second = index.nFiles;
                forEveryFileInFolder(indexFolder_finalized, noteIndex, &counter);
            }
            LOG8("Visited folders");
            index.writeBinaryCache("index", details);
            LOG8("Wrote cache");
        } else {
            LOG8("Read from cache");
        }
        status.caches.push_back(&index);
    }

    LOG4("Processing monitors");
    CMonitorCache monitors;
    if (contains(mode, "|monitors|")) {
        LOG8("Reporting on monitors");
        if (!monitors.readBinaryCache("monitors", details)) {
            monitors.type = monitors.getRuntimeClass()->m_ClassName;
            expContext().types[monitors.type] = monitors.getRuntimeClass();
            monitors.path = pathName("monitors");
            LOG4("counting monitors");
            forEveryFileInFolder(getCachePath("monitors/"), countFiles, &monitors);
            LOG4("done counting monitors");
            CItemCounter counter(this, start, end);
            counter.cachePtr = &monitors;
            counter.monitorArray = &monitors.items;
            LOG4("forEvery monitors");
            if (details) {
                forEveryFileInFolder(getCachePath("monitors/"), noteMonitor, &counter);
            } else {
                forEveryFileInFolder(getCachePath("monitors/"), noteMonitor_light, &counter);
                if (monitors.addrs.size() == 0)
                    monitors.is_valid = true;
            }
            LOG4("forEvery monitors done");
            LOG8("\tWriting monitors cache");
            //            monitors.writeBinaryCache("monitors", details);
            LOG4("done writing cache");
        }
        status.caches.push_back(&monitors);
    }

    CNameCache names;
    if (contains(mode, "|names|")) {
        LOG8("Reporting on names");
        if (!names.readBinaryCache("names", details)) {
            names.type = names.getRuntimeClass()->m_ClassName;
            expContext().types[names.type] = names.getRuntimeClass();
            names.path = pathName("names");
            forEveryFileInFolder(getCachePath("monitors/"), countFiles, &names);
            CItemCounter counter(this, start, end);
            counter.cachePtr = &names;
            counter.monitorArray = &names.items;
            if (details) {
                forEveryFileInFolder(getCachePath("monitors/"), noteMonitor, &counter);
            } else {
                forEveryFileInFolder(getCachePath("monitors/"), noteMonitor_light, &counter);
                if (names.addrs.size() == 0)
                    names.is_valid = true;
            }
            LOG8("\tre-writing names cache");
            names.writeBinaryCache("names", details);
        }
        status.caches.push_back(&names);
    }

    CAbiCache abis;
    if (contains(mode, "|abis|")) {
        LOG8("Reporting on abis");
        if (!abis.readBinaryCache("abis", details)) {
            abis.type = abis.getRuntimeClass()->m_ClassName;
            expContext().types[abis.type] = abis.getRuntimeClass();
            abis.path = pathName("abis");
            forEveryFileInFolder(getCachePath("abis/"), countFiles, &abis);
            if (details) {
                CItemCounter counter(this, start, end);
                counter.cachePtr = &abis;
                counter.abiArray = &abis.items;
                forEveryFileInFolder(getCachePath("abis/"), noteABI, &counter);
            }
            LOG8("\tre-writing abis cache");
            abis.writeBinaryCache("abis", details);
        }
        status.caches.push_back(&abis);
    }

    CChainCache blocks;
    if (contains(mode, "|blocks|") || contains(mode, "|data|")) {
        LOG8("Reporting on blocks");
        if (!blocks.readBinaryCache("blocks", details)) {
            blocks.type = blocks.getRuntimeClass()->m_ClassName;
            expContext().types[blocks.type] = blocks.getRuntimeClass();
            blocks.path = pathName("blocks");
            blocks.max_depth = countOf(getCachePath("blocks/"), '/') + depth;
            forEveryFileInFolder(getCachePath("blocks/"), countFilesInCache, &blocks);
            blocks.max_depth = depth;
            LOG8("\tre-writing blocks cache");
            blocks.writeBinaryCache("blocks", details);
        }
        status.caches.push_back(&blocks);
    }

    CChainCache txs;
    if (contains(mode, "|transactions|") || contains(mode, "|data|")) {
        LOG8("Reporting on txs");
        if (!txs.readBinaryCache("txs", details)) {
            txs.type = txs.getRuntimeClass()->m_ClassName;
            expContext().types[txs.type] = txs.getRuntimeClass();
            txs.path = pathName("txs");
            txs.max_depth = countOf(getCachePath("transactions/"), '/') + depth;
            forEveryFileInFolder(getCachePath("txs/"), countFilesInCache, &txs);
            txs.max_depth = depth;
            LOG8("\tre-writing txs cache");
            txs.writeBinaryCache("txs", details);
        }
        status.caches.push_back(&txs);
    }

    CChainCache traces;
    if (contains(mode, "|traces|") || contains(mode, "|data|")) {
        LOG8("Reporting on traces");
        if (!traces.readBinaryCache("traces", details)) {
            traces.type = traces.getRuntimeClass()->m_ClassName;
            expContext().types[traces.type] = traces.getRuntimeClass();
            traces.path = pathName("traces");
            traces.max_depth = countOf(getCachePath("traces/"), '/') + depth;
            forEveryFileInFolder(getCachePath("traces/"), countFilesInCache, &traces);
            traces.max_depth = depth;
            LOG8("\tre-writing traces cache");
            traces.writeBinaryCache("traces", details);
        }
        status.caches.push_back(&traces);
    }

    CSlurpCache slurps;
    if (contains(mode, "|slurps|")) {
        LOG8("Reporting on slurps");
        if (!slurps.readBinaryCache("slurps", details)) {
            slurps.type = slurps.getRuntimeClass()->m_ClassName;
            expContext().types[slurps.type] = slurps.getRuntimeClass();
            slurps.path = pathName("slurps");
            forEveryFileInFolder(getCachePath("slurps/"), countFiles, &slurps);
            CItemCounter counter(this, start, end);
            counter.cachePtr = &slurps;
            counter.monitorArray = &slurps.items;
            if (details) {
                forEveryFileInFolder(getCachePath("slurps/"), noteMonitor, &counter);
            } else {
                HIDE_FIELD(CSlurpCache, "addrs");
                forEveryFileInFolder(getCachePath("slurps/"), noteMonitor_light, &counter);
                if (slurps.addrs.size() == 0)
                    slurps.is_valid = true;
            }
            LOG8("\tre-writing slurps cache");
            slurps.writeBinaryCache("slurps", details);
        }
        status.caches.push_back(&slurps);
    }

    CPriceCache prices;
    if (contains(mode, "|prices|")) {
        LOG8("Reporting on prices");
        if (!prices.readBinaryCache("prices", details)) {
            prices.type = prices.getRuntimeClass()->m_ClassName;
            expContext().types[prices.type] = prices.getRuntimeClass();
            prices.path = pathName("prices");
            forEveryFileInFolder(getCachePath("prices/"), countFiles, &prices);
            if (details) {
                CItemCounter counter(this, start, end);
                counter.cachePtr = &prices;
                counter.priceArray = &prices.items;
                forEveryFileInFolder(getCachePath("prices/"), notePrice, &counter);
            }
            LOG8("\tre-writing prices cache");
            prices.writeBinaryCache("prices", details);
        }
        status.caches.push_back(&prices);
    }

    status.toJson(os);

    EXIT_NOMSG(true);
}

//---------------------------------------------------------------------------
bool countFilesInCache(const string_q& path, void* data) {
    CChainCache* counter = reinterpret_cast<CChainCache*>(data);
    if (endsWith(path, '/')) {
        if (contains(path, "/0")) {
            uint64_t d = countOf(path, '/') - 1;
            uint64_t m = counter->max_depth;
            if (d == m) {  // TODO(tjayrush) fails after 100,000,000 blocks!
                if (isTestMode()) {
                    counter->items.push_back("CachePath/00/00/00");
                    counter->items.push_back("CachePath/00/01/00");
                    return false;
                } else {
                    counter->items.push_back(substitute(path, counter->path, ""));
                }
            }
            if (!isTestMode()) {
                counter->noteFolder(path);
            }
        }
        return forEveryFileInFolder(path + "*", countFilesInCache, data);

    } else {
        if (!isTestMode())
            counter->noteFile(path);
        counter->is_valid = true;
        if (isTestMode()) {
            counter->items.push_back("CachePath/00/00/00/file1.bin");
            counter->items.push_back("CachePath/00/01/00/file2.bin");
            return false;

        } else if (counter->max_depth == countOf(path, '/')) {
            counter->items.push_back(substitute(path, counter->path, ""));
        }
    }
    return !shouldQuit();
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
        counter->is_valid = true;
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
        string_q addr = substitute(path, "/0x", "|");
        nextTokenClear(addr, '|');
        if (isTestMode()) {
            if (ptr->addrs.size() < 2)
                ptr->addrs.push_back("---address---");
        } else {
            ptr->addrs.push_back("0x" + nextTokenClear(addr, '.'));
        }
    }
    return !shouldQuit();
}

//---------------------------------------------------------------------------
bool noteCollection(const string_q& path, void* data) {
    if (endsWith(path, '/')) {
        return forEveryFileInFolder(path + "*", noteCollection, data);

    } else if (endsWith(path, ".bin")) {
        CItemCounter* counter = reinterpret_cast<CItemCounter*>(data);
        ASSERT(counter->options);

        CCollectionCacheItem coi;
        coi.type = coi.getRuntimeClass()->m_ClassName;
        expContext().types[coi.type] = coi.getRuntimeClass();
        if (isTestMode()) {
            coi.name = "---name---";
            coi.sizeInBytes = 1010202;
            coi.nAppearances = 2020101;
        } else {
            coi.name = path;
            coi.sizeInBytes = fileSize(path);
            coi.nAppearances = fileSize(path) / sizeof(CPriceQuote);
        }
        counter->collectionArray->push_back(coi);
    }
    return !shouldQuit();
}

//---------------------------------------------------------------------------
bool noteMonitor(const string_q& path, void* data) {
    if (contains(path, "/staging"))
        return !shouldQuit();

    if (endsWith(path, '/')) {
        return forEveryFileInFolder(path + "*", noteMonitor, data);

    } else if (endsWith(path, "acct.bin")) {  // || endsWith(path, ".json")) {
        CItemCounter* counter = reinterpret_cast<CItemCounter*>(data);
        ASSERT(counter->options);
        CMonitorCacheItem mdi;
        mdi.type = mdi.getRuntimeClass()->m_ClassName;
        expContext().types[mdi.type] = mdi.getRuntimeClass();
        string_q addr = substitute(path, "/0x", "|");
        nextTokenClear(addr, '|');
        mdi.address = "0x" + nextTokenClear(addr, '.');
        counter->options->getNamedAccount(mdi, mdi.address);
        if (!isTestMode()) {
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
            mdi.nAppearances = fileSize(path) / sizeof(CAppearance_base);
            mdi.sizeInBytes = fileSize(path);
            string_q fn = substitute(path, ".acct.bin", ".txs.bin");
            mdi.sizeInBytes2 = fileSize(fn);
        } else {
            mdi = CMonitorCacheItem();
            mdi.address = "---address---";
            mdi.name = "--name--";
            mdi.tags = "--tags--";
            mdi.source = "--source--";
            mdi.symbol = "--symbol--";
        }

        CMonitor m;
        m.address = mdi.address;
        mdi.deleted = m.isDeleted();
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
        CItemCounter* counter = reinterpret_cast<CItemCounter*>(data);

        timestamp_t unused;
        blknum_t last = NOPOS;
        blknum_t first = bnFromPath(path, last, unused);
        if (!isTestMode()) {
            LOG_PROGRESS1("Scanning", ++counter->fileRange.first, counter->fileRange.second, "\r");
        }

        if (last < counter->scanRange.first)
            return true;
        if (first > counter->scanRange.second)
            return false;  //! contains(path, "finalized");
        if (!endsWith(path, ".bin"))
            return true;

        ASSERT(counter->options);
        CIndexCacheItem aci;
        aci.type = aci.getRuntimeClass()->m_ClassName;
        expContext().types[aci.type] = aci.getRuntimeClass();
        aci.filename = substitute(path, counter->cachePtr->path, "");
        string_q fn = aci.filename;
        if (isTestMode())
            aci.filename = substitute(aci.filename, indexFolder_finalized, "/--index-path--/");
        uint64_t tmp;
        aci.firstAppearance = (uint32_t)bnFromPath(fn, tmp, unused);
        aci.latestAppearance = (uint32_t)tmp;
        CStringArray parts;
        explode(parts, path, '/');
        blknum_t num = str_2_Uint(nextTokenClear(parts[parts.size() - 1], '-'));
        {
            aci.bloom_hash = counter->options->bloomHashes[num];
            size_t len = aci.bloom_hash.length();
            if (len && !verbose)
                aci.bloom_hash = aci.bloom_hash.substr(0, 4) + "..." + aci.bloom_hash.substr(len - 4, len);
            aci.bloomSizeBytes =
                (uint32_t)fileSize(substitute(substitute(path, "finalized", "blooms"), ".bin", ".bloom"));
        }
        {
            aci.index_hash = counter->options->indexHashes[num];
            size_t len = aci.index_hash.length();
            if (len && !verbose)
                aci.index_hash = aci.index_hash.substr(0, 4) + "..." + aci.index_hash.substr(len - 4, len);
            aci.indexSizeBytes = (uint32_t)fileSize(path);
        }

        if (counter->ts_array) {
            if (aci.firstAppearance < counter->ts_cnt && aci.latestAppearance < counter->ts_cnt) {
                aci.firstTs = (timestamp_t)counter->ts_array[(aci.firstAppearance * 2) + 1];
                aci.latestTs = (timestamp_t)counter->ts_array[(aci.latestAppearance * 2) + 1];
            } else {
                aci.firstTs = (timestamp_t)0;
                aci.latestTs = (timestamp_t)0;
            }
        }
        getIndexMetrics(path, aci.nAppearances, aci.nAddresses);
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
        expContext().types[abii.type] = abii.getRuntimeClass();
        string_q addr = substitute(path, "/0x", "|");
        nextTokenClear(addr, '|');
        abii.address = "0x" + nextTokenClear(addr, '.');
        CAccountName n;
        counter->options->getNamedAccount(n, abii.address);
        if (isTestMode())
            abii.address = "---address---";
        abii.name = n.name;
        if (isTestMode()) {
            abii.nFunctions = abii.nEvents = abii.nOther = abii.sizeInBytes = 36963;
        } else {
            CAbi abi;
            abi.loadAbiFromFile(path, false);
            abi.sortInterfaces();
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

        CPriceCacheItem pri;
        pri.type = pri.getRuntimeClass()->m_ClassName;
        expContext().types[pri.type] = pri.getRuntimeClass();
        if (isTestMode()) {
            pri.pair = "---pair---";
            pri.sizeInBytes = 1010202;
            pri.nAppearances = 2020101;
        } else {
            string_q pair = substitute(path, "/0x", "|");
            nextTokenClear(pair, '|');
            pri.pair = substitute(substitute(path, counter->cachePtr->path, ""), ".bin", "");
            pri.sizeInBytes = fileSize(path);
            pri.nAppearances = fileSize(path) / sizeof(CPriceQuote);
        }
        counter->priceArray->push_back(pri);
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
