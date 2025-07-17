//go:build integration
// +build integration

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import (
	"fmt"
	"os"
	"os/user"
	"path/filepath"
	"runtime"
	"strings"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

type PathTest struct {
	group    string
	xdg      string
	os       string
	chain    string
	part     string
	expected string
	disabled bool
}

func Test_GetPathTo(t *testing.T) {
	userOs := runtime.GOOS
	for index, test := range testSet1 {
		if test.disabled || userOs != test.os {
			continue
		}

		user, _ := user.Current()
		testPath := ""
		withChain := true
		// TODO: bad xdg paths cause panic, so we turn this off
		test.xdg = ""

		switch test.group {
		case "Config":
			os.Setenv("XDG_CONFIG_HOME", test.xdg)
			os.Setenv("TEST_OS", test.os)
			globals := []string{"trueBlocks.toml", "makeClass.toml", "testRunner.toml", "abis/"}
			for _, g := range globals {
				if strings.HasPrefix(test.part, g) {
					withChain = false
				}
			}
			if withChain {
				testPath = filepath.Join(MustGetPathToChainConfig(test.chain), test.part)
			} else {
				testPath = filepath.Join(PathToRootConfig(), test.part)
			}
		case "Cache":
			os.Setenv("XDG_CACHE_HOME", test.xdg)
			testPath = filepath.Join(PathToCache(test.chain), test.part)
		case "Index":
			os.Setenv("XDG_CACHE_HOME", test.xdg)
			testPath = filepath.Join(PathToIndex(test.chain), test.part)
		}

		testPath = strings.Replace(testPath, user.HomeDir, "$HOME", -1)
		if test.expected == "" {
			fmt.Println("")
		} else {
			test.expected = strings.ReplaceAll(test.expected, "{CHAIN}", test.chain)
			if testPath != test.expected {
				test.Report(index, testPath)
				t.Error(index, "Paths don't match (", test.expected, testPath, ")")
			} else {
				test.Report(index, testPath)
			}
		}
	}
}

func (p *PathTest) Report(index int, got string) {
	fmt.Printf("%2d ", index)
	fmt.Printf("%-7.7s", p.group)
	fmt.Printf("%-5.5s", p.xdg)
	fmt.Printf("%-6.6s", p.os)
	fmt.Printf("%-8.8s", p.chain)
	fmt.Printf("%-12.10s", p.part)
	fmt.Printf("%-50.48s", p.expected)
	fmt.Printf("%s\n", got)
}

var testSet1 = []PathTest{
	{
		group:    "Config",
		xdg:      "",
		os:       "linux",
		chain:    utils.GetTestChain(),
		part:     "trueBlocks.toml",
		expected: "$HOME/.local/share/trueblocks/trueBlocks.toml",
	},
	{
		group:    "Config",
		xdg:      "xdg",
		os:       "linux",
		chain:    utils.GetTestChain(),
		part:     "blockScrape.toml",
		expected: "$HOME/.local/share/trueblocks/config/{CHAIN}/blockScrape.toml",
	},
	{
		group: "Config",
		xdg:   "/xdg",
		os:    "linux",
		chain: utils.GetTestChain(),
		part:  "trueBlocks.toml",
		// expected: "/xdg/trueBlocks.toml",
		expected: "$HOME/.local/share/trueblocks/trueBlocks.toml",
	},
	{
		group: "Config",
		xdg:   "/xdg",
		os:    "linux",
		chain: utils.GetTestChain(),
		part:  "abis/known-000/uniq_funcs.tab",
		// expected: "/xdg/trueBlocks.toml",
		expected: "$HOME/.local/share/trueblocks/abis/known-000/uniq_funcs.tab",
	},
	{
		group: "Config",
		xdg:   "/xdg",
		os:    "linux",
		chain: "gnosis",
		part:  "blockScrape.toml",
		// expected: "/xdg/config/gnosis/blockScrape.toml",
		expected: "$HOME/.local/share/trueblocks/config/gnosis/blockScrape.toml",
	},
	{
		group:    "Config",
		xdg:      "",
		os:       "darwin",
		chain:    utils.GetTestChain(),
		part:     "blockScrape.toml",
		expected: "$HOME/Library/Application Support/TrueBlocks/config/mainnet/blockScrape.toml",
	},
	{
		group:    "Config",
		xdg:      "xdg",
		os:       "darwin",
		chain:    utils.GetTestChain(),
		part:     "blockScrape.toml",
		expected: "$HOME/Library/Application Support/TrueBlocks/config/{CHAIN}/blockScrape.toml",
	},
	{
		group: "Config",
		xdg:   "/xdg",
		os:    "darwin",
		chain: utils.GetTestChain(),
		part:  "trueBlocks.toml",
		// expected: "/xdg/trueBlocks.toml",
		expected: "$HOME/Library/Application Support/TrueBlocks/trueBlocks.toml",
	},
	{
		group:    "Config",
		xdg:      "",
		os:       "darwin",
		chain:    "gnosis",
		part:     "trueBlocks.toml",
		expected: "$HOME/Library/Application Support/TrueBlocks/trueBlocks.toml",
	},
	{
		group:    "Config",
		xdg:      "xdg",
		os:       "darwin",
		chain:    "gnosis",
		part:     "trueBlocks.toml",
		expected: "$HOME/Library/Application Support/TrueBlocks/trueBlocks.toml",
	},
	{
		group: "Config",
		xdg:   "/xdg",
		os:    "darwin",
		chain: utils.GetTestChain(),
		part:  "trueBlocks.toml",
		// expected: "/xdg/trueBlocks.toml",
		expected: "$HOME/Library/Application Support/TrueBlocks/trueBlocks.toml",
	},
	// {
	// 	group:    "Cache",
	// 	xdg:      "/xdg",
	// 	os:       "linux",
	// 	chain:    "polygon",
	// 	part:     "tx/00/00/",
	// 	expected: "/xdg/cache/{CHAIN}/tx/00/00/",
	// expected: "$HOME/.local/share/trueblocks/cache/{CHAIN}/tx/00/00/",
	// },
	{
		group:    "Cache",
		xdg:      "",
		os:       "darwin",
		chain:    utils.GetTestChain(),
		part:     "abis/0x12.json",
		expected: "$HOME/Library/Application Support/TrueBlocks/cache/{CHAIN}/abis/0x12.json",
		disabled: true,
	},
	{
		group:    "Cache",
		xdg:      "",
		os:       "windows",
		chain:    utils.GetTestChain(),
		part:     "abis/0x12.json",
		expected: "$HOME/Library/Application Support/TrueBlocks/cache/{CHAIN}/abis/0x12.json",
		disabled: true,
	},
	{
		group:    "Index",
		xdg:      "",
		os:       "darwin",
		chain:    utils.GetTestChain(),
		part:     "names.bin",
		expected: "$HOME/Library/Application Support/TrueBlocks/unchained/{CHAIN}/names.bin",
		disabled: true,
	},
}
