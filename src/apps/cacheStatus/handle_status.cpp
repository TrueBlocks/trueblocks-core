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
extern string_q pathName(const string_q& str, const string_q& path);
extern bool getChainList(CChainArray& chains);
extern bool getKeyList(CKeyArray& keys);
//--------------------------------------------------------------------------------
bool COptions::handle_status(ostream& os) {
    if (terse) {
        string_q fmt = STR_TERSE_REPORT;
        if (getEnvStr("LOG_TIMING_OFF") == "true") {
            // Do nothing
        } else {
            replaceAll(
                fmt, "[{TIME}]",
                isTestMode() ? "--TIME--" : substitute(substitute(Now().Format(FMT_EXPORT), "T", " "), "-", "/"));
        }

        CStatusTerse st = status;
        bool isText = expContext().exportFmt != JSON1;
        if (!isText) {
            fmt = "";
            manageFields("CStatusTerse:modes1,modes2", FLD_HIDE);
            manageFields("CStatus:clientIds,host,isApi,isScraping,caches,chains,keys", FLD_HIDE);
        }
        size_t nRecords = ((fileSize(indexFolderBin_ts) / sizeof(uint32_t)) / 2);
        ostringstream m;
        if (isTestMode())
            m << "--client--, --final--, --staging--, --unripe-- ts: --ts--";
        else
            m << meta.client << ", " << meta.finalized << ", " << meta.staging << ", " << meta.unripe << ", "
              << "ts: " << nRecords;
        replace(fmt, "[{PROGRESS}]", m.str());
        ostringstream n;
        n << meta.chain << " (" << meta.chainId << "," << meta.networkId << ")";
        replace(fmt, "[{CHAINDATA}]", n.str());

        string_q res = st.Format(fmt);
        replaceAll(res, "++C1++", cGreen);
        replaceAll(res, "++C2++", cOff);
        os << res << endl;

        return true;
    }

    establishCacheFolders();

    CIndexCache index;
    if (contains(mode, "|index|")) {
        LOG8("Reporting on index");
        if (!index.readBinaryCache(indexFolder_finalized, "index", details)) {
            LOG8("Regenerating cache");
            index.type = index.getRuntimeClass()->m_ClassName;
            index.path = pathName("index", indexFolder);
            forEveryFileInFolder(indexFolder, countFiles, &index);
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
        if (!isTestMode() && !origMode.empty()) {
            LOG_PROG("Complete ", index.nFiles, " of ", index.nFiles);
        }
        status.caches.push_back(&index);
    }

    LOG4("Processing monitors");
    CMonitorCache monitors;
    if (contains(mode, "|monitors|")) {
        LOG8("Reporting on monitors");
        if (!monitors.readBinaryCache(cacheFolder_monitors, "monitors", details)) {
            CMonitor m;
            string_q thePath = m.getPathToMonitor("", false);
            monitors.type = monitors.getRuntimeClass()->m_ClassName;
            monitors.path = pathName("monitors", cacheFolder_monitors);
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
                monitors.isValid = true;
            }
            LOG4("forEvery monitors done");
            LOG8("\tWriting monitors cache");
            //            monitors.writeBinaryCache("monitors", details);
            LOG4("done writing cache");
        }
        SHOW_FIELD(CMonitorCacheItem, "deleted");
        status.caches.push_back(&monitors);
    }

    CNameCache names;
    if (contains(mode, "|names|")) {
        LOG8("Reporting on names");
        if (!names.readBinaryCache(cacheFolder_names, "names", details)) {
            string_q thePath = cacheFolder_names;
            names.type = names.getRuntimeClass()->m_ClassName;
            names.path = pathName("names", cacheFolder_names);
            forEveryFileInFolder(thePath, countFiles, &names);
            CItemCounter counter(this);
            counter.cachePtr = &names;
            counter.monitorArray = &names.items;
            if (details) {
                forEveryFileInFolder(thePath, noteMonitor, &counter);
            } else {
                forEveryFileInFolder(thePath, noteMonitor_light, &counter);
                names.isValid = true;
            }
            LOG8("\tre-writing names cache");
            names.writeBinaryCache("names", details);
        }
        status.caches.push_back(&names);
    }

    CAbiCache abi_cache;
    if (contains(mode, "|abis|")) {
        LOG8("Reporting on abis");
        if (!abi_cache.readBinaryCache(cacheFolder_abis, "abis", details)) {
            string_q thePath = cacheFolder_abis;
            abi_cache.type = abi_cache.getRuntimeClass()->m_ClassName;
            abi_cache.path = pathName("abis", cacheFolder_abis);
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
        if (!blocks.readBinaryCache(cacheFolder_blocks, "blocks", details)) {
            string_q thePath = cacheFolder_blocks;
            blocks.type = blocks.getRuntimeClass()->m_ClassName;
            blocks.path = pathName("blocks", cacheFolder_blocks);
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
        if (!txs.readBinaryCache(cacheFolder_txs, "txs", details)) {
            string_q thePath = cacheFolder_txs;
            txs.type = txs.getRuntimeClass()->m_ClassName;
            txs.path = pathName("txs", cacheFolder_txs);
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
        if (!traces.readBinaryCache(cacheFolder_traces, "traces", details)) {
            string_q thePath = cacheFolder_traces;
            traces.type = traces.getRuntimeClass()->m_ClassName;
            traces.path = pathName("traces", cacheFolder_traces);
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
        if (!slurps.readBinaryCache(cacheFolder_slurps, "slurps", details)) {
            string_q thePath = cacheFolder_slurps;
            slurps.type = slurps.getRuntimeClass()->m_ClassName;
            slurps.path = pathName("slurps", cacheFolder_slurps);
            forEveryFileInFolder(thePath, countFiles, &slurps);
            CItemCounter counter(this);
            counter.cachePtr = &slurps;
            counter.monitorArray = &slurps.items;
            if (details) {
                forEveryFileInFolder(thePath, noteMonitor, &counter);
            } else {
                HIDE_FIELD(CSlurpCache, "addrs");
                forEveryFileInFolder(thePath, noteMonitor_light, &counter);
                slurps.isValid = true;
            }
            LOG8("\tre-writing slurps cache");
            slurps.writeBinaryCache("slurps", details);
        }
        status.caches.push_back(&slurps);
    }

    if (origMode.empty() || contains(origMode, "all") || contains(origMode, "some")) {
        getChainList(status.chains);
        getKeyList(status.keys);
    } else {
        HIDE_FIELD(CStatus, "chains");
        HIDE_FIELD(CStatus, "keys");
    }
    status.toJson(os);

    return true;
}

//---------------------------------------------------------------------------
bool noteMonitor_light(const string_q& path, void* data) {
    if (contains(path, "/staging"))
        return !shouldQuit();

    if (endsWith(path, '/')) {
        return forEveryFileInFolder(path + "*", noteMonitor_light, data);

    } else if (isMonitorFilePath(path)) {
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
bool noteMonitor(const string_q& path, void* data) {
    if (contains(path, "/staging"))
        return !shouldQuit();

    if (endsWith(path, '/')) {
        return forEveryFileInFolder(path + "*", noteMonitor, data);

    } else if (isMonitorFilePath(path)) {
        CMonitorCacheItem mdi;
        mdi.type = mdi.getRuntimeClass()->m_ClassName;

        string_q addr = substitute(path, "/0x", "|");
        nextTokenClear(addr, '|');
        mdi.address = "0x" + nextTokenClear(addr, '.');
        mdi.petname = addr_2_Petname(mdi.address, '-');
        findName(mdi.address, mdi);
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
            mdi.petname = "--petname--";
            mdi.name = "--name--";
            mdi.tags = "--tags--";
            mdi.source = "--source--";
            mdi.symbol = "--symbol--";
        }

        CMonitor m;
        m.address = mdi.address;
        m.petname = mdi.petname;
        CMonitorHeader header;
        m.readMonitorHeader(header);
        mdi.deleted = header.deleted;

        CItemCounter* counter = reinterpret_cast<CItemCounter*>(data);
        ASSERT(counter->options);
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
        blknum_t first = path_2_Bn(path, last, unused);
        if (!isTestMode()) {
            LOG_PROG("Scanning ", ++counter->fileRange.first, " of ", counter->fileRange.second, "\r");
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
        aci.filename = substitute(path, counter->cachePtr->path, "");
        string_q fn = aci.filename;
        if (isTestMode())
            aci.filename = substitute(aci.filename, indexFolder_finalized, "/--index-path--/");
        uint64_t tmp;
        aci.firstApp = (uint32_t)path_2_Bn(fn, tmp, unused);
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

        aci.firstTs = bn_2_Timestamp(aci.firstApp);
        aci.latestTs = bn_2_Timestamp(aci.latestApp);

        CIndexArchive index(READING_ARCHIVE);
        if (index.ReadIndexFromBinary(path, IP_HEADER)) {
            aci.nApps = index.header.nApps;
            aci.nAddrs = index.header.nAddrs;
            counter->indexArray->push_back(aci);
        }
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
        string_q addr = substitute(path, "/0x", "|");
        nextTokenClear(addr, '|');
        abii.address = "0x" + nextTokenClear(addr, '.');
        abii.petname = addr_2_Petname(abii.address, '-');
        CAccountName n;
        findName(abii.address, n);
        if (isTestMode()) {
            abii.address = "---address---";
            abii.petname = "--petname--";
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

//--------------------------------------------------------------------------------
string_q pathName(const string_q& str, const string_q& path) {
    return (isTestMode() ? str + "Path" : path);
}

//--------------------------------------------------------------------------------
const char* STR_TERSE_REPORT =
    "[{TIME}] ++C1++Client:++C2++       [{CLIENTVERSION}][{MODES1}]\n"
    "[{TIME}] ++C1++TrueBlocks:++C2++   [{TRUEBLOCKSVERSION}][{MODES2}]\n"
    "[{TIME}] ++C1++RPC Provider:++C2++ [{RPCPROVIDER}] - [{CHAINDATA}]\n"
    "[{TIME}] ++C1++Config Path:++C2++  [{CONFIGPATH}]\n"
    "[{TIME}] ++C1++Cache Path:++C2++   [{CACHEPATH}]\n"
    "[{TIME}] ++C1++Index Path:++C2++   [{INDEXPATH}]\n"
    "[{TIME}] ++C1++Progress:++C2++     [{PROGRESS}]";

//--------------------------------------------------------------------------------
static void replaceNames(const string_q& chain, string_q& key, string_q& value) {
    if (!isTestMode())
        return;
    if (containsI(key, "Explorer"))
        value = "--explorer-" + chain + "--";
    else if (containsI(key, "Provider"))
        value = "--provider-" + chain + "--";
    else if (containsI(key, "Gateway"))
        value = "--gateway-" + chain + "--";
}

//--------------------------------------------------------------------------------
bool getChainList(CChainArray& chains) {
    // TODO: This hacky assed code is because our TOML code does not read arrays
    string_q configFn = rootConfigToml_trueBlocks;
    string_q binFn = cacheFolder_tmp + "chains.bin";

    time_q configFileDate = fileLastModifyDate(configFn);
    time_q binFileDate = fileLastModifyDate(binFn);
    if (!isTestMode() && binFileDate > configFileDate) {
        CArchive archive(READING_ARCHIVE);
        if (archive.Lock(cacheFolder_tmp + "chains.bin", modeReadOnly, LOCK_NOWAIT)) {
            archive >> chains;
            archive.Release();
            return true;
        }
    }

    typedef enum { SEARCHING = 0, IN_CHAINS } State;
    State state = SEARCHING;

    CStringArray lines, lines2;
    asciiFileToLines(rootConfigToml_trueBlocks, lines);
    for (auto line : lines) {
        if (!line.empty()) {
            if (state == SEARCHING) {
                if (line == "[chains]")
                    state = IN_CHAINS;
            } else {
                lines2.push_back(line);
            }
        }
    }

    map<string, CChain> chainMap;
    CChain current;
    for (auto line : lines2) {
        if (startsWith(line, "[chains.")) {
            if (!current.chain.empty()) {
                if (!isTestMode() || current.chain == "mainnet" || current.chain == "gnosis")
                    chains.push_back(current);
            }
            current = CChain();
            current.chain = substitute(substitute(line, "[chains.", ""), "]", "");
        } else {
            CStringArray parts;
            explode(parts, line, '=');
            if (parts.size() == 2) {
                parts[0] = trim(substitute(parts[0], "\"", ""));
                parts[1] = trim(nextTokenClear(parts[1], '#'));
                parts[1] = trim(substitute(parts[1], "\"", ""));
                replaceNames(current.chain, parts[0], parts[1]);
                current.setValueByName(parts[0], parts[1]);
            }
        }
    }
    if (!current.chain.empty()) {
        if (!isTestMode() || current.chain == "mainnet" || current.chain == "gnosis")
            chains.push_back(current);
    }

    if (!isTestMode()) {
        CArchive archive(WRITING_ARCHIVE);
        if (archive.Lock(cacheFolder_tmp + "chains.bin", modeWriteCreate, LOCK_WAIT)) {
            archive << chains;
            archive.Release();
        }
    }

    return true;
}

//--------------------------------------------------------------------------------
bool getKeyList(CKeyArray& keys) {
    // TODO: This hacky assed code is because our TOML code does not read arrays
    string_q configFn = rootConfigToml_trueBlocks;
    string_q binFn = cacheFolder_tmp + "keys.bin";

    time_q configFileDate = fileLastModifyDate(configFn);
    time_q binFileDate = fileLastModifyDate(binFn);
    if (!isTestMode() && binFileDate > configFileDate) {
        CArchive archive(READING_ARCHIVE);
        if (archive.Lock(cacheFolder_tmp + "keys.bin", modeReadOnly, LOCK_NOWAIT)) {
            archive >> keys;
            archive.Release();
            return true;
        }
    }

    typedef enum { SEARCHING = 0, IN_CHAINS } State;
    State state = SEARCHING;

    CStringArray lines, lines2;
    asciiFileToLines(rootConfigToml_trueBlocks, lines);
    for (auto line : lines) {
        if (!line.empty()) {
            if (state == SEARCHING) {
                if (line == "[keys]")
                    state = IN_CHAINS;
            } else {
                lines2.push_back(line);
            }
        }
    }

    map<string, CKey> keyMap;
    CKey current;
    for (auto line : lines2) {
        if (startsWith(line, "[keys.")) {
            if (!current.provider.empty()) {
                if (!isTestMode() || current.provider == "etherscan" || current.provider == "pinata")
                    keys.push_back(current);
            }
            current = CKey();
            current.provider = substitute(substitute(line, "[keys.", ""), "]", "");
        } else {
            CStringArray parts;
            explode(parts, line, '=');
            if (parts.size() == 2) {
                parts[0] = trim(substitute(parts[0], "\"", ""));
                parts[1] = trim(nextTokenClear(parts[1], '#'));
                parts[1] = trim(substitute(parts[1], "\"", ""));
                replaceNames(current.provider, parts[0], parts[1]);
                current.setValueByName(parts[0], parts[1]);
            }
        }
    }
    if (!current.provider.empty()) {
        if (!isTestMode() || current.provider == "etherscan" || current.provider == "pinata")
            keys.push_back(current);
    }

    if (!isTestMode()) {
        CArchive archive(WRITING_ARCHIVE);
        if (archive.Lock(cacheFolder_tmp + "keys.bin", modeWriteCreate, LOCK_WAIT)) {
            archive << keys;
            archive.Release();
        }
    } else {
        for (auto& key : keys) {
            key.apiKey = "--api-key--";
            key.jwt = "--jwt--";
            key.secret = "--secret";
        }
    }

    return true;
}