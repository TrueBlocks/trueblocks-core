/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"
#include "statusterse.h"

extern const char* STR_TERSE_REPORT;
extern string_q pathName(const string_q& str, const string_q& path = "");
//--------------------------------------------------------------------------------
bool COptions::handle_status(ostream& os) {
    ENTER("handle_status");

    if (terse) {
        string_q fmt = STR_TERSE_REPORT;
        replaceAll(fmt, "[{TIME}]",
                   isTestMode() ? "--TIME--" : substitute(substitute(Now().Format(FMT_EXPORT), "T", " "), "-", "/"));

        CStatusTerse st = status;
        bool isText = expContext().exportFmt != JSON1 && expContext().exportFmt != API1;
        if (!isText) {
            fmt = "";
            manageFields("CStatusTerse:modes1,modes2", FLD_HIDE);
            manageFields("CStatus:client_ids,balance_provider,host,is_api,is_scraping,caches", FLD_HIDE);
        }
        os << st.Format(fmt) << endl;
        return true;
    }

    establishFolder(getCachePath("abis/"));
    establishFolder(getCachePath("blocks/"));
    establishFolder(getCachePath("monitors/"));
    establishFolder(getCachePath("names/"));
    establishFolder(getCachePath("prices/"));
    establishFolder(getCachePath("slurps/"));
    establishFolder(getCachePath("tmp/"));
    establishFolder(getCachePath("traces/"));
    establishFolder(getCachePath("txs/"));

    CIndexCache index;
    if (contains(mode, "|index|")) {
        LOG8("Reporting on index");
        if (!index.readBinaryCache("index", details)) {
            string_q thePath = getIndexPath("");
            LOG8("Regenerating cache");
            index.type = index.getRuntimeClass()->m_ClassName;
            expContext().types[index.type] = index.getRuntimeClass();
            index.path = pathName("index", thePath);
            forEveryFileInFolder(thePath, countFiles, &index);
            LOG8("Counted files");
            CItemCounter counter(this);
            loadTimestamps(&counter.tsMemMap, counter.tsCnt);
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
            CMonitor m;
            string_q thePath = m.getMonitorPath("");
            monitors.type = monitors.getRuntimeClass()->m_ClassName;
            expContext().types[monitors.type] = monitors.getRuntimeClass();
            monitors.path = pathName("monitors");
            LOG4("counting monitors");
            forEveryFileInFolder(thePath, countFiles, &monitors);
            LOG4("done counting monitors");
            CItemCounter counter(this);
            counter.cachePtr = &monitors;
            counter.monitorArray = &monitors.items;
            LOG4("forEvery monitors");
            if (details) {
                forEveryFileInFolder(thePath, noteMonitor, &counter);
            } else {
                forEveryFileInFolder(thePath, noteMonitor_light, &counter);
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
            string_q thePath = getCachePath("names/");
            names.type = names.getRuntimeClass()->m_ClassName;
            expContext().types[names.type] = names.getRuntimeClass();
            names.path = pathName("names");
            forEveryFileInFolder(thePath, countFiles, &names);
            CItemCounter counter(this);
            counter.cachePtr = &names;
            counter.monitorArray = &names.items;
            if (details) {
                forEveryFileInFolder(thePath, noteMonitor, &counter);
            } else {
                forEveryFileInFolder(thePath, noteMonitor_light, &counter);
                names.is_valid = true;
            }
            LOG8("\tre-writing names cache");
            names.writeBinaryCache("names", details);
        }
        status.caches.push_back(&names);
    }

    CAbiCache abi_cache;
    if (contains(mode, "|abis|")) {
        LOG8("Reporting on abis");
        if (!abi_cache.readBinaryCache("abis", details)) {
            string_q thePath = getCachePath("abis/");
            abi_cache.type = abi_cache.getRuntimeClass()->m_ClassName;
            expContext().types[abi_cache.type] = abi_cache.getRuntimeClass();
            abi_cache.path = pathName("abis");
            forEveryFileInFolder(thePath, countFiles, &abi_cache);
            if (details) {
                CItemCounter counter(this);
                counter.cachePtr = &abi_cache;
                counter.abiArray = &abi_cache.items;
                forEveryFileInFolder(thePath, noteABI, &counter);
            }
            LOG8("\tre-writing abis cache");
            abi_cache.writeBinaryCache("abis", details);
        }
        status.caches.push_back(&abi_cache);
    }

    CChainCache blocks;
    if (contains(mode, "|blocks|") || contains(mode, "|data|")) {
        LOG8("Reporting on blocks");
        if (!blocks.readBinaryCache("blocks", details)) {
            string_q thePath = getCachePath("blocks/");
            blocks.type = blocks.getRuntimeClass()->m_ClassName;
            expContext().types[blocks.type] = blocks.getRuntimeClass();
            blocks.path = pathName("blocks");
            blocks.max_depth = countOf(thePath, '/') + depth;
            forEveryFileInFolder(thePath, countFilesInCache, &blocks);
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
            string_q thePath = getCachePath("txs/");
            txs.type = txs.getRuntimeClass()->m_ClassName;
            expContext().types[txs.type] = txs.getRuntimeClass();
            txs.path = pathName("txs");
            txs.max_depth = countOf(thePath, '/') + depth;
            forEveryFileInFolder(thePath, countFilesInCache, &txs);
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
            string_q thePath = getCachePath("traces/");
            traces.type = traces.getRuntimeClass()->m_ClassName;
            expContext().types[traces.type] = traces.getRuntimeClass();
            traces.path = pathName("traces");
            traces.max_depth = countOf(thePath, '/') + depth;
            forEveryFileInFolder(thePath, countFilesInCache, &traces);
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
            string_q thePath = getCachePath("slurps/");
            slurps.type = slurps.getRuntimeClass()->m_ClassName;
            expContext().types[slurps.type] = slurps.getRuntimeClass();
            slurps.path = pathName("slurps");
            forEveryFileInFolder(thePath, countFiles, &slurps);
            CItemCounter counter(this);
            counter.cachePtr = &slurps;
            counter.monitorArray = &slurps.items;
            if (details) {
                forEveryFileInFolder(thePath, noteMonitor, &counter);
            } else {
                HIDE_FIELD(CSlurpCache, "addrs");
                forEveryFileInFolder(thePath, noteMonitor_light, &counter);
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
            string_q thePath = getCachePath("prices/");
            prices.type = prices.getRuntimeClass()->m_ClassName;
            expContext().types[prices.type] = prices.getRuntimeClass();
            prices.path = pathName("prices");
            forEveryFileInFolder(thePath, countFiles, &prices);
            if (details) {
                CItemCounter counter(this);
                counter.cachePtr = &prices;
                counter.priceArray = &prices.items;
                forEveryFileInFolder(thePath, notePrice, &counter);
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
bool noteEntity(const string_q& path, void* data) {
    if (endsWith(path, '/')) {
        return forEveryFileInFolder(path + "*", noteEntity, data);

    } else if (endsWith(path, ".bin")) {
        CItemCounter* counter = reinterpret_cast<CItemCounter*>(data);
        ASSERT(counter->options);

        CEntityCacheItem coi;
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
        counter->entityArray->push_back(coi);
    }
    return !shouldQuit();
}

//---------------------------------------------------------------------------
bool noteMonitor(const string_q& path, void* data) {
    if (contains(path, "/staging"))
        return !shouldQuit();

    if (endsWith(path, '/')) {
        return forEveryFileInFolder(path + "*", noteMonitor, data);

    } else if (endsWith(path, "acct.bin")) {
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
        if (isTestMode() && !contains(path, "000000000")) {
            // In some cases, the installation may have no index chunks
            // so we report only on block zero (which will be there always)
            return false;
        }

        CItemCounter* counter = reinterpret_cast<CItemCounter*>(data);

        timestamp_t unused;
        blknum_t last = NOPOS;
        blknum_t first = bnFromPath(path, last, unused);
        if (!isTestMode()) {
            LOG_PROGRESS("Scanning", ++counter->fileRange.first, counter->fileRange.second, "\r");
        }

        if (last < counter->options->scanRange.first)
            return true;
        if (first > counter->options->scanRange.second)
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

        if (counter->tsMemMap) {
            if (aci.firstAppearance < counter->tsCnt && aci.latestAppearance < counter->tsCnt) {
                aci.firstTs = (timestamp_t)counter->tsMemMap[(aci.firstAppearance * 2) + 1];
                aci.latestTs = (timestamp_t)counter->tsMemMap[(aci.latestAppearance * 2) + 1];
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
        if (isTestMode()) {
            abii.address = "---address---";
            abii.name = "--name--";
            abii.nFunctions = abii.nEvents = abii.nOther = abii.sizeInBytes = 36963;
        } else {
            abii.name = n.name;
            counter->options->abi_spec = CAbi();  // reset
            loadAbiFile(path, &counter->options->abi_spec);
            abii.nFunctions = counter->options->abi_spec.nFunctions();
            abii.nEvents = counter->options->abi_spec.nEvents();
            abii.nOther = counter->options->abi_spec.nOther();
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
        CAddressBoolMap addrMap;
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

//--------------------------------------------------------------------------------
string_q pathName(const string_q& str, const string_q& path) {
    return (isTestMode() ? str + "Path" : (path.empty() ? getCachePath(str + "/") : path));
}

//--------------------------------------------------------------------------------
const char* STR_TERSE_REPORT =
    "client: [{CLIENT_VERSION}][{MODES1}]\n"
    "[{TIME}] trueblocks: [{TRUEBLOCKS_VERSION}][{MODES2}]\n"
    "[{TIME}] configPath: [{CONFIG_PATH}]\n"
    "[{TIME}] cachePath: [{CACHE_PATH}]\n"
    "[{TIME}] indexPath: [{INDEX_PATH}]\n"
    "[{TIME}] rpcProvider: [{RPC_PROVIDER}]";
