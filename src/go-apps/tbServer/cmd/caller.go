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
	"strings"
	"time"

	"github.com/gorilla/mux"
)

func callOne(w http.ResponseWriter, r *http.Request, tbCmd string) {
	callOneExtra(w, r, tbCmd, "")
}

func callOneExtra(w http.ResponseWriter, r *http.Request, tbCmd, extra string) {
	w.Header().Set("Content-Type", "application/json; charset=UTF-8")
	w.Header().Set("Access-Control-Allow-Origin", "*")
	w.Header().Set("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept")
	w.Header().Set("Access-Control-Allow-Methods", "PUT, POST, GET, DELETE, OPTIONS")
	w.WriteHeader(http.StatusOK)

	allDogs := []string{}
	if extra != "" {
		allDogs = append(allDogs, extra)
	}
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
	}
	cmd := exec.Command(tbCmd, allDogs...)

	if r.Header.Get("User-Agent") == "testRunner" {
		cmd.Env = append(append(os.Environ(), "TEST_MODE=true"), "API_MODE=true") 
		vars := strings.Split(r.Header.Get("X-TestRunner-Env"), "|")
		for _, v := range vars {
			cmd.Env = append(cmd.Env, v) 
            // log.Printf(v)
		}
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

// FileExists
func FileExists(filename string) bool {
	info, err := os.Stat(filename)
	if os.IsNotExist(err) {
		return false
	}
	return !info.IsDir()
}

func AccountsAbis(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "grabABI")
}

func AccountsCollections(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "ethNames")
}

func AccountsExport(w http.ResponseWriter, r *http.Request) {
	callOneExtra(w, r, "chifra", "export")
}

func AccountsList(w http.ResponseWriter, r *http.Request) {
	callOneExtra(w, r, "chifra", "list")
}

func AccountsNames(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "ethNames")
}

func AccountsRm(w http.ResponseWriter, r *http.Request) {
	callOneExtra(w, r, "chifra", "rm")
}

func AccountsTags(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "ethNames")
}

func AdminScrape(w http.ResponseWriter, r *http.Request) {
	callOneExtra(w, r, "chifra", "scrape")
}

func AdminPins(w http.ResponseWriter, r *http.Request) {
    callOne(w, r, "pinStatus");
}

func AdminStatus(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "cacheStatus")
}

func DataBlocks(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "getBlock")
}

func DataLogs(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "getLogs")
}

func DataReceipts(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "getReceipt")
}

func DataTraces(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "getTrace")
}

func DataTransactions(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "getTrans")
}

func DataWhen(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "whenBlock")
}

func OtherDive(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "turboDive")
}

func OtherQuotes(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "ethQuote")
}

func OtherSlurp(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "ethslurp")
}

func OtherWhere(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "whereBlock")
}

func StateState(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "getState")
}

func StateTokens(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "getTokenInfo")
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
