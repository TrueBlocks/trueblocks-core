// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package daemonPkg

import (
	// EXISTING_CODE
	"errors"
	"net/http"

	// EXISTING_CODE

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
)

// Route A structure to hold the API's routes
type Route struct {
	Name        string
	Method      string
	Pattern     string
	HandlerFunc http.HandlerFunc
}

var routes = []Route{
	{"RouteList", "GET", "/list", func(w http.ResponseWriter, r *http.Request) {
		if err := listPkg.ServeList(w, r); err != nil {
			RespondWithError(w, http.StatusInternalServerError, err)
		}
	}},
	{"RouteExport", "GET", "/export", func(w http.ResponseWriter, r *http.Request) {
		if err := exportPkg.ServeExport(w, r); err != nil {
			RespondWithError(w, http.StatusInternalServerError, err)
		}
	}},
	{"RouteMonitors", "GET", "/monitors", func(w http.ResponseWriter, r *http.Request) {
		if err := monitorsPkg.ServeMonitors(w, r); err != nil {
			RespondWithError(w, http.StatusInternalServerError, err)
		}
	}},
	{"RouteNames", "GET", "/names", func(w http.ResponseWriter, r *http.Request) {
		if err := namesPkg.ServeNames(w, r); err != nil {
			RespondWithError(w, http.StatusInternalServerError, err)
		}
	}},
	{"RouteAbis", "GET", "/abis", func(w http.ResponseWriter, r *http.Request) {
		if err := abisPkg.ServeAbis(w, r); err != nil {
			RespondWithError(w, http.StatusInternalServerError, err)
		}
	}},
	{"RouteBlocks", "GET", "/blocks", func(w http.ResponseWriter, r *http.Request) {
		if err := blocksPkg.ServeBlocks(w, r); err != nil {
			RespondWithError(w, http.StatusInternalServerError, err)
		}
	}},
	{"RouteTransactions", "GET", "/transactions", func(w http.ResponseWriter, r *http.Request) {
		if err := transactionsPkg.ServeTransactions(w, r); err != nil {
			RespondWithError(w, http.StatusInternalServerError, err)
		}
	}},
	{"RouteReceipts", "GET", "/receipts", func(w http.ResponseWriter, r *http.Request) {
		if err := receiptsPkg.ServeReceipts(w, r); err != nil {
			RespondWithError(w, http.StatusInternalServerError, err)
		}
	}},
	{"RouteLogs", "GET", "/logs", func(w http.ResponseWriter, r *http.Request) {
		if err := logsPkg.ServeLogs(w, r); err != nil {
			RespondWithError(w, http.StatusInternalServerError, err)
		}
	}},
	{"RouteTraces", "GET", "/traces", func(w http.ResponseWriter, r *http.Request) {
		if err := tracesPkg.ServeTraces(w, r); err != nil {
			RespondWithError(w, http.StatusInternalServerError, err)
		}
	}},
	{"RouteWhen", "GET", "/when", func(w http.ResponseWriter, r *http.Request) {
		if err := whenPkg.ServeWhen(w, r); err != nil {
			RespondWithError(w, http.StatusInternalServerError, err)
		}
	}},
	{"RouteState", "GET", "/state", func(w http.ResponseWriter, r *http.Request) {
		if err := statePkg.ServeState(w, r); err != nil {
			RespondWithError(w, http.StatusInternalServerError, err)
		}
	}},
	{"RouteTokens", "GET", "/tokens", func(w http.ResponseWriter, r *http.Request) {
		if err := tokensPkg.ServeTokens(w, r); err != nil {
			RespondWithError(w, http.StatusInternalServerError, err)
		}
	}},
	{"RouteConfig", "GET", "/config", func(w http.ResponseWriter, r *http.Request) {
		if err := configPkg.ServeConfig(w, r); err != nil {
			RespondWithError(w, http.StatusInternalServerError, err)
		}
	}},
	{"RouteStatus", "GET", "/status", func(w http.ResponseWriter, r *http.Request) {
		if err := statusPkg.ServeStatus(w, r); err != nil {
			RespondWithError(w, http.StatusInternalServerError, err)
		}
	}},
	{"RouteScrape", "GET", "/scrape", func(w http.ResponseWriter, r *http.Request) {
		if err := scrapePkg.ServeScrape(w, r); err != nil {
			RespondWithError(w, http.StatusInternalServerError, err)
		}
	}},
	{"RouteChunks", "GET", "/chunks", func(w http.ResponseWriter, r *http.Request) {
		if err := chunksPkg.ServeChunks(w, r); err != nil {
			RespondWithError(w, http.StatusInternalServerError, err)
		}
	}},
	{"RouteInit", "GET", "/init", func(w http.ResponseWriter, r *http.Request) {
		if err := initPkg.ServeInit(w, r); err != nil {
			RespondWithError(w, http.StatusInternalServerError, err)
		}
	}},
	{"RouteExplore", "GET", "/explore", func(w http.ResponseWriter, r *http.Request) {
		if err := explorePkg.ServeExplore(w, r); err != nil {
			RespondWithError(w, http.StatusInternalServerError, err)
		}
	}},
	{"RouteSlurp", "GET", "/slurp", func(w http.ResponseWriter, r *http.Request) {
		if err := slurpPkg.ServeSlurp(w, r); err != nil {
			RespondWithError(w, http.StatusInternalServerError, err)
		}
	}},

	// EXISTING_CODE
	{"Index", "GET", "/", func(w http.ResponseWriter, r *http.Request) {
		http.Redirect(w, r, "https://trueblocks.io/docs/", http.StatusMovedPermanently)
	}},
	{"Websockets", "GET", "/websocket", func(w http.ResponseWriter, r *http.Request) {
		HandleWebsockets(connectionPool, w, r)
	}},
	{"DeleteMonitors", "DELETE", "/monitors", func(w http.ResponseWriter, r *http.Request) {
		if err := monitorsPkg.ServeMonitors(w, r); err != nil {
			RespondWithError(w, http.StatusInternalServerError, err)
		}
	}},
	{"CreateName", "POST", "/names", func(w http.ResponseWriter, r *http.Request) {
		if err := namesPkg.ServeNames(w, r); err != nil {
			RespondWithError(w, http.StatusInternalServerError, err)
		}
	}},
	{"EditName", "PUT", "/names", func(w http.ResponseWriter, r *http.Request) {
		if err := namesPkg.ServeNames(w, r); err != nil {
			RespondWithError(w, http.StatusInternalServerError, err)
		}
	}},
	{"DeleteName", "DELETE", "/names", func(w http.ResponseWriter, r *http.Request) {
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
	// EXISTING_CODE
}

// EXISTING_CODE
// By removing, inserting into, or altering any lines of code in this
// repo without preserving the license, you are violating the terms of
// our usage license. Don't do it.
// EXISTING_CODE
