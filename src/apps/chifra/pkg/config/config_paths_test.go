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

func (p *PathTest) Report(index int, got string) {
	fmt.Printf("%-d,", index)
	fmt.Printf("\"%-10.10s", p.group+"\",")
	fmt.Printf("\"%-10.10s", p.xdg+"\",")
	fmt.Printf("\"%-10.10s", p.os+"\",")
	fmt.Printf("\"%-10.10s", p.chain+"\",")
	fmt.Printf("\"%-20.20s", p.part+"\",")
	fmt.Printf("\"%-40.40s    ", p.expected+"\",")
	fmt.Printf("\"%s\"\n", got)
}

func Test_GetPathTo(t *testing.T) {
	for index, test := range testSet1 {
		if test.disabled {
			continue
		}

		user, _ := user.Current()
		testPath := ""
		withChain := true

		os.Setenv("XDG_CONFIG_HOME", test.xdg)
		os.Setenv("TEST_CHAIN", test.chain)
		if test.group == "Config" {
			os.Setenv("TEST_OS", test.os)
			withChain = test.part != "trueBlocks.toml" && !strings.HasPrefix(test.part, "abis")
			testPath = GetPathToConfig(withChain) + test.part
		} else if test.group == "Cache" {
			testPath = GetPathToCache() + test.part
		}

		testPath = strings.Replace(testPath, user.HomeDir, "$HOME", -1)
		if test.expected == "" {
			fmt.Println("")
		} else {
			test.expected = strings.Replace(test.expected, "{CHAIN}/", GetChain(test.chain), -1)
			if testPath != test.expected {
				test.Report(index, testPath)
				t.Error(index, "Paths don't match (", test.expected, testPath, ")")
			} else {
				test.Report(index, testPath)
			}
		}
	}
}

var testSet1 = []PathTest{
	{
		group:    "Config",
		xdg:      "",
		os:       "linux",
		chain:    "",
		part:     "trueBlocks.toml",
		expected: "$HOME/.local/share/trueblocks/trueBlocks.toml",
	},
	{
		group:    "Config",
		xdg:      "xdg",
		os:       "linux",
		chain:    "",
		part:     "blockScrape.toml",
		expected: "$HOME/.local/share/trueblocks/config/{CHAIN}/blockScrape.toml",
	},
	{
		group:    "Config",
		xdg:      "xdg",
		os:       "linux",
		chain:    "",
		part:     "blockScrape.toml",
		expected: "$HOME/.local/share/trueblocks/config/{CHAIN}/blockScrape.toml",
	},
	{
		group:    "Config",
		xdg:      "/xdg",
		os:       "linux",
		chain:    "",
		part:     "trueBlocks.toml",
		expected: "/xdg/trueBlocks.toml",
	},
	{
		group:    "Config",
		xdg:      "/xdg",
		os:       "linux",
		chain:    "",
		part:     "abis/known-000/uniq_funcs.tab",
		expected: "/xdg/abis/known-000/uniq_funcs.tab",
	},
	{
		expected: "",
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
		expected: "",
	},
	{
		group:    "Config",
		xdg:      "",
		os:       "darwin",
		chain:    "",
		part:     "blockScrape.toml",
		expected: "$HOME/Library/Application Support/TrueBlocks/config/mainnet/blockScrape.toml",
	},
	{
		group:    "Config",
		xdg:      "xdg",
		os:       "darwin",
		chain:    "",
		part:     "blockScrape.toml",
		expected: "$HOME/Library/Application Support/TrueBlocks/config/{CHAIN}/blockScrape.toml",
	},
	{
		group:    "Config",
		xdg:      "/xdg",
		os:       "darwin",
		chain:    "",
		part:     "trueBlocks.toml",
		expected: "/xdg/trueBlocks.toml",
	},
	{
		expected: "",
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
		expected: "",
	},
	{
		group:    "Cache",
		xdg:      "",
		os:       "linux",
		chain:    "polygon",
		part:     "tx/00/00/",
		expected: "$HOME/.local/share/trueblocks/cache/{CHAIN}/tx/00/00/",
		disabled: true,
	},
	{
		group:    "Cache",
		xdg:      "",
		os:       "darwin",
		chain:    "",
		part:     "abis/0x12.json",
		expected: "$HOME/Library/Application Support/TrueBlocks/cache/{CHAIN}/abis/0x12.json",
		disabled: true,
	},
}
