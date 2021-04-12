/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
extern bool app_Pre(CTraverser* trav, void* data);
extern bool app_Display(CTraverser* trav, void* data);
extern bool app_Post(CTraverser* trav, void* data);
//-----------------------------------------------------------------------
bool COptions::handle_appearances(void) {
    CTraverser trav(this, cout, "appearances");
    trav.preFunc = app_Pre;
    trav.displayFunc = app_Display;
    trav.postFunc = app_Post;
    trav.dataFunc = noopFunc;

    CTraverserArray traversers;
    traversers.push_back(trav);

    CAppearanceDisplay dapp(hackAppAddr, hackAppName, NOPOS, NOPOS);
    forEveryAppearance(traversers, apps, &dapp);

    return !shouldQuit();
}

//-----------------------------------------------------------------------
bool app_Display(CTraverser* trav, void* data) {
    COptions* opt = (COptions*)trav->options;

    trav->nProcessed++;
    if (opt->freshen)
        return true;

    trav->inCache = true;
    CAppearanceDisplay* dapp = (CAppearanceDisplay*)data;
    dapp->blockNumber = trav->app->blk;
    dapp->transactionIndex = trav->app->txid;

    cout << ((isJson() && !opt->firstOut) ? ", " : "");
    cout << dapp->Format() << endl;
    opt->firstOut = false;

    prog_Log(trav, data, trav->inCache ? TR_PROGRESS_CACHE : TR_PROGRESS_NODE);
    return !shouldQuit();
}

//-----------------------------------------------------------------------
const char* APP_FIELDS_ALL = "CAccountName:all";
const char* APP_FIELDS_DISP = "CAccountName:address,name";
const char* APP_FIELDS_DISP_V = "CAccountName:address,name,timestamp,date";
const char* APP_FIELDS_HIDE =
    "CAccountName:schema,deleted,showing,cname,nAppearances,lastExport,firstAppearance,latestAppearance,path,"
    "sizeInBytes";

//-----------------------------------------------------------------------
bool app_Pre(CTraverser* trav, void* data) {
    manageFields(APP_FIELDS_ALL, false);
    manageFields(verbose ? APP_FIELDS_DISP_V : APP_FIELDS_DISP, true);

    start_Log(trav, data);
    return true;
}

//-----------------------------------------------------------------------
bool app_Post(CTraverser* trav, void* data) {
    manageFields(APP_FIELDS_ALL, true);
    manageFields(APP_FIELDS_HIDE, false);

    end_Log(trav, data);
    return true;
}
