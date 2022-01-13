// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package servePkg

import (
	"errors"
	"fmt"
	"log"
	"net/http"
	"os"
	"os/exec"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// CallOne handles a route by calling into chifra
func CallOne(w http.ResponseWriter, r *http.Request, tbCmd, extra, apiCmd string) {

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
				key = convertToCommandLine(key)
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
	if Options.Globals.Verbose && !hasVerbose {
		allDogs = append(allDogs, "--verbose")
	}

	// Do the actual call
	cmd := exec.Command(tbCmd, allDogs...)
	if Options.Globals.Verbose {
		log.Print(colors.Yellow, "Calling: ", cmd, colors.Off)
	}

	if cmd.Process != nil {
		// Listen if the call gets canceled
		go func() {
			<-r.Context().Done()
			pid := cmd.Process.Pid
			if err := cmd.Process.Kill(); err != nil {
				log.Println("failed to kill process: ", err)
			}
			log.Println("apiCmd: ", apiCmd)
			log.Println("The client closed the connection to process id ", pid, ". Cleaning up.")
		}()
	}

	configPath := config.GetPathToRootConfig()
	cachePath := config.GetPathToCache1(Options.Globals.Chain)
	indexPath := config.GetPathToIndex1(Options.Globals.Chain)

	configPath = strings.Replace(configPath, "mainnet/", "", -1)
	cachePath = strings.Replace(cachePath, "mainnet/", "", -1)
	indexPath = strings.Replace(indexPath, "mainnet/", "", -1)

	// In regular operation, we set an environment variable API_MODE=true. When
	// testing (the test harness sends a special header) we also set the
	// TEST_MODE=true environment variable and any other vars for this
	// particular test
	if utils.IsTestModeServer(r) {
		cmd.Env = append(append(os.Environ(), "TEST_MODE=true"), "API_MODE=true")
		vars := strings.Split(r.Header.Get("X-TestRunner-Env"), "|")
		cmd.Env = append(cmd.Env, vars...)
	} else {
		if !Options.Globals.TestMode && Options.Globals.LogLevel > 3 {
			fmt.Fprintf(os.Stderr, "%s%s%s%s%s\n", colors.Blue, colors.Bright, "CONFIG_PATH: ", configPath, colors.Off)
			fmt.Fprintf(os.Stderr, "%s%s%s%s%s\n", colors.Blue, colors.Bright, "CACHE_PATH:  ", config.GetPathToCache1(Options.Globals.Chain), colors.Off)
			fmt.Fprintf(os.Stderr, "%s%s%s%s%s\n", colors.Blue, colors.Bright, "INDEX_PATH:  ", config.GetPathToIndex1(Options.Globals.Chain), colors.Off)
		}
		cmd.Env = append(os.Environ(), "API_MODE=true")
	}
	cmd.Env = append(cmd.Env, "TB_CONFIG_PATH="+configPath)
	cmd.Env = append(cmd.Env, "TB_CACHE_PATH="+cachePath)
	cmd.Env = append(cmd.Env, "TB_INDEX_PATH="+indexPath)
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
	if strings.Contains(output, "Error:") {
		// Remove Cobra's "Error:\n" decorator
		parsed := strings.Replace(output, "Error:", "", 1)
		parsed = strings.Trim(parsed, " \n")
		// TODO: Need this to build. Probably not right
		var unused globals.GlobalOptions
		unused.TestMode = utils.IsTestModeServer(r)
		unused.Writer = w
		unused.RespondWithError(w, http.StatusBadRequest, errors.New(parsed))
		return
	}
	if strings.Contains(output, "\"errors\":") {
		w.WriteHeader(http.StatusBadRequest)
	} else {
		w.WriteHeader(http.StatusOK)
	}
	fmt.Fprint(w, output)
}
