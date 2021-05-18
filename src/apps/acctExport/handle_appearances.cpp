/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

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
    cout << *dapp;
    opt->firstOut = false;

    // prog_Log(trav, data);
    return !shouldQuit();
}

extern const char* APP_FIELDS_ALL;
extern const char* APP_FIELDS_HIDE;
//-----------------------------------------------------------------------
bool app_Post(CTraverser* trav, void* data) {
    manageFields(APP_FIELDS_ALL, true);
    manageFields(APP_FIELDS_HIDE, false);

    end_Log(trav, data);
    return true;
}

//-----------------------------------------------------------------------
const char* APP_FIELDS_ALL = "CAccountName:all";
const char* APP_FIELDS_HIDE =
    "CAccountName:schema,deleted,showing,cname,nAppearances,lastExport,firstAppearance,latestAppearance,path,"
    "sizeInBytes";
