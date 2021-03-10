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

	"golang.org/x/time/rate"
)

// CallOne handles a route that calls the underlying TrueBlocks tool directly
func CallOne(w http.ResponseWriter, r *http.Request, tbCmd , apiCmd string) {
	CallOneExtra(w, r, tbCmd, "", apiCmd)
}

// CallOneExtra handles a route by calling into chifra
func CallOneExtra(w http.ResponseWriter, r *http.Request, tbCmd, extra, apiCmd string) {
	
	w.Header().Set("Content-Type", "application/json; charset=UTF-8")
	w.Header().Set("Access-Control-Allow-Origin", "*")
	w.Header().Set("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept")
	w.Header().Set("Access-Control-Allow-Methods", "PUT, POST, GET, DELETE, OPTIONS")
	w.WriteHeader(http.StatusOK)

	// We build an array of options that we send along with the call...
	allDogs := []string{}
	if extra != "" {
		allDogs = append(allDogs, extra)
	}
	hasVerbose := false;
	hasRun := apiCmd == "scrape";
	for key, value := range r.URL.Query() {
		// These keys exist only in the API. We strip them here since the command line
		// tools will report them as invalid options.
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
		if key == "verbose" {
			hasVerbose = true
		}
		if apiCmd == "scrape" && key == "mode" && (value[0] == "quit" || value[0] == "pause" || value[0] == "restart") {
			hasRun = false;
		}
		allDogs = append(allDogs, value...)
	}
	if hasRun && apiCmd == "scrape" {
		log.Println("You cannot run the scrape command from the API. Quitting.");
		fmt.Fprint(w, "{\"error\": \"You cannot run the scrape command from the API. Quitting.\"}")
		return
	}
	// If the server was started with --verbose and hte command does not have --verbose...
	if Options.Verbose > 0 && !hasVerbose {
		allDogs = append(allDogs, "--verbose")
		allDogs = append(allDogs, strconv.Itoa(Options.Verbose))
	}

	// Do the actual call
	cmd := exec.Command(tbCmd, allDogs...)

	// Listen if the call gets canceled
	notify := w.(http.CloseNotifier).CloseNotify()
	go func() {
		<-notify
        pid := cmd.Process.Pid
		if err := cmd.Process.Kill(); err != nil {
			log.Println("failed to kill process: ", err)
		}
		log.Println("apiCmd: ", apiCmd)
		if (apiCmd == "scrape") {
			out, err := exec.Command("blockScrape", "quit --verbose").Output()
			if err != nil {
				fmt.Printf("%s", err)
			} else {
				log.Printf(string(out[:]))
			}
		}		
		log.Println("The client closed the connection to process id ", pid, ". Cleaning up.")
	}()

	// In regular operation, we set an environment variable API_MODE=true. When
	// testing (the test harness sends a special header) we also send the TEST_MODE=true
	// environment variable and any other vars for this particular test
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

	// We need to pass the stderr through to the command line and also pick
	// off and pass along through the web socket and progress reports
	stderrPipe, err := cmd.StderrPipe()
	if err != nil {
		log.Printf("%s", err)
	} else {
		go func() {
			ScanForProgress(stderrPipe, func(msg string) {
				connectionPool.broadcast <- &Message{
					Action: ProgressMessage,
					ID: tbCmd,
					Content: msg,
				}
			})
		}()
	}

	out, err := cmd.Output()
	if err != nil {
		log.Println(err)
		connectionPool.broadcast <- &Message{
			Action:  CommandErrorMessage,
			ID:      tbCmd,
			Content: err.Error(),
		}
	}

	output := string(out[:])
	// connectionPool.broadcast <- &Message{
	// 	Action:  CommandOutputMessage,
	// 	ID:      tbCmd,
	// 	Content: string(output),
	// }
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

var nProcessed int
// Logger sends information to the server's console
func Logger(inner http.Handler, name string) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		var limiter = rate.NewLimiter(1, 3)
		// fmt.Println("limiter.Limit: ", limiter.Limit())
		if limiter.Allow() == false {
            http.Error(w, http.StatusText(429), http.StatusTooManyRequests)
            return
        }
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
