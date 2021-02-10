/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
package trueblocks

import (
	"fmt"
	"log"
	"net/http"
	"os"
	"os/exec"
	"time"

	"github.com/gorilla/mux"
)

func callOneExtra(w http.ResponseWriter, r *http.Request, tbCmd, extra string) {
	w.Header().Set("Content-Type", "application/json; charset=UTF-8")
	w.Header().Set("Access-Control-Allow-Origin", "*")
	w.Header().Set("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept")
	w.Header().Set("Access-Control-Allow-Methods", "PUT, POST, GET, DELETE, OPTIONS")
	w.WriteHeader(http.StatusOK)

	allDogs := []string{}
	mocked := false
	for key, value := range r.URL.Query() {
		if key != "addrs" &&
			key != "terms" &&
			key != "modes" &&
			key != "blocks" &&
			key != "transactions" &&
			key != "block_list" &&
			key != "mode" &&
			key != "names" &&
			key != "addrs2" {
			allDogs = append(allDogs, "--"+key)
		}
		allDogs = append(allDogs, value...)
		if (key == "mocked") {
			mocked = true
		}
	}
	if tbCmd != "chifra" {
		allDogs = append([]string{extra}, allDogs...)
	} else if mocked {
		tbCmd = "chifra"
		allDogs = append([]string{extra}, allDogs...)
	}
	cmd := exec.Command(tbCmd, allDogs...)

	if r.Header.Get("User-Agent") == "testRunner" {
		cmd.Env = append(append(os.Environ(), "TEST_MODE=true"), "API_MODE=true") 
	} else {
        cmd.Env = append(os.Environ(), "API_MODE=true")
    }
	out, err := cmd.Output()
	if err != nil {
		fmt.Printf("%s", err)
	}
	output := string(out[:])
	fmt.Fprint(w, output)
}

// FileExists returns true if the given file exists
func FileExists(filename string) bool {
	info, err := os.Stat(filename)
	if os.IsNotExist(err) {
		return false
	}
	return !info.IsDir()
}

func AccountsAbis(w http.ResponseWriter, r *http.Request) {
	callOneExtra(w, r, "grabABI", "collections")
}

func AccountsCollections(w http.ResponseWriter, r *http.Request) {
	callOneExtra(w, r, "ethNames", "names")
}

func AccountsExport(w http.ResponseWriter, r *http.Request) {
	callOneExtra(w, r, "chifra", "export")
}

func AccountsList(w http.ResponseWriter, r *http.Request) {
	callOneExtra(w, r, "chifra", "list")
}

func AccountsNames(w http.ResponseWriter, r *http.Request) {
	callOneExtra(w, r, "ethNames", "names")
}

func AccountsRm(w http.ResponseWriter, r *http.Request) {
	callOneExtra(w, r, "chifra", "rm")
}

func AccountsTags(w http.ResponseWriter, r *http.Request) {
	callOneExtra(w, r, "ethNames", "tags")
}

func AdminScrape(w http.ResponseWriter, r *http.Request) {
	callOneExtra(w, r, "chifra", "scrape")
}

func AdminPins(w http.ResponseWriter, r *http.Request) {
    callOneExtra(w, r, "pinStatus", "pins");
}

func AdminStatus(w http.ResponseWriter, r *http.Request) {
	callOneExtra(w, r, "cacheStatus", "status")
}

func DataBlocks(w http.ResponseWriter, r *http.Request) {
	callOneExtra(w, r, "getBlock", "blocks")
}

func DataLogs(w http.ResponseWriter, r *http.Request) {
	callOneExtra(w, r, "getLogs", "logs")
}

func DataReceipts(w http.ResponseWriter, r *http.Request) {
	callOneExtra(w, r, "getReceipt", "receipts")
}

func DataTraces(w http.ResponseWriter, r *http.Request) {
	callOneExtra(w, r, "getTrace", "traces")
}

func DataTransactions(w http.ResponseWriter, r *http.Request) {
	callOneExtra(w, r, "getTrans", "transactions")
}

func DataWhen(w http.ResponseWriter, r *http.Request) {
	callOneExtra(w, r, "whenBlock", "when")
}

func OtherDive(w http.ResponseWriter, r *http.Request) {
	callOneExtra(w, r, "turboDive", "dive")
}

func OtherQuotes(w http.ResponseWriter, r *http.Request) {
	callOneExtra(w, r, "ethQuote", "quotes")
}

func OtherSlurp(w http.ResponseWriter, r *http.Request) {
	callOneExtra(w, r, "ethslurp", "slurp")
}

func OtherWhere(w http.ResponseWriter, r *http.Request) {
	callOneExtra(w, r, "whereBlock", "where")
}

func StateState(w http.ResponseWriter, r *http.Request) {
	callOneExtra(w, r, "getState", "state")
}

func StateTokens(w http.ResponseWriter, r *http.Request) {
	callOneExtra(w, r, "getTokenInfo", "tokens")
}

var nProcessed int
func Logger(inner http.Handler, name string) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		start := time.Now()
		inner.ServeHTTP(w, r)
        t := ""
        if r.Header.Get("User-Agent") == "testRunner" {
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

type Response struct {
	Data *interface{} `json:"data,omitempty"`

	Error_ []string `json:"error,omitempty"`
}

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
