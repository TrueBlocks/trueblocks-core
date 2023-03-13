// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated with makeClass --gocmds. Edit only those parts of
 * the code outside of 'BEG_/END_' tags.
 */
package daemonPkg

import (
	"bufio"
	"bytes"
	"errors"
	"fmt"
	"io"
	"net/http"
	"os"
	"os/exec"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient/ens"
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

	chain := config.GetDefaultChain()
	for key, value := range r.URL.Query() {
		if key == "chain" {
			chain = value[0]
		} else {
			if len(value) > 0 && value[0] != "false" {
				// These keys exist only in the API. We strip them here since
				// the command line tools will report them as invalid options.
				if key != "addrs" &&
					key != "terms" &&
					key != "modes" &&
					key != "blocks" &&
					key != "transactions" &&
					key != "mode" &&
					key != "topics" &&
					key != "fourbytes" &&
					key != "names" {
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
	}

	GetOptions().Globals.Chain = chain
	if tbCmd == "chifra" {
		allDogs = append(allDogs, "--chain")
		allDogs = append(allDogs, chain)
	}

	// If the server was started with --verbose and the command does not have --verbose...
	if GetOptions().Globals.Verbose && !hasVerbose {
		allDogs = append(allDogs, "--verbose")
		allDogs = append(allDogs, "--log_level")
		allDogs = append(allDogs, "4")
		GetOptions().Globals.LogLevel = 4
	}

	allDogs, _ = ens.ConvertEns(chain, allDogs)

	// Do the actual call
	cmd := exec.Command(tbCmd, allDogs...)
	if GetOptions().Globals.Verbose {
		logger.Log(logger.Info, colors.Yellow, "Calling: ", cmd, colors.Off)
	}

	if cmd.Process != nil {
		// Listen if the call gets canceled
		go func() {
			<-r.Context().Done()
			pid := cmd.Process.Pid
			if err := cmd.Process.Kill(); err != nil {
				logger.Error("failed to kill process: ", err)
			}
			logger.Log(logger.Info, "apiCmd: ", apiCmd)
			logger.Log(logger.Info, "The client closed the connection to process id ", pid, ". Cleaning up.")
		}()
	}

	var env config.ConfigEnv
	env.Chain = chain
	env.ConfigPath = config.GetPathToRootConfig()
	env.CachePath = config.GetPathToCache(env.Chain)
	env.ChainConfigPath = config.GetPathToChainConfig(env.Chain) // order matters
	env.IndexPath = config.GetPathToIndex(env.Chain)             // order matters
	env.DefaultChain = config.GetDefaultChain()
	env.RpcProvider = config.GetRpcProvider(env.Chain)
	envStr := env.ToCSV()

	if utils.IsTestModeServer(r) {
		// In regular operation, we set an environment variable CPP_API_MODE=true. When
		// testing (the test harness sends a special header) we also set the
		// TEST_MODE=true environment variable and any other vars for this
		// particular test
		cmd.Env = append(append(os.Environ(), "TEST_MODE=true"), "CPP_API_MODE=true")
		vars := strings.Split(r.Header.Get("X-TestRunner-Env"), "|")
		cmd.Env = append(cmd.Env, vars...)
	} else {
		cmd.Env = append(os.Environ(), "CPP_API_MODE=true")
	}
	cmd.Env = append(cmd.Env, "TB_CONFIG_ENV="+envStr)
	cmd.Env = append(cmd.Env, "PROG_NAME=chifra "+apiCmd)

	// We need to pass the stderr through to the command line and also pick
	// off and pass along through the web socket and progress reports
	stderrPipe, err := cmd.StderrPipe()
	if err != nil {
		logger.Error(err)
	} else {
		go func() {
			scanForProgress(stderrPipe, func(msg string) {
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
		logger.Error(err)
		connectionPool.broadcast <- &Message{
			Action:  CommandErrorMessage,
			ID:      tbCmd,
			Content: err.Error(),
		}
	}

	outp := string(out[:])
	// connectionPool.broadcast <- &Message{
	// 	Action:  CommandOutputMessage,
	// 	ID:      tbCmd,
	// 	Content: string(outp),
	// }
	w.Header().Set("Content-Type", "application/json; charset=UTF-8")
	if strings.Contains(outp, "Error:") {
		// Remove Cobra's "Error:\n" decorator
		parsed := strings.Replace(outp, "Error:", "", 1)
		parsed = strings.Trim(parsed, " \n")
		var unused globals.GlobalOptions
		unused.Chain = chain
		unused.TestMode = utils.IsTestModeServer(r)
		unused.Writer = w
		RespondWithError(w, http.StatusBadRequest, errors.New(parsed))
		return
	}
	if strings.Contains(outp, "\"errors\":") {
		w.WriteHeader(http.StatusBadRequest)
	} else {
		w.WriteHeader(http.StatusOK)
	}
	fmt.Fprint(w, outp)
}

// TODO: Once we're fully ported to Go, this can go away when CallOne and PassItOn goes away
// TODO: camelCase vs. snake_case - issue #1974
func convertToCommandLine(in string) string {
	// BEG_CONVERT_CODE
	switch in {
	case "accountFor":
		return "account_for"
	case "allowMissing":
		return "allow_missing"
	case "appsPerChunk":
		return "apps_per_chunk"
	case "bigRange":
		return "big_range"
	case "blockCnt":
		return "block_cnt"
	case "byAcct":
		return "by_acct"
	case "cacheTraces":
		return "cache_traces"
	case "channelCount":
		return "channel_count"
	case "firstBlock":
		return "first_block"
	case "firstRecord":
		return "first_record"
	case "firstSnap":
		return "first_snap"
	case "lastBlock":
		return "last_block"
	case "listCount":
		return "list_count"
	case "logLevel":
		return "log_level"
	case "matchCase":
		return "match_case"
	case "maxRecords":
		return "max_records"
	case "noHeader":
		return "no_header"
	case "noZero":
		return "no_zero"
	case "proxyFor":
		return "proxy_for"
	case "skipDdos":
		return "skip_ddos"
	case "snapToGrid":
		return "snap_to_grid"
	case "startBlock":
		return "start_block"
	case "toCustom":
		return "to_custom"
	case "unripeDist":
		return "unripe_dist"
	}
	// END_CONVERT_CODE
	return in
}

// dropNL drops new line characters (\n) from the progress stream
func dropNL(data []byte) []byte {
	if len(data) > 0 && data[len(data)-1] == '\n' {
		return data[0 : len(data)-1]
	}
	return data
}

// scanProgressLine looks for "lines" that end with `\r` not `\n` like usual
func scanProgressLine(data []byte, atEOF bool) (advance int, token []byte, err error) {
	if atEOF && len(data) == 0 {
		return 0, nil, nil
	}
	if i := bytes.IndexByte(data, '\r'); i >= 0 {
		return i + 1, dropNL(data[0:i]), nil
	}
	return bufio.ScanLines(data, atEOF)
}

// scanForProgress watches stderr and picks of progress messages
func scanForProgress(stderrPipe io.Reader, fn func(string)) {
	scanner := bufio.NewScanner(stderrPipe)
	buf := make([]byte, 1024*1024)
	scanner.Buffer(buf, 1024*1024)
	scanner.Split(scanProgressLine)
	for scanner.Scan() {
		text := scanner.Text()
		if len(text) > 0 {
			fmt.Println(text)
			if strings.Contains(text, "<PROG>") {
				fn(strings.SplitAfter(text, ":")[1])
			}
		}
	}

	if err := scanner.Err(); err != nil {
		fmt.Println("Error while reading stderr:", err)
	}
}
