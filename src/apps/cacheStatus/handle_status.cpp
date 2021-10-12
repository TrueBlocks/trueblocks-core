/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "options.h"

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
            string_q thePath = m.getMonitorPath("", false);
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
    if (contains(mode, "|txs|") || contains(mode, "|data|")) {
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
            coi.nApps = 2020101;
        } else {
            coi.name = path;
            coi.sizeInBytes = fileSize(path);
            coi.nApps = fileSize(path) / sizeof(CPriceQuote);
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

    } else if (endsWith(path, "acct.bin")) {
        CItemCounter* counter = reinterpret_cast<CItemCounter*>(data);
        ASSERT(counter->options);
        CMonitorCacheItem mdi;
        mdi.type = mdi.getRuntimeClass()->m_ClassName;
        expContext().types[mdi.type] = mdi.getRuntimeClass();
        string_q addr = substitute(path, "/0x", "|");
        nextTokenClear(addr, '|');
        mdi.address = "0x" + nextTokenClear(addr, '.');
        counter->options->findName(mdi.address, mdi);
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
                mdi.firstApp = first;
                mdi.latestApp = last;
            } else {
                mdi.firstApp = NOPOS;
                mdi.latestApp = NOPOS;
            }
            CMonitor m;
            mdi.nApps = m.getRecordCnt(path);
            mdi.sizeInBytes = fileSize(path);
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
        LOG_PROGRESS("Scanning", ++counter->fileRange.first, counter->fileRange.second, "\r");

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
        aci.firstApp = (uint32_t)bnFromPath(fn, tmp, unused);
        aci.latestApp = (uint32_t)tmp;
        CStringArray parts;
        explode(parts, path, '/');
        blknum_t num = str_2_Uint(nextTokenClear(parts[parts.size() - 1], '-'));
        {
            aci.bloomHash = counter->options->bloomHashes[num];
            size_t len = aci.bloomHash.length();
            if (len && !verbose)
                aci.bloomHash = aci.bloomHash.substr(0, 4) + "..." + aci.bloomHash.substr(len - 4, len);
            aci.bloomSizeBytes =
                (uint32_t)fileSize(substitute(substitute(path, "finalized", "blooms"), ".bin", ".bloom"));
        }
        {
            aci.indexHash = counter->options->indexHashes[num];
            size_t len = aci.indexHash.length();
            if (len && !verbose)
                aci.indexHash = aci.indexHash.substr(0, 4) + "..." + aci.indexHash.substr(len - 4, len);
            aci.indexSizeBytes = (uint32_t)fileSize(path);
        }

        aci.firstTs = getTimestampAt(aci.firstApp);
        aci.latestTs = getTimestampAt(aci.latestApp);

        CIndexHeader header;
        readIndexHeader(path, header);
        aci.nApps = header.nRows;
        aci.nAddrs = header.nAddrs;
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
        counter->options->findName(abii.address, n);
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
            pri.nApps = 2020101;
        } else {
            string_q pair = substitute(path, "/0x", "|");
            nextTokenClear(pair, '|');
            pri.pair = substitute(substitute(path, counter->cachePtr->path, ""), ".bin", "");
            pri.sizeInBytes = fileSize(path);
            pri.nApps = fileSize(path) / sizeof(CPriceQuote);
        }
        counter->priceArray->push_back(pri);
    }
    return !shouldQuit();
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
