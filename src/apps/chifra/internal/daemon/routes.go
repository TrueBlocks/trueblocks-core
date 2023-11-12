// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --openapi. Edit only those parts of
 * the code outside of 'BEG_ROUTE/END_ROUTE' tags.
 */

package daemonPkg

import (
	"encoding/json"
	"errors"
	"fmt"
	"net/http"
	"time"

	// BEG_ROUTE_PKGS
 
	abisPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/abis"
	blocksPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/blocks"
	chunksPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/chunks"
	configPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/config"
	explorePkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/explore"
	exportPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/export"
	initPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/init"
	listPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/list"
	logsPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/logs"
	monitorsPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/monitors"
	namesPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/names"
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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/gorilla/mux"
	"golang.org/x/time/rate"
)

// BEG_ROUTE_CODE

// RouteList List every appearance of an address anywhere on the chain.
func RouteList(w http.ResponseWriter, r *http.Request) {
	if err := listPkg.ServeList(w, r); err != nil {
		RespondWithError(w, http.StatusInternalServerError, err)
	}
}

// RouteExport Export full details of transactions for one or more addresses.
func RouteExport(w http.ResponseWriter, r *http.Request) {
	if err := exportPkg.ServeExport(w, r); err != nil {
		RespondWithError(w, http.StatusInternalServerError, err)
	}
}

// RouteMonitors Add, remove, clean, and list address monitors.
func RouteMonitors(w http.ResponseWriter, r *http.Request) {
	if err := monitorsPkg.ServeMonitors(w, r); err != nil {
		RespondWithError(w, http.StatusInternalServerError, err)
	}
}

// RouteNames Query addresses or names of well-known accounts.
func RouteNames(w http.ResponseWriter, r *http.Request) {
	if err := namesPkg.ServeNames(w, r); err != nil {
		RespondWithError(w, http.StatusInternalServerError, err)
	}
}

// RouteAbis Fetches the ABI for a smart contract.
func RouteAbis(w http.ResponseWriter, r *http.Request) {
	if err := abisPkg.ServeAbis(w, r); err != nil {
		RespondWithError(w, http.StatusInternalServerError, err)
	}
}

// RouteBlocks Retrieve one or more blocks from the chain or local cache.
func RouteBlocks(w http.ResponseWriter, r *http.Request) {
	if err := blocksPkg.ServeBlocks(w, r); err != nil {
		RespondWithError(w, http.StatusInternalServerError, err)
	}
}

// RouteTransactions Retrieve one or more transactions from the chain or local cache.
func RouteTransactions(w http.ResponseWriter, r *http.Request) {
	if err := transactionsPkg.ServeTransactions(w, r); err != nil {
		RespondWithError(w, http.StatusInternalServerError, err)
	}
}

// RouteReceipts Retrieve receipts for the given transaction(s).
func RouteReceipts(w http.ResponseWriter, r *http.Request) {
	if err := receiptsPkg.ServeReceipts(w, r); err != nil {
		RespondWithError(w, http.StatusInternalServerError, err)
	}
}

// RouteLogs Retrieve logs for the given transaction(s).
func RouteLogs(w http.ResponseWriter, r *http.Request) {
	if err := logsPkg.ServeLogs(w, r); err != nil {
		RespondWithError(w, http.StatusInternalServerError, err)
	}
}

// RouteTraces Retrieve traces for the given transaction(s).
func RouteTraces(w http.ResponseWriter, r *http.Request) {
	if err := tracesPkg.ServeTraces(w, r); err != nil {
		RespondWithError(w, http.StatusInternalServerError, err)
	}
}

// RouteWhen Find block(s) based on date, blockNum, timestamp, or 'special'.
func RouteWhen(w http.ResponseWriter, r *http.Request) {
	if err := whenPkg.ServeWhen(w, r); err != nil {
		RespondWithError(w, http.StatusInternalServerError, err)
	}
}

// RouteState Retrieve account balance(s) for one or more addresses at given block(s).
func RouteState(w http.ResponseWriter, r *http.Request) {
	if err := statePkg.ServeState(w, r); err != nil {
		RespondWithError(w, http.StatusInternalServerError, err)
	}
}

// RouteTokens Retrieve token balance(s) for one or more addresses at given block(s).
func RouteTokens(w http.ResponseWriter, r *http.Request) {
	if err := tokensPkg.ServeTokens(w, r); err != nil {
		RespondWithError(w, http.StatusInternalServerError, err)
	}
}

// RouteConfig Report on and edit the configuration of the TrueBlocks system.
func RouteConfig(w http.ResponseWriter, r *http.Request) {
	if err := configPkg.ServeConfig(w, r); err != nil {
		RespondWithError(w, http.StatusInternalServerError, err)
	}
}

// RouteStatus Report on the state of the internal binary caches.
func RouteStatus(w http.ResponseWriter, r *http.Request) {
	if err := statusPkg.ServeStatus(w, r); err != nil {
		RespondWithError(w, http.StatusInternalServerError, err)
	}
}

// RouteScrape Scan the chain and update the TrueBlocks index of appearances.
func RouteScrape(w http.ResponseWriter, r *http.Request) {
	if err := scrapePkg.ServeScrape(w, r); err != nil {
		RespondWithError(w, http.StatusInternalServerError, err)
	}
}

