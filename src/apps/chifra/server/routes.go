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
	"os"

	// BEG_ROUTE_PKGS
	abisPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/abis"
	blocksPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/blocks"
	chunksPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/chunks"
	exportPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/export"
	initPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/init"
	listPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/list"
	logsPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/logs"
	monitorsPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/monitors"
	namesPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/names"
	pinsPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/pins"
	quotesPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/quotes"
	receiptsPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/receipts"
	scrapePkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/scrape"
	slurpPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/slurp"
	statePkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/state"
	statusPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/status"
	tokensPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/tokens"
	tracesPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/traces"
	transactionsPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/transactions"
	whenPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/when"
	// END_ROUTE_PKGS
)

// BEG_ROUTE_CODE

// RouteList List every appearance of an address anywhere on the chain.
func RouteList(w http.ResponseWriter, r *http.Request) {
	if !listPkg.ServeList(w, r) {
		CallOne(w, r, "chifra", "list", "list")
	}
}

// RouteExport Export full detail of transactions for one or more addresses.
func RouteExport(w http.ResponseWriter, r *http.Request) {
	if !exportPkg.ServeExport(w, r) {
		CallOne(w, r, GetCommandPath("acctExport"), "", "export")
	}
}

// RouteMonitors Add, remove, clean, and list address monitors.
func RouteMonitors(w http.ResponseWriter, r *http.Request) {
	if !monitorsPkg.ServeMonitors(w, r) {
		CallOne(w, r, "chifra", "monitors", "monitors")
	}
}

// RouteNames Query addresses or names of well known accounts.
func RouteNames(w http.ResponseWriter, r *http.Request) {
	if !namesPkg.ServeNames(w, r) {
		CallOne(w, r, GetCommandPath("ethNames"), "", "names")
	}
}

// RouteAbis Fetches the ABI for a smart contract.
func RouteAbis(w http.ResponseWriter, r *http.Request) {
	if !abisPkg.ServeAbis(w, r) {
		CallOne(w, r, "chifra", "abis", "abis")
	}
}

// RouteBlocks Retrieve one or more blocks from the chain or local cache.
func RouteBlocks(w http.ResponseWriter, r *http.Request) {
	if !blocksPkg.ServeBlocks(w, r) {
		CallOne(w, r, GetCommandPath("getBlocks"), "", "blocks")
	}
}

// RouteTransactions Retrieve one or more transactions from the chain or local cache.
func RouteTransactions(w http.ResponseWriter, r *http.Request) {
	if !transactionsPkg.ServeTransactions(w, r) {
		CallOne(w, r, GetCommandPath("getTrans"), "", "transactions")
	}
}

// RouteReceipts Retrieve receipts for the given transaction(s).
func RouteReceipts(w http.ResponseWriter, r *http.Request) {
	if !receiptsPkg.ServeReceipts(w, r) {
		CallOne(w, r, GetCommandPath("getReceipts"), "", "receipts")
	}
}

// RouteLogs Retrieve logs for the given transaction(s).
func RouteLogs(w http.ResponseWriter, r *http.Request) {
	if !logsPkg.ServeLogs(w, r) {
		CallOne(w, r, GetCommandPath("getLogs"), "", "logs")
	}
}

// RouteTraces Retrieve traces for the given transaction(s).
func RouteTraces(w http.ResponseWriter, r *http.Request) {
	if !tracesPkg.ServeTraces(w, r) {
		CallOne(w, r, GetCommandPath("getTraces"), "", "traces")
	}
}

// RouteWhen Find block(s) based on date, blockNum, timestamp, or 'special'.
func RouteWhen(w http.ResponseWriter, r *http.Request) {
	if !whenPkg.ServeWhen(w, r) {
		os.Setenv("NO_SCHEMAS", "true") // temporary while porting to go
		os.Setenv("GO_PORT", "true")    // temporary while porting to go
		CallOne(w, r, GetCommandPath("whenBlock"), "", "when")
		os.Setenv("NO_SCHEMAS", "") // temporary while porting to go
		os.Setenv("GO_PORT", "")    // temporary while porting to go
	}
}

