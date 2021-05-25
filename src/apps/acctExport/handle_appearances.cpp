/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
bool app_Display(CTraverser* trav, void* data) {
    COptions* opt = (COptions*)data;

    trav->nProcessed++;
    if (opt->freshenOnly)
        return true;

    CAppearanceDisplay dapp(opt->accountedFor, opt->accountedForName, trav->app->blk, trav->app->txid);
    cout << ((isJson() && !opt->firstOut) ? ", " : "");
    cout << dapp;
    opt->firstOut = false;

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
