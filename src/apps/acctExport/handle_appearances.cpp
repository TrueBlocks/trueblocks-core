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

//-----------------------------------------------------------------------
bool app_Display(CTraverser* trav, void* data) {
    COptions* opt = (COptions*)data;

    CAppearanceDisplay dapp(opt->accountedFor.address, opt->accountedFor.name, trav->app->blk, trav->app->txid);
    cout << ((isJson() && !opt->firstOut) ? ", " : "");
    cout << dapp;
    opt->firstOut = false;

    return !shouldQuit();
}

//-----------------------------------------------------------------------
const char* APP_FIELDS_ALL = "CAccountName:all";
const char* APP_FIELDS_HIDE = "CAccountName:schema,deleted,showing,cname";
//-----------------------------------------------------------------------
bool app_Post(CTraverser* trav, void* data) {
    // TODO(tjayrush): Oddly, we show different fields with 'appearances' for account names than we do for other
    // exports. Why?
    manageFields(APP_FIELDS_ALL, true);
    manageFields(APP_FIELDS_HIDE, false);
    end_Log(trav, data);
    return true;
}
