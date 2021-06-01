/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-----------------------------------------------------------------------
bool app_Display(CTraverser* trav, void* data) {
    COptions* opt = (COptions*)data;

    CAppearanceDisplay dapp(opt->accountedFor, opt->accountedForName, trav->app->blk, trav->app->txid);
    cout << ((isJson() && !opt->firstOut) ? ", " : "");
    cout << dapp;
    opt->firstOut = false;

    return !shouldQuit();
}

//-----------------------------------------------------------------------
const char* APP_FIELDS_ALL = "CAccountName:all";
const char* APP_FIELDS_HIDE =
    "CAccountName:schema,deleted,showing,cname,nAppearances,lastExport,firstAppearance,latestAppearance,path,"
    "sizeInBytes";
//-----------------------------------------------------------------------
bool app_Post(CTraverser* trav, void* data) {
    // TODO(tjayrush): Oddly, we show different fields with 'appearances' for account names than we do for other
    // exports. Why?
    manageFields(APP_FIELDS_ALL, true);
    manageFields(APP_FIELDS_HIDE, false);
    end_Log(trav, data);
    return true;
}
