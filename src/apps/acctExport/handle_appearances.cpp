/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
extern bool app_Pre(const CTraverser* trav, void* data);
extern bool app_Display(const CTraverser* trav, void* data);
extern bool app_Post(const CTraverser* trav, void* data);
extern void app_Log(const CTraverser* trav, void* data, TraverserLog mode);
//-----------------------------------------------------------------------
bool COptions::handle_appearances(void) {
    CTraverser trav(this, cout, "appearances");
    trav.preFunc = app_Pre;
    trav.filterFunc = rangeFilter;
    trav.displayFunc = app_Display;
    trav.postFunc = app_Post;
    trav.logFunc = app_Log;

    CTraverserArray traversers;
    traversers.push_back(trav);

    CAppearanceDisplay dapp(hackAppAddr, hackAppName, NOPOS, NOPOS);
    forEveryAppearance(traversers, apps, &dapp);

    return !shouldQuit();
}

//-----------------------------------------------------------------------
bool app_Display(const CTraverser* trav, void* data) {
    const COptions* opt = trav->options;
    if (opt->freshen)
        return true;

    CAppearanceDisplay* dapp = (CAppearanceDisplay*)data;
    dapp->blockNumber = trav->app->blk;
    dapp->transactionIndex = trav->app->txid;

    cout << ((isJson() && !opt->firstOut) ? ", " : "");
    cout << dapp->Format() << endl;

    return !shouldQuit();
}

//-----------------------------------------------------------------------
void app_Log(const CTraverser* trav, void* data, TraverserLog mode) {
    if (mode == TR_END) {
        end_Log(trav, data, mode);

    } else if (mode == TR_PROGRESS) {
        // prog_Log(trav, data, mode);
    }
}

//-----------------------------------------------------------------------
const char* APP_FIELDS_ALL = "CAccountName:all";
const char* APP_FIELDS_DISP = "CAccountName:address,name";
const char* APP_FIELDS_DISP_V = "CAccountName:address,name,timestamp,date";
const char* APP_FIELDS_HIDE =
    "CAccountName:schema,deleted,showing,cname,nAppearances,lastExport,firstAppearance,latestAppearance,path,"
    "sizeInBytes";

//-----------------------------------------------------------------------
bool app_Pre(const CTraverser* trav, void* data) {
    manageFields(APP_FIELDS_ALL, false);
    manageFields(verbose ? APP_FIELDS_DISP_V : APP_FIELDS_DISP, true);
    // (*trav->logFunc)(at, data, TR_START);
    return true;
}

//-----------------------------------------------------------------------
bool app_Post(const CTraverser* trav, void* data) {
    manageFields(APP_FIELDS_ALL, true);
    manageFields(APP_FIELDS_HIDE, false);
    // (*trav->logFunc)(at, data, TR_END);
    return true;
}
