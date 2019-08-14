/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//--------------------------------------------------------------------------------
void COptions::doStatus(ostream& os) {

    CAddressCache aid;
    if (contains(mode, "|scraper|")) {
        aid.type = aid.getRuntimeClass()->m_ClassName;
        aid.path = getIndexPath("");
        forEveryFileInFolder(aid.path, countFiles, &aid);
        status.caches.push_back(&aid);
    }

    CMonitorCache md;
    if (contains(mode, "|monitors|")) {
        md.type = md.getRuntimeClass()->m_ClassName;
        md.path = getCachePath("monitors/");
        forEveryFileInFolder(md.path, countFiles, &md);
        status.caches.push_back(&md);
        CItemCounter counter(this);
        counter.cachePtr = &md;
        counter.monitorArray = &md.items;
        if (details) {
            forEveryFileInFolder(md.path, noteMonitor, &counter);
        } else {
            forEveryFileInFolder(md.path, noteMonitor_light, &counter);
        }
    }

//names

    CAbiCache abi;
    if (contains(mode, "|abis|")) {
        abi.type = abi.getRuntimeClass()->m_ClassName;
        abi.path = getCachePath("abis/");
        forEveryFileInFolder(abi.path, countFiles, &abi);
        status.caches.push_back(&abi);
        if (details) {
            CItemCounter counter(this);
            counter.cachePtr = &abi;
            counter.abiArray = &abi.items;
            forEveryFileInFolder(abi.path, noteABI, &counter);
        }
    }

    CChainCache cd_blocks;
    if (contains(mode, "|blocks|")) {
        cd_blocks.type = cd_blocks.getRuntimeClass()->m_ClassName;
        cd_blocks.path = getCachePath("blocks/");
        forEveryFileInFolder(cd_blocks.path, countFiles, &cd_blocks);
        status.caches.push_back(&cd_blocks);
    }

    CChainCache cd_txs;
    if (contains(mode, "|txs|")) {
        cd_txs.type = cd_txs.getRuntimeClass()->m_ClassName;
        cd_txs.path = getCachePath("txs/");
        forEveryFileInFolder(cd_txs.path, countFiles, &cd_txs);
        status.caches.push_back(&cd_txs);
    }

    CChainCache cd_traces;
    if (contains(mode, "|traces|")) {
        cd_traces.type = cd_traces.getRuntimeClass()->m_ClassName;
        cd_traces.path = getCachePath("traces/");
        forEveryFileInFolder(cd_traces.path, countFiles, &cd_traces);
        status.caches.push_back(&cd_traces);
    }

    CSlurpCache slurps;
    if (contains(mode, "|slurps|")) {
        slurps.type = slurps.getRuntimeClass()->m_ClassName;
        slurps.path = getCachePath("slurps/");
        forEveryFileInFolder(slurps.path, countFiles, &slurps);
        status.caches.push_back(&slurps);
        CItemCounter counter(this);
        counter.cachePtr = &md;
        counter.monitorArray = &md.items;
        if (details) {
            forEveryFileInFolder(md.path, noteMonitor, &counter);
        } else {
            forEveryFileInFolder(md.path, noteMonitor_light, &counter);
        }
    }

    CPriceCache pd;
    if (contains(mode, "|prices|")) {
        pd.type = pd.getRuntimeClass()->m_ClassName;
        pd.path = getCachePath("prices/");
        forEveryFileInFolder(pd.path, countFiles, &pd);
        status.caches.push_back(&pd);
        if (details) {
            CItemCounter counter(this);
            counter.cachePtr = &pd;
            counter.priceArray = &pd.items;
            forEveryFileInFolder(pd.path, notePrice, &counter);
        }
    }

    os << exportPreamble(JSON1, expContext().fmtMap["header"], GETRUNTIME_CLASS(CStatus));
    os << status << endl;
    os << exportPostamble(JSON1, expContext().fmtMap["meta"]);

    return;
}

//---------------------------------------------------------------------------
bool countFiles(const string_q& path, void *data) {

    CCache *counter = (CCache *)data;
    if (endsWith(path, '/')) {
        if (!contains(path, "monitors/staging") && !isTestMode())
            counter->noteFolder(path);
        return forEveryFileInFolder(path + "*", countFiles, data);

    } else if (endsWith(path, ".bin") || endsWith(path, ".json")) {
        if (isTestMode()) {
            counter->noteFile("testing");
            counter->valid_counts = true;
            return false;
        }
        counter->noteFile(path);
        counter->valid_counts = true;

    }
    return true;
}

