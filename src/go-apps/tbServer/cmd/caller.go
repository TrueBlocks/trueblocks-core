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
	"strconv"
	"strings"
	"time"

	"github.com/gorilla/mux"
)

type optionsType struct {
	Verbose int
	Port string
}
// Options command line options
var Options optionsType
 
func callOne(w http.ResponseWriter, r *http.Request, tbCmd , apiCmd string) {
	callOneExtra(w, r, tbCmd, "", apiCmd)
}

func callOneExtra(w http.ResponseWriter, r *http.Request, tbCmd, extra, apiCmd string) {
	w.Header().Set("Content-Type", "application/json; charset=UTF-8")
	w.Header().Set("Access-Control-Allow-Origin", "*")
	w.Header().Set("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept")
	w.Header().Set("Access-Control-Allow-Methods", "PUT, POST, GET, DELETE, OPTIONS")
	w.WriteHeader(http.StatusOK)

	allDogs := []string{}
	if extra != "" {
		allDogs = append(allDogs, extra)
	}
	hasVerbose := false;
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
		if (key == "verbose") {
			hasVerbose = true
		}
		allDogs = append(allDogs, value...)
	}
	if Options.Verbose > 0 && !hasVerbose {
		allDogs = append(allDogs, "--verbose")
		allDogs = append(allDogs, strconv.Itoa(Options.Verbose))
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

	stderrPipe, err := cmd.StderrPipe()

	if err != nil {
		log.Println(err)
	} else {
		go func() {
			ScanForProgress(stderrPipe, func(commandProgress *CommandProgress) {
				connectionPool.broadcast <- &Message{
					Action: ProgressMessage,
					// TODO: this should be tbCmd, but current frontend (Explorer) does not support
					// ids other than "export"
					Id:       "export",
					Progress: commandProgress,
				}
			})
		}()
	}

	out, err := cmd.Output()

	if err != nil {
		log.Println(err)
		connectionPool.broadcast <- &Message{
			Action:  CommandErrorMessage,
			Id:      tbCmd,
			Content: err.Error(),
		}
	}

	output := string(out[:])
	connectionPool.broadcast <- &Message{
		Action:  CommandOutputMessage,
		Id:      tbCmd,
		Content: string(output),
	}
	fmt.Fprint(w, output)
}

// FileExists help text todo
func FileExists(filename string) bool {
	info, err := os.Stat(filename)
	if os.IsNotExist(err) {
		return false
	}
	return !info.IsDir()
}

// AccountsAbis help text todo
func AccountsAbis(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "grabABI", "abis")
}

// AccountsEntities help text todo
func AccountsEntities(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "ethNames", "entities")
}

// AccountsExport help text todo
func AccountsExport(w http.ResponseWriter, r *http.Request) {
	callOneExtra(w, r, "chifra", "export", "export")
}

// AccountsMonitor help text todo
func AccountsMonitor(w http.ResponseWriter, r *http.Request) {
	callOneExtra(w, r, "chifra", "monitor", "monitor")
}

// AccountsList help text todo
func AccountsList(w http.ResponseWriter, r *http.Request) {
	callOneExtra(w, r, "chifra", "list", "list")
}

// AccountsNames help text todo
func AccountsNames(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "ethNames", "names")
}

// AccountsTags help text todo
func AccountsTags(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "ethNames", "tags")
}

// AdminScrape help text todo
func AdminScrape(w http.ResponseWriter, r *http.Request) {
	callOneExtra(w, r, "chifra", "scrape", "scrape")
}

// AdminPins help text todo
func AdminPins(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "pinMan", "pins")
}

// AdminStatus help text todo
func AdminStatus(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "cacheStatus", "status")
}

// DataBlocks help text todo
func DataBlocks(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "getBlock", "blocks")
}

// DataLogs help text todo
func DataLogs(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "getLogs", "logs")
}

// DataReceipts help text todo
func DataReceipts(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "getReceipt", "receipts")
}

// DataTraces help text todo
func DataTraces(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "getTrace", "traces")
}

// DataTransactions help text todo
func DataTransactions(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "getTrans", "transactions")
}

// DataWhen help text todo
func DataWhen(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "whenBlock", "when")
}

// OtherDive help text todo
func OtherDive(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "turboDive", "dive")
}

// OtherQuotes help text todo
func OtherQuotes(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "ethQuote", "quotes")
}

// OtherSlurp help text todo
func OtherSlurp(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "ethslurp", "slurp")
}

// OtherWhere help text todo
func OtherWhere(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "whereBlock", "where")
}

// StateState help text todo
func StateState(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "getState", "state")
}

// StateTokens help text todo
func StateTokens(w http.ResponseWriter, r *http.Request) {
	callOne(w, r, "getTokenInfo", "tokens")
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

var connectionPool = newConnectionPool()

func RunWebsocketPool() {
	go connectionPool.run()
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
		"AccountsExport",
		"GET",
		"/export",
		AccountsExport,
	},

	Route{
		"AccountsMonitor",
		"GET",
		"/monitor",
		AccountsMonitor,
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
