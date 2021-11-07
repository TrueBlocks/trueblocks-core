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

//----------------------------------------------------------------
bool showApp(const CAppearance& item, void* data) {
    COptions* opt = reinterpret_cast<COptions*>(data);

    static blknum_t last = NOPOS;
    if (item.bn != last) {
        cout << endl;
        last = item.bn;
    }

    cout << ((isJson() && !opt->firstOut) ? ", " : "");
    cout << item;
    opt->firstOut = false;

    return !shouldQuit();
}

//----------------------------------------------------------------
bool visitApp(const CAppearance& item, void* data) {
    COptions* opt = reinterpret_cast<COptions*>(data);
    if (item.tc == 10 || isZeroAddr(item.addr))
        return !shouldQuit();

    if (item.reason == "input" && item.addr != opt->accountedFor.address)
        return !shouldQuit();
    if (contains(item.reason, "topic") && item.addr != opt->accountedFor.address)
        return !shouldQuit();

    // if (opt->cache) {
    //     string_q path = getBinaryCacheFilename(CT_APPS, item.bn, item.tx);
    //     string_q csvPath = substitute(path, ".bin", ".csv");
    //     ostringstream os;
    //     os << item.Format(STR_DISPLAY_APPEARANCE) << endl;
    //     cout << "Writing: " << item.Format(STR_DISPLAY_APPEARANCE) << endl;
    //     getchar();
    //     appendToAsciiFile(path, os.str());
    // }

    return showApp(item, data);
}

//----------------------------------------------------------------
// Return 'true' if we want the caller NOT to visit the traces of this transaction
bool transFilter(const CTransaction* trans, void* data) {
    if (!ddosRange(trans->blockNumber))
        return false;
    return (getTraceCount(trans->hash) > 250);
}

//-----------------------------------------------------------------------
bool neighbors_Display(CTraverser* trav, void* data) {
    string_q path = getBinaryCacheFilename(CT_APPS, trav->trans.blockNumber, trav->trans.transactionIndex);
    establishFolder(path);

    CAppearanceArray apps;
    if (!isTestMode() && fileExists(path)) {
        // CArchive archive(READING_ARCHIVE);
        // if (archive.Lock(path, modeReadOnly, LOCK_NOWAIT)) {
        //     // archive >> apps;
        //     archive.Release();
        //     for (auto app : apps) {
        //         showApp(app, data);
        //     }
        //     return true;
        // }
    }

    string_q csvPath = substitute(path, ".bin", ".csv");
    // ::remove(csvPath.c_str());  // we don't have a cache, so clear out the temp file
    trav->trans.forEveryUniqueAppearanceInTxPerTx(visitApp, transFilter, data);

    // COptions* opt = (COptions*)data;
    // if (opt->cache) {
    //     cout << csvPath << endl;
    //     getchar();
    //     if (fileExists(csvPath)) {
    //         CStringArray lines, fields;
    //         asciiFileToLines(csvPath, lines);
    //         for (auto line : lines) {
    //             if (fields.empty()) {
    //                 fields = CStringArray{"bn", "tx", "tc", "addr", "reason"};
    //             } else {
    //                 CAppearance app;
    //                 app.parseCSV(fields, line);
    //                 apps.push_back(app);
    //             }
    //         }
    //         CArchive archive(WRITING_ARCHIVE);
    //         if (archive.Lock(path, modeWriteCreate, LOCK_WAIT)) {
    //             archive << apps;
    //             archive.Release();
    //         }
    //     }
    // }
    prog_Log(trav, data);
    return !shouldQuit();
}

//-----------------------------------------------------------------------
size_t neighbors_Count(CTraverser* trav, void* data) {
    return trav->trans.receipt.logs.size();
}
