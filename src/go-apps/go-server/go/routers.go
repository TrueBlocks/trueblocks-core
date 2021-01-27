/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
package swagger

import (
	"fmt"
	"net/http"

	"github.com/gorilla/mux"
)

type Route struct {
	Name        string
	Method      string
	Pattern     string
	HandlerFunc http.HandlerFunc
}

type Routes []Route

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

func Index(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "Users Manual")
}

var routes = Routes{
	Route{
		"Index",
		"GET",
		"/",
		Index,
	},

	Route{
		"AccountsAbis",
		"GET",
		"/abis",
		AccountsAbis,
	},

	Route{
		"AccountsCollections",
		"GET",
		"/collections",
		AccountsCollections,
	},

	Route{
		"AccountsExport",
		"GET",
		"/export",
		AccountsExport,
	},

	Route{
		"AccountsList",
		"GET",
		"/list",
		AccountsList,
	},

	Route{
		"AccountsNames",
		"GET",
		"/names",
		AccountsNames,
	},

	Route{
		"AccountsRm",
		"GET",
		"/rm",
		AccountsRm,
	},

	Route{
		"AccountsTags",
		"GET",
		"/tags",
		AccountsTags,
	},

	Route{
		"AdminScrape",
		"GET",
		"/scrape",
		AdminScrape,
	},

	Route{
		"AdminStatus",
		"GET",
		"/status",
		AdminStatus,
	},

	Route{
		"DataBlocks",
		"GET",
		"/blocks",
		DataBlocks,
	},

	Route{
		"DataLogs",
		"GET",
		"/logs",
		DataLogs,
	},

	Route{
		"DataReceipts",
		"GET",
		"/receipts",
		DataReceipts,
	},

	Route{
		"DataTraces",
		"GET",
		"/traces",
		DataTraces,
	},

	Route{
		"DataTransactions",
		"GET",
		"/transactions",
		DataTransactions,
	},

	Route{
		"DataWhen",
		"GET",
		"/when",
		DataWhen,
	},

	Route{
		"OtherDive",
		"GET",
		"/dive",
		OtherDive,
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
}
