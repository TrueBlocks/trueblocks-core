package trueblocks

/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
/*
 * Parts of this file were generated with makeClass --openapi. Edit only those parts of
 * the code outside of 'BEG_ROUTE/END_ROUTE' tags.
 */

import (
	"fmt"
	"net/http"

	"github.com/gorilla/mux"
)

// Route A structure to hold the API's routes
type Route struct {
	Name        string
	Method      string
	Pattern     string
	HandlerFunc http.HandlerFunc
}

// Routes An array of Route structures
type Routes []Route

// NewRouter Creates a new router given the routes array
func NewRouter() *mux.Router {
	router := mux.NewRouter().StrictSlash(true)
	for _, route := range routes {
		var handler http.Handler
		handler = route.HandlerFunc
		handler = Logger(handler, route.Name)
		router.
			Methods(route.Method).
			Path(route.Pattern).
			Name(route.Name).
			Handler(handler)
	}

	return router
}

// Index shows the home page
func Index(w http.ResponseWriter, r *http.Request) {
    fmt.Fprintf(w, "Users Manual")
}

// BEG_ROUTE_CODE

// AccountsList help text todo
func AccountsList(w http.ResponseWriter, r *http.Request) {
	CallOneExtra(w, r, "chifra", "list", "list")
}

// AccountsExport help text todo
func AccountsExport(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "acctExport", "export")
}

// AccountsMonitors help text todo
func AccountsMonitors(w http.ResponseWriter, r *http.Request) {
	CallOneExtra(w, r, "chifra", "monitors", "monitors")
}

// AccountsNames help text todo
func AccountsNames(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "ethNames", "names")
}

// AccountsAbis help text todo
func AccountsAbis(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "grabABI", "abis")
}

// AccountsEntities help text todo
func AccountsEntities(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "ethNames", "entities")
}

// AccountsTags help text todo
func AccountsTags(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "ethNames", "tags")
}

// ChainDataBlocks help text todo
func ChainDataBlocks(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "getBlocks", "blocks")
}

// ChainDataTransactions help text todo
func ChainDataTransactions(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "getTrans", "transactions")
}

// ChainDataReceipts help text todo
func ChainDataReceipts(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "getReceipts", "receipts")
}

// ChainDataLogs help text todo
func ChainDataLogs(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "getLogs", "logs")
}

// ChainDataTraces help text todo
func ChainDataTraces(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "getTraces", "traces")
}

// ChainDataWhen help text todo
func ChainDataWhen(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "whenBlock", "when")
}

// ChainStateState help text todo
func ChainStateState(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "getState", "state")
}

// ChainStateTokens help text todo
func ChainStateTokens(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "getTokens", "tokens")
}

// AdminStatus help text todo
func AdminStatus(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "cacheStatus", "status")
}

// AdminScrape help text todo
func AdminScrape(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "blockScrape", "scrape")
}

// AdminPins help text todo
func AdminPins(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "pinMan", "pins")
}

// OtherQuotes help text todo
func OtherQuotes(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "getQuotes", "quotes")
}

// OtherSlurp help text todo
func OtherSlurp(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "ethslurp", "slurp")
}
// END_ROUTE_CODE

var routes = Routes{
	Route{
		"Websockets",
		"GET",
		"/websocket",
		func(w http.ResponseWriter, r *http.Request) {
			HandleWebsockets(connectionPool, w, r)
		},
	},

	Route{
		"Index",
		"GET",
		"/",
		Index,
	},

	// BEG_ROUTE_ITEMS

	Route{
		"AccountsList",
		"GET",
		"/list",
		AccountsList,
	},

	Route{
		"AccountsExport",
		"GET",
		"/export",
		AccountsExport,
	},

	Route{
		"AccountsMonitors",
		"GET",
		"/monitors",
		AccountsMonitors,
	},

	Route{
		"AccountsNames",
		"GET",
		"/names",
		AccountsNames,
	},

	Route{
		"AccountsAbis",
		"GET",
		"/abis",
		AccountsAbis,
	},

	Route{
		"AccountsEntities",
		"GET",
		"/entities",
		AccountsEntities,
	},

	Route{
		"AccountsTags",
		"GET",
		"/tags",
		AccountsTags,
	},

	Route{
		"ChainDataBlocks",
		"GET",
		"/blocks",
		ChainDataBlocks,
	},

	Route{
		"ChainDataTransactions",
		"GET",
		"/transactions",
		ChainDataTransactions,
	},

	Route{
		"ChainDataReceipts",
		"GET",
		"/receipts",
		ChainDataReceipts,
	},

	Route{
		"ChainDataLogs",
		"GET",
		"/logs",
		ChainDataLogs,
	},

	Route{
		"ChainDataTraces",
		"GET",
		"/traces",
		ChainDataTraces,
	},

	Route{
		"ChainDataWhen",
		"GET",
		"/when",
		ChainDataWhen,
	},

	Route{
		"ChainStateState",
		"GET",
		"/state",
		ChainStateState,
	},

	Route{
		"ChainStateTokens",
		"GET",
		"/tokens",
		ChainStateTokens,
	},

	Route{
		"AdminStatus",
		"GET",
		"/status",
		AdminStatus,
	},

	Route{
		"AdminScrape",
		"GET",
		"/scrape",
		AdminScrape,
	},

	Route{
		"AdminPins",
		"GET",
		"/pins",
		AdminPins,
	},

	Route{
		"OtherQuotes",
		"GET",
		"/quotes",
		OtherQuotes,
	},

	Route{
		"OtherSlurp",
		"GET",
		"/slurp",
		OtherSlurp,
	},
	// END_ROUTE_ITEMS
}
