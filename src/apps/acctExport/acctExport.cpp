/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//-----------------------------------------------------------------------
bool exportData(COptions& options) {

    string_q transFmt = expContext().fmtMap["trans_fmt"];
    string_q traceFmt = expContext().fmtMap["trace_fmt"];
    string_q header = toLower(transFmt);
    for (uint32_t i = 0 ; i < 10 ; i++) {
        string_q str = "w:" + uint_2_Str(i);
        header = substitute(header, str, "");
    }
    for (auto const& ch : header)
        if (ch != '[' && ch != '{' && ch != '}' && ch != ']' && ch != ':')
            cout << ch;
    cout << endl;

    bool isJson = transFmt.empty();
    if (isJson)
        cout << "[";

    bool first = true;
    for (size_t i = 0 ; i < options.items.size() && !shouldQuit() ; i++) {
        const CAppearance_base *item = &options.items[i];

        CBlock block; // do not move this from this scope
        CTransaction trans;
        string_q txFilename = getBinaryCacheFilename(CT_TXS, item->blk, item->txid);
        if (fileExists(txFilename)) {
            readTransFromBinary(trans, txFilename);
            trans.finishParse();
        } else {
            getBlock(block, item->blk);
            if (item->txid < block.transactions.size()) {
                CTransaction *tx = &block.transactions[item->txid];
                tx->timestamp = block.timestamp;
                writeTransToBinary(*tx, txFilename);
                trans = *tx;
                tx->pBlock = &block;
            } else {
                // silently return
                return true;
            }
        }

        if (!IS_HIDDEN(CTransaction, "traces")) {
            if (transFmt.empty() || contains(toLower(transFmt), "traces")) {
                getTraces(trans.traces, trans.hash);
                for (size_t j = 0 ; j < trans.traces.size() ; j++)
                    trans.traces[j].pTrans = &trans;
            }
        }

        for (size_t w = 0 ; w < options.monitors.size() ; w++) {
            options.monitors[w].abi_spec.articulateTransaction(&trans);
            HIDE_FIELD(CFunction, "message");
            if (!trans.articulatedTx.message.empty())
                SHOW_FIELD(CFunction, "message");
        }

        if (isJson && !first)
            cout << ", ";
        cout << trans.Format(transFmt) << endl;
        HIDE_FIELD(CFunction, "message");
    }

    if (isJson)
        cout << "]";

    return true;
}

//-----------------------------------------------------------------------
bool loadData(CAppearanceArray_base& apps, const address_t& addr) {

    string_q fn = getMonitorPath(addr);

    size_t nRecords = (fileSize(fn) / sizeof(CAppearance_base));
    ASSERT(nRecords);

    CAppearance_base *buffer = new CAppearance_base[nRecords];
    if (buffer) {
        bzero(buffer, nRecords * sizeof(CAppearance_base));

        CArchive txCache(READING_ARCHIVE);
        if (txCache.Lock(fn, modeReadOnly, LOCK_NOWAIT)) {
            txCache.Read(buffer, sizeof(CAppearance_base), nRecords);
            txCache.Release();
        } else {
            cerr << "Could not open old style cache file. Quiting...";
            return false;
        }

        // Add to the apps which may be non-empty
        apps.reserve(apps.size() + nRecords);
        for (size_t i = 0 ; i < nRecords ; i++)
            apps.push_back(buffer[i]);

        delete [] buffer;

    } else {
        cerr << "Could not allocate memory for address " << addr << endl;
        return false;

    }

    return true;
}

//-----------------------------------------------------------------------
bool loadData(COptions& options) {

    CAppearanceArray_base tmp;
    for (auto monitor : options.monitors) {
        if (!loadData(tmp, monitor.address)) {
            cerr << "Could not load data. Quitting...";
            return false;
        }
    }
    if (tmp.size() == 0) {
        cerr << "Nothing to export. Quitting...";
        return false;
    }

    sort(tmp.begin(), tmp.end());

    bool hasFuture = false;
    options.items.push_back(tmp[0]);
    for (auto item : tmp) {
        CAppearance_base *prev = &options.items[options.items.size() - 1];
        if (item.blk != prev->blk || item.txid != prev->txid) {
            if (item.blk > options.lastAtClient)
                hasFuture = true;
            else
                options.items.push_back(item);
        }
    }

    if (hasFuture)
        cerr << bRed << "[WARNING]: " << cTeal << "Cache file contains blocks ahead of the local chain. These items cannot be exported." << cOff << endl;

    return true;
}

//-----------------------------------------------------------------------
int main(int argc, const char *argv[]) {

    acctlib_init(quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;
        loadData(options);
        exportData(options);
    }

    acctlib_cleanup();
    return 0;
}