// RouteState Retrieve account balance(s) for one or more addresses at given block(s).
func RouteState(w http.ResponseWriter, r *http.Request) {
	if !statePkg.ServeState(w, r) {
		CallOne(w, r, GetCommandPath("getState"), "", "state")
	}
}

// RouteTokens Retrieve token balance(s) for one or more addresses at given block(s).
func RouteTokens(w http.ResponseWriter, r *http.Request) {
	if !tokensPkg.ServeTokens(w, r) {
		CallOne(w, r, GetCommandPath("getTokens"), "", "tokens")
	}
}

// RouteStatus Report on the status of the TrueBlocks system.
func RouteStatus(w http.ResponseWriter, r *http.Request) {
	if !statusPkg.ServeStatus(w, r) {
		CallOne(w, r, GetCommandPath("cacheStatus"), "", "status")
	}
}

// RouteScrape Scan the chain and update (and optionally pin) the TrueBlocks index of appearances.
func RouteScrape(w http.ResponseWriter, r *http.Request) {
	if !scrapePkg.ServeScrape(w, r) {
		CallOne(w, r, GetCommandPath("blockScrape"), "", "scrape")
	}
}

// RouteInit Initialize the TrueBlocks system by downloading from IPFS.
func RouteInit(w http.ResponseWriter, r *http.Request) {
	if !initPkg.ServeInit(w, r) {
		CallOne(w, r, "chifra", "init", "init")
	}
}

// RoutePins Manage pinned index of appearances and associated blooms.
func RoutePins(w http.ResponseWriter, r *http.Request) {
	pinsPkg.ServePins(w, r)
}

// RouteChunks Manage and investigate chunks and bloom filters.
func RouteChunks(w http.ResponseWriter, r *http.Request) {
	if !chunksPkg.ServeChunks(w, r) {
		CallOne(w, r, GetCommandPath("chunkMan"), "", "chunks")
	}
}

// RouteQuotes Freshen or display Ethereum price data. This tool has been deprecated.
func RouteQuotes(w http.ResponseWriter, r *http.Request) {
	if !quotesPkg.ServeQuotes(w, r) {
		CallOne(w, r, GetCommandPath("getQuotes"), "", "quotes")
	}
}

// RouteSlurp Fetch data from EtherScan for any address.
func RouteSlurp(w http.ResponseWriter, r *http.Request) {
	if !slurpPkg.ServeSlurp(w, r) {
		CallOne(w, r, GetCommandPath("ethslurp"), "", "slurp")
	}
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
	Route{"RouteList", "GET", "/list", RouteList},
	Route{"RouteExport", "GET", "/export", RouteExport},
	Route{"RouteMonitors", "GET", "/monitors", RouteMonitors},
	Route{"RouteNames", "GET", "/names", RouteNames},
	Route{"RouteAbis", "GET", "/abis", RouteAbis},
	Route{"RouteBlocks", "GET", "/blocks", RouteBlocks},
	Route{"RouteTransactions", "GET", "/transactions", RouteTransactions},
	Route{"RouteReceipts", "GET", "/receipts", RouteReceipts},
	Route{"RouteLogs", "GET", "/logs", RouteLogs},
	Route{"RouteTraces", "GET", "/traces", RouteTraces},
	Route{"RouteWhen", "GET", "/when", RouteWhen},
	Route{"RouteState", "GET", "/state", RouteState},
	Route{"RouteTokens", "GET", "/tokens", RouteTokens},
	Route{"RouteStatus", "GET", "/status", RouteStatus},
	Route{"RouteScrape", "GET", "/scrape", RouteScrape},
	Route{"RouteInit", "GET", "/init", RouteInit},
	Route{"RoutePins", "GET", "/pins", RoutePins},
	Route{"RouteChunks", "GET", "/chunks", RouteChunks},
	Route{"RouteQuotes", "GET", "/quotes", RouteQuotes},
	Route{"RouteSlurp", "GET", "/slurp", RouteSlurp},
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
