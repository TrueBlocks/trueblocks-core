#include "acctlib.h"

//-----------------------------------------------------------------------
bool header(CTraverser* trav, void* data) {
    cout << "bn,tx_id,hash" << endl;
    return true;
}

CStringArray lines;
string_q path = "/Users/jrush/Development/tokenomics.io/giveth/exports/mainnet/neighbors/";
//-----------------------------------------------------------------------
bool display(CTraverser* trav, void* data) {
    cout << "logging: " << trav->logging << endl;
    cout << "nProcessed: " << trav->nProcessed << endl;
    cout << "searchType: " << trav->searchType << endl;
    cout << "curMonitor: " << trav->curMonitor << endl;
    if (trav->curMonitor) {
        cout << "  curMonitor::nAppearances: " << trav->curMonitor->nAppearances << endl;
        cout << "  curMonitor::lastExport: " << trav->curMonitor->lastExport << endl;
        cout << "  curMonitor::firstAppearance: " << trav->curMonitor->firstAppearance << endl;
        cout << "  curMonitor::latestAppearance: " << trav->curMonitor->latestAppearance << endl;
        cout << "  curMonitor::lastVisitedBlock: " << trav->curMonitor->lastVisitedBlock << endl;
        cout << "  curMonitor::sizeInBytes: " << trav->curMonitor->sizeInBytes << endl;
    }
    // string_q p = path + trav->curMonitor->address + ".csv";
    // if (fileExists(p)) {
    //     if (lines.size() == 0) {
    //         asciiFileToLines(p, lines);
    //     }
    //     for (auto line : lines) {
    //         ostringstream os;
    //         os << "\"" << trav->trans.blockNumber << "\",\"" << trav->trans.transactionIndex << "\"";
    //         // cerr << line << endl << os.str() << " " << contains(line, os.str()) << endl;
    //         // getchar();
    //         if (contains(line, os.str()) && contains(line, ",\"to\"")) {
    //             cerr << "The Line: " << line << endl;
    //         }
    //     }
    // } else {
    //     cerr << p << " not found" << endl;
    // }
    for (auto item : trav->monitorMap) {
        cout << "monitorMap: " << item.first << " " << item.second << endl;
    }
    cout << "traverserRange: " << trav->traverserRange << endl;
    cout << trav->app->blk << ",";
    cout << trav->app->txid << ",";
    cout << trav->trans.hash << endl;
    return true;
}

//-----------------------------------------------------------------------
extern "C" CTraverser* makeTraverser(uint32_t wanted) {
    if (getVersionNum() < wanted) {
        LOG_ERR("Cannot load traverser from older versions: ", getVersionNum());
        LOG_ERR("You need to re-install TrueBlocks and rebuild this traverser.");
        return nullptr;
    }

    CMonitor::registerClass();
    CTraverser* trav = new CTraverser("testing");

    trav->preFunc = header;
    trav->displayFunc = display;

    return trav;
}
