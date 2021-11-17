package servePkg

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
/*
 * Parts of this file were generated with makeClass --openapi. Edit only those parts of
 * the code outside of 'BEG_ROUTE/END_ROUTE' tags.
 */

import (
	"fmt"
	"net/http"

	blocksPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/blocks"
	namesPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/names"
	pinsPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/pins"
	receiptsPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/receipts"
	tokensPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/tokens"
)

// BEG_ROUTE_CODE

func RouteAbis(w http.ResponseWriter, r *http.Request) {
	CallOneExtra(w, r, "chifra", "abis", "abis")
	// abisPkg.ServeAbis(w, r)
}

func RouteBlocks(w http.ResponseWriter, r *http.Request) {
	// TODO: Use the blocksPkg instead
	// blocksPkg.ServeBlocks(w, r)
	opts := blocksPkg.FromRequest(w, r)
	err := opts.ValidateBlocks()
	if err != nil {
		opts.Globals.RespondWithError(w, http.StatusInternalServerError, err)
		return
	}
	CallOne(w, r, "getBlocks", "blocks")
}

func RouteChunks(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "chunkMan", "chunks")
}

func RouteExport(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "acctExport", "export")
}

func RouteInit(w http.ResponseWriter, r *http.Request) {
	CallOneExtra(w, r, "chifra", "init", "init")
}

func RouteList(w http.ResponseWriter, r *http.Request) {
	CallOneExtra(w, r, "chifra", "list", "list")
}

func RouteLogs(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "getLogs", "logs")
}

func RouteMonitors(w http.ResponseWriter, r *http.Request) {
	CallOneExtra(w, r, "chifra", "monitors", "monitors")
}

func RouteNames(w http.ResponseWriter, r *http.Request) {
	// TODO: Use the namesPkg instead
	// namesPkg.ServeNames(w, r)
	opts := namesPkg.FromRequest(w, r)
	err := opts.ValidateNames()
	if err != nil {
		opts.Globals.RespondWithError(w, http.StatusInternalServerError, err)
		return
	}
	CallOne(w, r, "ethNames", "names")
}

func RoutePins(w http.ResponseWriter, r *http.Request) {
	pinsPkg.ServePins(w, r)
}

func RouteQuotes(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "getQuotes", "quotes")
}

func RouteTransactions(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "getTrans", "transactions")
}

func RouteReceipts(w http.ResponseWriter, r *http.Request) {
	// TODO: Use the receiptsPkg instead
	// receiptPkg.ServeReceipts(w, r)
	opts := receiptsPkg.FromRequest(w, r)
	err := opts.ValidateReceipts()
	if err != nil {
		opts.Globals.RespondWithError(w, http.StatusInternalServerError, err)
		return
	}
	CallOne(w, r, "getReceipts", "receipts")
}

func RouteScrape(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "blockScrape", "scrape")
}

func RouteSlurp(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "ethslurp", "slurp")
}

func RouteState(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "getState", "state")
}

func RouteStatus(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "cacheStatus", "status")
}

func RouteTokens(w http.ResponseWriter, r *http.Request) {
	// TODO: Use the tokensPkg instead
	// tokensPkg.ServeTokens(w, r)
	opts := tokensPkg.FromRequest(w, r)
	err := opts.ValidateTokens()
	if err != nil {
		opts.Globals.RespondWithError(w, http.StatusInternalServerError, err)
		return
	}
	CallOne(w, r, "getTokens", "tokens")
}

func RouteTraces(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "getTraces", "traces")
}

func RouteWhen(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "whenBlock", "when")
}

// END_ROUTE_CODE

func Index(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "Users Manual")
}

var routes = Routes{
	Route{"Websockets", "GET", "/websocket", func(w http.ResponseWriter, r *http.Request) {
		HandleWebsockets(connectionPool, w, r)
	}},
	Route{"Index", "GET", "/", Index},
	Route{"EditName", "POST", "/names", EditName},

	// BEG_ROUTE_ITEMS
	Route{"RouteAbis", "GET", "/abis", RouteAbis},
	Route{"RouteBlocks", "GET", "/blocks", RouteBlocks},
	Route{"RouteChunks", "GET", "/chunks", RouteChunks},
	// not a route
	// Route{"RouteExplore", "GET", "/explore", RouteExplore},
	Route{"RouteExport", "GET", "/export", RouteExport},
	Route{"RouteInit", "GET", "/init", RouteInit},
	Route{"RouteList", "GET", "/list", RouteList},
	Route{"RouteLogs", "GET", "/logs", RouteLogs},
	Route{"RouteMonitors", "GET", "/monitors", RouteMonitors},
	Route{"RouteNames", "GET", "/names", RouteNames},
	Route{"RoutePins", "GET", "/pins", RoutePins},
	Route{"RouteQuotes", "GET", "/quotes", RouteQuotes},
	Route{"RouteReceipts", "GET", "/receipts", RouteReceipts},
	Route{"RouteScrape,", "GET", "/scrape", RouteScrape},
	Route{"RouteSlurp", "GET", "/slurp", RouteSlurp},
	Route{"RouteState", "GET", "/state", RouteState},
	Route{"RouteStatus", "GET", "/status", RouteStatus},
	Route{"RouteTokens", "GET", "/tokens", RouteTokens},
	Route{"RouteTraces", "GET", "/traces", RouteTraces},
	Route{"RouteTransactions", "GET", "/transactions", RouteTransactions},
	Route{"RouteWhen", "GET", "/when", RouteWhen},
	// END_ROUTE_ITEMS
}

// By removing, inserting into, or altering any of the following 10  lines
// of code, you are violating the terms of our usage license. Don't do it.
// fileName := Options.Status.CachePath + "lics/export.txt"
// if false { // !utils.FileExists(fileName)) {
//	w.Header().Set("Content-Type", "application/json; charset=UTF-8")
//	w.Header().Set("Access-Control-Allow-Origin", "*")
//	w.Header().Set("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept")
//	w.Header().Set("Access-Control-Allow-Methods", "PUT, POST, GET, DELETE, OPTIONS")
//	w.WriteHeader(http.StatusOK)
//	fmt.Fprint(w, "In order to use the 'export' route, you must license the module. Quitting")
//	return
// }
// By removing, inserting into, or altering any of the above 10 uncommented
// lines of code, you are violating the terms of our usage license. Don't do it.
