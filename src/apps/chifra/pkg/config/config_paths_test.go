// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import (
	"fmt"
	"os"
	"os/user"
	"strings"
	"testing"
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
	for index, test := range testSet1 {
		if test.disabled {
			continue
		}

		user, _ := user.Current()
		testPath := ""
		withChain := true

		os.Setenv("TEST_CHAIN", test.chain)
		if test.group == "Config" {
			os.Setenv("XDG_CONFIG_HOME", test.xdg)
			os.Setenv("TEST_OS", test.os)
			globals := []string{"trueBlocks.toml", "makeClass.toml", "testRunner.toml", "abis/"}
			for _, g := range globals {
				if strings.HasPrefix(test.part, g) {
					withChain = false
				}
			}
			testPath = GetPathToConfig(withChain) + test.part
		} else if test.group == "Cache" {
			os.Setenv("XDG_CACHE_HOME", test.xdg)
			testPath = GetPathToCache1(test.chain) + test.part
		} else if test.group == "Index" {
			os.Setenv("XDG_CACHE_HOME", test.xdg)
			testPath = GetPathToIndex1(test.chain) + test.part
		}

		testPath = strings.Replace(testPath, user.HomeDir, "$HOME", -1)
		if test.expected == "" {
			fmt.Println("")
		} else {
			test.expected = strings.Replace(test.expected, "{CHAIN}/", test.chain+"/", -1)
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
		chain:    "mainnet",
		part:     "trueBlocks.toml",
		expected: "$HOME/.local/share/trueblocks/trueBlocks.toml",
	},
	{
		group:    "Config",
		xdg:      "xdg",
		os:       "linux",
		chain:    "mainnet",
		part:     "blockScrape.toml",
		expected: "$HOME/.local/share/trueblocks/config/{CHAIN}/blockScrape.toml",
	},
	{
		group:    "Config",
		xdg:      "xdg",
		os:       "linux",
		chain:    "mainnet",
		part:     "blockScrape.toml",
		expected: "$HOME/.local/share/trueblocks/config/{CHAIN}/blockScrape.toml",
	},
	{
		group:    "Config",
		xdg:      "/xdg",
		os:       "linux",
		chain:    "mainnet",
		part:     "trueBlocks.toml",
		expected: "/xdg/trueBlocks.toml",
	},
	{
		group:    "Config",
		xdg:      "/xdg",
		os:       "linux",
		chain:    "mainnet",
		part:     "abis/known-000/uniq_funcs.tab",
		expected: "/xdg/abis/known-000/uniq_funcs.tab",
	},
	{
		group:    "Config",
		xdg:      "",
		os:       "linux",
		chain:    "mainnet",
		part:     "trueBlocks.toml",
		expected: "$HOME/.local/share/trueblocks/trueBlocks.toml",
	},
	{
		group:    "Config",
		xdg:      "xdg",
		os:       "linux",
		chain:    "mainnet",
		part:     "blockScrape.toml",
		expected: "$HOME/.local/share/trueblocks/config/{CHAIN}/blockScrape.toml",
	},
	{
		group:    "Config",
		xdg:      "/xdg",
		os:       "linux",
		chain:    "xdai",
		part:     "blockScrape.toml",
		expected: "/xdg/config/xdai/blockScrape.toml",
	},
	{
		group:    "Config",
		xdg:      "",
		os:       "darwin",
		chain:    "mainnet",
		part:     "blockScrape.toml",
		expected: "$HOME/Library/Application Support/TrueBlocks/config/mainnet/blockScrape.toml",
	},
	{
		group:    "Config",
		xdg:      "xdg",
		os:       "darwin",
		chain:    "mainnet",
		part:     "blockScrape.toml",
		expected: "$HOME/Library/Application Support/TrueBlocks/config/{CHAIN}/blockScrape.toml",
	},
	{
		group:    "Config",
		xdg:      "/xdg",
		os:       "darwin",
		chain:    "mainnet",
		part:     "trueBlocks.toml",
		expected: "/xdg/trueBlocks.toml",
	},
	{
		group:    "Config",
		xdg:      "",
		os:       "darwin",
		chain:    "xdai",
		part:     "trueBlocks.toml",
		expected: "$HOME/Library/Application Support/TrueBlocks/trueBlocks.toml",
	},
	{
		group:    "Config",
		xdg:      "xdg",
		os:       "darwin",
		chain:    "xdai",
		part:     "trueBlocks.toml",
		expected: "$HOME/Library/Application Support/TrueBlocks/trueBlocks.toml",
	},
	{
		group:    "Config",
		xdg:      "/xdg",
		os:       "darwin",
		chain:    "mainnet",
		part:     "trueBlocks.toml",
		expected: "/xdg/trueBlocks.toml",
	},
	{
		group:    "Cache",
		xdg:      "/xdg",
		os:       "linux",
		chain:    "polygon",
		part:     "tx/00/00/",
		expected: "/xdg/cache/{CHAIN}/tx/00/00/",
	},
	{
		group:    "Cache",
		xdg:      "",
		os:       "darwin",
		chain:    "mainnet",
		part:     "abis/0x12.json",
		expected: "$HOME/Library/Application Support/TrueBlocks/cache/{CHAIN}/abis/0x12.json",
	},
	{
		group:    "Cache",
		xdg:      "",
		os:       "windows",
		chain:    "mainnet",
		part:     "abis/0x12.json",
		expected: "$HOME/Library/Application Support/TrueBlocks/cache/{CHAIN}/abis/0x12.json",
	},
	{
		group:    "Index",
		xdg:      "/xdg",
		os:       "linux",
		chain:    "polygon",
		part:     "tx/00/00/",
		expected: "/xdg/unchained/{CHAIN}/tx/00/00/",
	},
	{
		group:    "Index",
		xdg:      "",
		os:       "darwin",
		chain:    "mainnet",
		part:     "names/names.bin",
		expected: "$HOME/Library/Application Support/TrueBlocks/unchained/{CHAIN}/names/names.bin",
	},
}