//---------------------------------------------------------------------------
bool noteMonitor_light(const string_q& path, void *data) {

    if (contains(path, "/staging"))
        return true;

    if (endsWith(path, '/')) {
        return forEveryFileInFolder(path + "*", noteMonitor_light, data);

    } else if (endsWith(path, ".bin") || endsWith(path, ".json")) {

        CItemCounter *counter = (CItemCounter*)data;
        ASSERT(counter->options);

        if (isTestMode()) {
            ((CMonitorCache*)counter->cachePtr)->addrs.push_back("0xTesting");
            return false;
        }
        ((CMonitorCache*)counter->cachePtr)->addrs.push_back(substitute(substitute(substitute(substitute(path, counter->cachePtr->path, ""),".acct", ""),".bin", ""), ".json", ""));
    }
    return true;
}

//---------------------------------------------------------------------------
bool noteMonitor(const string_q& path, void *data) {

    if (contains(path, "/staging"))
        return true;

    if (endsWith(path, '/')) {
        return forEveryFileInFolder(path + "*", noteMonitor, data);

    } else if (endsWith(path, ".bin") || endsWith(path, ".json")) {

        CItemCounter *counter = (CItemCounter*)data;
        ASSERT(counter->options);

        CMonitorCacheItem mdi;
        if (isTestMode()) {
            mdi.type = mdi.getRuntimeClass()->m_ClassName;
            mdi.address = "0xTesting";
            mdi.firstAppearance = 1001001;
            mdi.latestAppearance = 8101001;
            mdi.nRecords = 1001;
            mdi.sizeInBytes = 1002;
            counter->monitorArray->push_back(mdi);
            return false;
        }

        mdi.type = mdi.getRuntimeClass()->m_ClassName;
        mdi.address = substitute(substitute(substitute(substitute(path, counter->cachePtr->path, ""),".acct", ""),".bin", ""), ".json", "");
        counter->options->getNamedAccount(mdi, mdi.address);
        mdi.firstAppearance = 1001001;
        mdi.latestAppearance = 8101001;
        mdi.nRecords = fileSize(path) / sizeof(CAppearance_base);
        mdi.sizeInBytes = fileSize(path);
        counter->monitorArray->push_back(mdi);

    }
    return true;
}

//---------------------------------------------------------------------------
bool noteABI(const string_q& path, void *data) {

    if (contains(path, "/staging"))
        return true;

    if (endsWith(path, '/')) {
        return forEveryFileInFolder(path + "*", noteABI, data);

    } else if (endsWith(path, ".bin") || endsWith(path, ".json")) {

        CItemCounter *counter = (CItemCounter*)data;
        ASSERT(counter->options);

        CAbiCacheItem abii;
        if (isTestMode()) {
            abii.type = abii.getRuntimeClass()->m_ClassName;
            abii.address = "0xTesting";
            abii.firstAppearance = 1001001;
            abii.latestAppearance = 8101001;
            abii.nRecords = 1001;
            abii.sizeInBytes = 1002;
            counter->abiArray->push_back(abii);
            return false;
        }

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
    return true;
}

//---------------------------------------------------------------------------
bool notePrice(const string_q& path, void *data) {

    if (endsWith(path, '/')) {
        return forEveryFileInFolder(path + "*", notePrice, data);

    } else if (endsWith(path, ".bin")) {

        CItemCounter *counter = (CItemCounter*)data;
        ASSERT(counter->options);

        CPriceCacheItem price;
        if (isTestMode()) {
            price.type = price.getRuntimeClass()->m_ClassName;
            price.address = "0xTesting";
            price.firstAppearance = 1001001;
            price.latestAppearance = 8101001;
            price.nRecords = 1001;
            price.sizeInBytes = 1002;
            counter->priceArray->push_back(price);
            return false;
        }
        price.type = price.getRuntimeClass()->m_ClassName;
        price.pair = substitute(substitute(substitute(substitute(path, counter->cachePtr->path, ""),".acct", ""),".bin", ""), ".json", "");
        price.sizeInBytes = fileSize(path);
        price.nRecords = fileSize(path) / sizeof(CPriceQuote);
        counter->priceArray->push_back(price);
    }
    return true;
}
