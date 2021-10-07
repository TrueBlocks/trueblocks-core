package server

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

import (
	"fmt"
	"log"
	"net/http"
	"os"
	"os/exec"
	"os/user"
	"strconv"
	"strings"

	utils "github.com/TrueBlocks/trueblocks-core/src/go-apps/blaze/utils"
)

// isTestMode return true if we are running from the testing harness
func isTestMode(r *http.Request) bool {
	return r.Header.Get("User-Agent") == "testRunner"
}

// GetCommandPath returns full path the the given tool
func GetCommandPath(cmd string) string {
	usr, _ := user.Current()
	dir := usr.HomeDir
	return dir + "/.local/bin/chifra/" + cmd
}

// CallOne handles a route that calls the underlying TrueBlocks tool directly
func CallOne(w http.ResponseWriter, r *http.Request, tbCmd, apiCmd string) {
	CallOneExtra(w, r, GetCommandPath(tbCmd), "", apiCmd)
}

// CallOneExtra handles a route by calling into chifra
func CallOneExtra(w http.ResponseWriter, r *http.Request, tbCmd, extra, apiCmd string) {

	// We build an array of options that we send along with the call...
	allDogs := []string{}
	if extra != "" {
		allDogs = append(allDogs, extra)
	}
	hasVerbose := false
	for key, value := range r.URL.Query() {
		if len(value) > 0 && value[0] != "false" {
			// These keys exist only in the API. We strip them here since the command line
			// tools will report them as invalid options.
			if key != "addrs" &&
				key != "terms" &&
				key != "modes" &&
				key != "blocks" &&
				key != "transactions" &&
				key != "mode" &&
				key != "topics" &&
				key != "fourbytes" &&
				key != "names" &&
				key != "addrs2" {
				allDogs = append(allDogs, "--"+key)
			}
			if key == "verbose" {
				hasVerbose = true
			}
			if len(value) > 1 || value[0] != "true" {
				allDogs = append(allDogs, value...)
			}
		}
	}

	// If the server was started with --verbose and the command does not have --verbose...
	if Options.Verbose > 0 && !hasVerbose {
		allDogs = append(allDogs, "--verbose")
		allDogs = append(allDogs, strconv.Itoa(Options.Verbose))
	}

	// Do the actual call
	cmd := exec.Command(tbCmd, allDogs...)
	if Options.Verbose > 0 {
		log.Print(utils.Yellow, "Calling: ", cmd, utils.Off)
	}

	if cmd.Process != nil {
		// Listen if the call gets canceled
		notify := w.(http.CloseNotifier).CloseNotify()
		go func() {
			<-notify
			pid := cmd.Process.Pid
			if err := cmd.Process.Kill(); err != nil {
				log.Println("failed to kill process: ", err)
			}
			log.Println("apiCmd: ", apiCmd)
			log.Println("The client closed the connection to process id ", pid, ". Cleaning up.")
		}()
	}

	// In regular operation, we set an environment variable API_MODE=true. When
	// testing (the test harness sends a special header) we also set the
	// TEST_MODE=true environment variable and any other vars for this
	// particular test
	if isTestMode(r) {
		cmd.Env = append(append(os.Environ(), "TEST_MODE=true"), "API_MODE=true")
		vars := strings.Split(r.Header.Get("X-TestRunner-Env"), "|")
		for _, v := range vars {
			cmd.Env = append(cmd.Env, v)
			// log.Printf(v)
		}
	} else {
		cmd.Env = append(os.Environ(), "API_MODE=true")
	}
	cmd.Env = append(cmd.Env, "PROG_NAME=chifra "+apiCmd)

	// We need to pass the stderr through to the command line and also pick
	// off and pass along through the web socket and progress reports
	stderrPipe, err := cmd.StderrPipe()
	if err != nil {
		log.Printf("%s", err)
	} else {
		go func() {
			ScanForProgress(stderrPipe, func(msg string) {
				connectionPool.broadcast <- &Message{
					Action:  ProgressMessage,
					ID:      tbCmd,
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
	w.Header().Set("Content-Type", "application/json; charset=UTF-8")
	w.Header().Set("Access-Control-Allow-Origin", "*")
	w.Header().Set("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept")
	w.Header().Set("Access-Control-Allow-Methods", "PUT, POST, GET, DELETE, OPTIONS")
	if strings.Contains(output, "\"errors\":") {
		w.WriteHeader(http.StatusBadRequest)
	} else {
		w.WriteHeader(http.StatusOK)
	}
	fmt.Fprint(w, output)
}
