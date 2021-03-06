/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
package trueblocks

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

// AccountsList help text todo
func AccountsList(w http.ResponseWriter, r *http.Request) {
	CallOneExtra(w, r, "chifra", "list", "list")
}

// AccountsTags help text todo
func AccountsTags(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "ethNames", "tags")
}

// AccountsEntities help text todo
func AccountsEntities(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "ethNames", "entities")
}

// AccountsMonitor help text todo
func AccountsMonitor(w http.ResponseWriter, r *http.Request) {
	CallOneExtra(w, r, "chifra", "monitor", "monitor")
}

// BEG_ROUTE_CODE

// AccountsExport help text todo
func AccountsExport(w http.ResponseWriter, r *http.Request) {
	CallOneExtra(w, r, "chifra", "export", "export")
}

// AccountsNames help text todo
func AccountsNames(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "ethNames", "names")
}

// AccountsAbis help text todo
func AccountsAbis(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "grabABI", "abis")
}

// AdminPins help text todo
func AdminPins(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "pinMan", "pins")
}

// AdminStatus help text todo
func AdminStatus(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "cacheStatus", "status")
}

// AdminScrape help text todo
func AdminScrape(w http.ResponseWriter, r *http.Request) {
	CallOneExtra(w, r, "chifra", "scrape", "scrape")
}

// DataBlocks help text todo
func DataBlocks(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "getBlock", "blocks")
}

// DataTransactions help text todo
func DataTransactions(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "getTrans", "transactions")
}

// DataReceipts help text todo
func DataReceipts(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "getReceipt", "receipts")
}

// DataLogs help text todo
func DataLogs(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "getLogs", "logs")
}

// DataTraces help text todo
func DataTraces(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "getTrace", "traces")
}

// DataWhen help text todo
func DataWhen(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "whenBlock", "when")
}

// StateState help text todo
func StateState(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "getState", "state")
}

// StateTokens help text todo
func StateTokens(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "getTokenInfo", "tokens")
}

// OtherQuotes help text todo
func OtherQuotes(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "ethQuote", "quotes")
}

// OtherSlurp help text todo
func OtherSlurp(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "ethslurp", "slurp")
}

// OtherWhere help text todo
func OtherWhere(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "whereBlock", "where")
}

// OtherDive help text todo
func OtherDive(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "turboDive", "dive")
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

	Route{
		"AccountsList",
		"GET",
		"/list",
		AccountsList,
	},

	Route{
		"AccountsTags",
		"GET",
		"/tags",
		AccountsTags,
	},

	Route{
		"AccountsEntities",
		"GET",
		"/entities",
		AccountsEntities,
	},


	Route{
		"AccountsMonitor",
		"GET",
		"/monitor",
		AccountsMonitor,
	},

	// BEG_ROUTE_ITEMS

	Route{
		"AccountsExport",
		"GET",
		"/export",
		AccountsExport,
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
		"AdminPins",
		"GET",
		"/pins",
		AdminPins,
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
		"DataBlocks",
		"GET",
		"/blocks",
		DataBlocks,
	},

	Route{
		"DataTransactions",
		"GET",
		"/transactions",
		DataTransactions,
	},

	Route{
		"DataReceipts",
		"GET",
		"/receipts",
		DataReceipts,
	},

	Route{
		"DataLogs",
		"GET",
		"/logs",
		DataLogs,
	},

	Route{
		"DataTraces",
		"GET",
		"/traces",
		DataTraces,
	},

	Route{
		"DataWhen",
		"GET",
		"/when",
		DataWhen,
	},

	Route{
		"StateState",
		"GET",
		"/state",
		StateState,
	},

	Route{
		"StateTokens",
		"GET",
		"/tokens",
		StateTokens,
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

	Route{
		"OtherWhere",
		"GET",
		"/where",
		OtherWhere,
	},

	Route{
		"OtherDive",
		"GET",
		"/dive",
		OtherDive,
	},
	// END_ROUTE_ITEMS
}