// RouteChunks Manage, investigate, and display the Unchained Index.
func RouteChunks(w http.ResponseWriter, r *http.Request) {
	if err := chunksPkg.ServeChunks(w, r); err != nil {
		RespondWithError(w, http.StatusInternalServerError, err)
	}
}

// RouteInit Initialize the TrueBlocks system by downloading the Unchained Index from IPFS.
func RouteInit(w http.ResponseWriter, r *http.Request) {
	if err := initPkg.ServeInit(w, r); err != nil {
		RespondWithError(w, http.StatusInternalServerError, err)
	}
}

// RouteExplore Open a local or remote explorer for one or more addresses, blocks, or transactions.
func RouteExplore(w http.ResponseWriter, r *http.Request) {
	if err := explorePkg.ServeExplore(w, r); err != nil {
		RespondWithError(w, http.StatusInternalServerError, err)
	}
}

// RouteSlurp Fetch data from Etherscan for any address.
func RouteSlurp(w http.ResponseWriter, r *http.Request) {
	if err := slurpPkg.ServeSlurp(w, r); err != nil {
		RespondWithError(w, http.StatusInternalServerError, err)
	}
}
// END_ROUTE_CODE

func Index(w http.ResponseWriter, r *http.Request) {
	http.Redirect(w, r, "https://trueblocks.io/docs/", http.StatusMovedPermanently)
}

var routes = Routes{
	Route{"Websockets", "GET", "/websocket", func(w http.ResponseWriter, r *http.Request) {
		HandleWebsockets(connectionPool, w, r)
	}},
	Route{"Index", "GET", "/", Index},
	Route{"CreateName", "POST", "/names", func(w http.ResponseWriter, r *http.Request) {
		if err := namesPkg.ServeNames(w, r); err != nil {
			RespondWithError(w, http.StatusInternalServerError, err)
		}
	}},
	Route{"EditName", "PUT", "/names", func(w http.ResponseWriter, r *http.Request) {
		if err := namesPkg.ServeNames(w, r); err != nil {
			RespondWithError(w, http.StatusInternalServerError, err)
		}
	}},
	Route{"DeleteName", "DELETE", "/names", func(w http.ResponseWriter, r *http.Request) {
		params := r.URL.Query()
		if !params.Has("delete") && !params.Has("undelete") && !params.Has("remove") {
			RespondWithError(
				w,
				http.StatusBadRequest,
				errors.New("one of following parameters is required: delete, undelete, remove"),
			)
			return
		}

		if err := namesPkg.ServeNames(w, r); err != nil {
			RespondWithError(w, http.StatusInternalServerError, err)
		}
	}},

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
	Route{"RouteConfig", "GET", "/config", RouteConfig},
	Route{"RouteStatus", "GET", "/status", RouteStatus},
	Route{"RouteScrape", "GET", "/scrape", RouteScrape},
	Route{"RouteChunks", "GET", "/chunks", RouteChunks},
	Route{"RouteInit", "GET", "/init", RouteInit},
	Route{"RouteExplore", "GET", "/explore", RouteExplore},
	Route{"RouteSlurp", "GET", "/slurp", RouteSlurp},
	// END_ROUTE_ITEMS
	Route{"DeleteMonitors", "DELETE", "/monitors", RouteMonitors},
}

// By removing, inserting into, or altering any lines of code in this
// repo without preserving the license, you are violating the terms of
// our usage license. Don't do it.

// RespondWithError marshals an err into JSON and returns the bytes
// back to the caller httpStatus HTTP error status code
func RespondWithError(w http.ResponseWriter, httpStatus int, err error) {
	type ErrorResponse struct {
		Errors []string `json:"errors,omitempty"`
	}
	marshalled, _ := json.MarshalIndent(ErrorResponse{Errors: []string{err.Error()}}, "", "  ")
	w.WriteHeader(httpStatus)
	_, _ = w.Write(marshalled)
}

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
	router.Use(CorsHandler)
	router.
		Methods("OPTIONS").
		Handler(OptionsHandler)
	router.Use(ContentTypeHandler)

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

func addCorsHeaders(w http.ResponseWriter) {
	w.Header().Set("Access-Control-Allow-Origin", "*")
	w.Header().Set("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept")
	w.Header().Set("Access-Control-Allow-Methods", "PUT, POST, GET, DELETE, OPTIONS")
}

var OptionsHandler = http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
	addCorsHeaders(w)
})

// CorsHandler handles CORS requests
func CorsHandler(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		addCorsHeaders(w)
		next.ServeHTTP(w, r)
	})
}

// ContentTypeHandler sets correct Content-Type header on response
func ContentTypeHandler(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		requestedFormat := r.URL.Query().Get("fmt")

		var contentType string
		switch requestedFormat {
		case "txt":
			contentType = "text/plain"
		case "csv":
			contentType = "text/csv"
		default:
			contentType = "application/json"
		}

		w.Header().Set("Content-Type", contentType)
		next.ServeHTTP(w, r)
	})
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
		if isTestModeServer(r) {
			t = "-test"
		}
		msg := fmt.Sprintf("%d %s%s %s %s %s", nProcessed, r.Method, t, r.RequestURI, name, time.Since(start))
		logger.Info(msg)
		nProcessed++
	})
}

// isTestModeServer return true if we are running from the testing harness
func isTestModeServer(r *http.Request) bool {
	return r.Header.Get("User-Agent") == "testRunner"
}
