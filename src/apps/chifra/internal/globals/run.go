// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package globals

import (
	"bufio"
	"bytes"
	"fmt"
	"io"
	"os"
	"os/exec"
	"strings"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
)

type ConfigEnv struct {
	Chain           string `json:"chain"`
	ConfigPath      string `json:"configPath"`
	ChainConfigPath string `json:"chainConfigPath"`
	CachePath       string `json:"cachePath"`
	IndexPath       string `json:"indexPath"`
	DefaultChain    string `json:"defChain"`
	RpcProvider     string `json:"rpcProvider"`
}

func (c *ConfigEnv) toCSV() string {
	var ret []string
	ret = append(ret, c.Chain)
	ret = append(ret, c.ConfigPath)
	ret = append(ret, c.ChainConfigPath)
	ret = append(ret, c.CachePath)
	ret = append(ret, c.IndexPath)
	ret = append(ret, c.DefaultChain)
	ret = append(ret, c.RpcProvider)
	return strings.Join(ret, ",")
}

func (opts *GlobalOptions) PassItOn(path string, flags string) error {
	options := flags
	options += opts.ToCmdLine()

	var wg sync.WaitGroup
	wg.Add(2)

	var env ConfigEnv
	env.Chain = opts.Chain
	env.ConfigPath = config.GetPathToRootConfig()
	env.ChainConfigPath = config.GetPathToChainConfig(env.Chain)
	env.CachePath = config.GetPathToCache(env.Chain)
	env.IndexPath = config.GetPathToIndex(env.Chain)
	env.DefaultChain = config.GetDefaultChain()
	env.RpcProvider = config.GetRpcProvider(env.Chain)
	envStr := env.toCSV()

	// fmt.Fprintf(os.Stderr, "Calling: %s %s\n", path, options)
	cmd := exec.Command(config.GetPathToCommands(path), options)
	cmd.Env = append(os.Environ(), "FROM_CHIFRA=true")
	if !opts.TestMode && opts.LogLevel > 8 {
		fmt.Fprintf(os.Stderr, "%s%s%s%s\n", colors.Blue, colors.Bright, envStr, colors.Off)
	}
	cmd.Env = append(cmd.Env, "TB_CONFIG_PATH="+env.ConfigPath)
	cmd.Env = append(cmd.Env, "TB_CHAIN_CONFIG_PATH="+env.ChainConfigPath)
	cmd.Env = append(cmd.Env, "TB_CACHE_PATH="+env.CachePath)
	cmd.Env = append(cmd.Env, "TB_INDEX_PATH="+env.IndexPath)
	cmd.Env = append(cmd.Env, "TB_DEFAULT_CHAIN="+env.DefaultChain)
	cmd.Env = append(cmd.Env, "TB_RPC_PROVIDER="+env.RpcProvider)
	cmd.Env = append(cmd.Env, "TB_CHAIN="+env.Chain)
	cmd.Env = append(cmd.Env, "TB_CONFIG_ENV="+envStr)
	if os.Getenv("TEST_MODE") == "true" {
		cmd.Env = append(cmd.Env, "TEST_MODE=true")
	}

	stderrPipe, err := cmd.StderrPipe()
	if err != nil {
		fmt.Fprintf(os.Stderr, "%s", err)
	} else {
		go func() {
			ScanForProgress2(stderrPipe, func(msg string) {
			})
			wg.Done()
		}()
	}

	stdout, err := cmd.StdoutPipe()
	if err != nil {
		fmt.Fprintf(os.Stderr, "%s", err)
	} else {
		go func() {
			cmd.Start()
			scanner := bufio.NewScanner(stdout)
			buf := make([]byte, 1024*1024)
			scanner.Buffer(buf, 1024*1024)
			for scanner.Scan() {
				m := scanner.Text()
				fmt.Println(m)
			}
			wg.Done()
		}()
	}
	wg.Wait()
	cmd.Wait()
	return nil
}

// dropNL2 drops new line characters (\n) from the progress stream
func dropNL2(data []byte) []byte {
	if len(data) > 0 && data[len(data)-1] == '\n' {
		return data[0 : len(data)-1]
	}
	return data
}

// ScanProgressLine2 looks for "lines" that end with `\r` not `\n` like usual
func ScanProgressLine2(data []byte, atEOF bool) (advance int, token []byte, err error) {
	if atEOF && len(data) == 0 {
		return 0, nil, nil
	}
	if i := bytes.IndexByte(data, '\n'); i >= 0 {
		fmt.Fprintf(os.Stderr, "%s\n", string(data[0:i]))
		return i + 1, data[0:i], nil
	}
	if i := bytes.IndexByte(data, '\r'); i >= 0 {
		fmt.Fprintf(os.Stderr, "%s\r", string(data[0:i]))
		return i + 1, dropNL2(data[0:i]), nil
	}
	return bufio.ScanLines(data, atEOF)
}

// ScanForProgress2 watches stderr and picks of progress messages
func ScanForProgress2(stderrPipe io.Reader, fn func(string)) {
	scanner := bufio.NewScanner(stderrPipe)
	scanner.Split(ScanProgressLine2)
	for scanner.Scan() {
		// we've already printed the token
	}
	if err := scanner.Err(); err != nil {
		fmt.Println("TB: Error while reading stderr -- ", err)
	}
}
