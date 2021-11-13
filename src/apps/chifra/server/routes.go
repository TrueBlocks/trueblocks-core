package serve

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
	"log"
	"net/http"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/server/exec"
	utils "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/gorilla/mux"
	"golang.org/x/time/rate"
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
	router.Use(FormatValidator)

	return router
}

var nProcessed int

// Logger sends information to the server's console
func Logger(inner http.Handler, name string) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		var limiter = rate.NewLimiter(1, 3)
		if !limiter.Allow() {
			http.Error(w, http.StatusText(http.StatusTooManyRequests), http.StatusTooManyRequests)
			return
		}
		start := time.Now()
		inner.ServeHTTP(w, r)
		t := ""
		if utils.IsTestModeServer(r) {
			t = "-test"
		}
		log.Printf(
			"%d %s%s %s %s %s",
			nProcessed,
			r.Method,
			t,
			r.RequestURI,
			name,
			time.Since(start),
		)
		nProcessed++
	})
}

// Index shows the home page
func Index(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "Users Manual")
}

// FormatValidator checks if the client wants a supported format
func FormatValidator(inner http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		fmtQuery := r.URL.Query()["fmt"]
		valid := false
		fmtValue := ""
		if len(fmtQuery) == 0 {
			valid = true
		} else {
			fmtValue = fmtQuery[0]
			valid = fmtValue == "" ||
				fmtValue == "json" ||
				fmtValue == "txt" ||
				fmtValue == "csv" ||
				fmtValue == "api"
		}

		if valid {
			inner.ServeHTTP(w, r)
			return
		}

		RespondWithError(
			w,
			http.StatusBadRequest,
			fmt.Errorf("The --fmt option (%s) must be one of [ json | txt | csv | api ]", fmtValue),
		)
	})
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

// AccountsAbis processes ABI queries
func AccountsAbis(w http.ResponseWriter, r *http.Request) {
	// This is temporary. Currently only --find is supported by
	// golang version and all other flags trigger the old C++ version
	// of grabABI to be called.
	if len(r.URL.Query()["find"]) == 0 {
		CallOneExtra(w, r, "chifra", "abis", "abis")
		return
	}

	result, err := exec.AccountsAbis(r)
	if err != nil {
		RespondWithError(w, http.StatusInternalServerError, err)
		return
	}

	format := r.URL.Query().Get("fmt")
	Respond(format, w, http.StatusOK, result)
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

// AdminInit help text todo
func AdminInit(w http.ResponseWriter, r *http.Request) {
	CallOneExtra(w, r, "chifra", "init", "init")
}

// AdminPins handles /pins route
func AdminPins(w http.ResponseWriter, r *http.Request) {
	result, err := exec.AdminPins(r)
	if err != nil {
		RespondWithError(w, http.StatusInternalServerError, err)
		return
	}

	format := r.URL.Query().Get("fmt")
	Respond(format, w, http.StatusOK, result)
}

// AdminChunks help text todo
func AdminChunks(w http.ResponseWriter, r *http.Request) {
	CallOne(w, r, "chunkMan", "chunks")
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

	Route{
		"EditName",
		"POST",
		"/names",
		EditName,
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
		"AdminInit",
		"GET",
		"/init",
		AdminInit,
	},

	Route{
		"AdminPins",
		"GET",
		"/pins",
		AdminPins,
	},

	Route{
		"AdminChunks",
		"GET",
		"/chunks",
		AdminChunks,
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
