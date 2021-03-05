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
)

func CallOne(w http.ResponseWriter, r *http.Request, tbCmd , apiCmd string) {
	CallOneExtra(w, r, tbCmd, "", apiCmd)
}

func CallOneExtra(w http.ResponseWriter, r *http.Request, tbCmd, extra, apiCmd string) {
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
