/*
 */
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
	f        string
	xdg      string
	os       string
	chain    string
	part     string
	expected string
}

func (p *PathTest) Report(got string) {
	fmt.Printf("\"%-20.20s", p.f+"\",")
	fmt.Printf("\"%-10.10s", p.xdg+"\",")
	fmt.Printf("\"%-10.10s", p.os+"\",")
	fmt.Printf("\"%-15.15s", p.chain+"\",")
	fmt.Printf("\"%-20.20s", p.part+"\",")
	fmt.Printf("\"%-40.40s    ", p.expected+"\",")
	fmt.Printf("\"%s\"\n", got)
}

func Test_GetConfigPath(t *testing.T) {
	for index, test := range testSet1 {
		os.Setenv("XDG_CONFIG_HOME", test.xdg)
		os.Setenv("TEST_OS", test.os)
		os.Setenv("TEST_CHAIN", test.chain)
		// fmt.Println("x: ", os.Getenv("TEST_CHAIN"))
		user, _ := user.Current()
		withChain := test.part != "trueBlocks.toml" && !strings.HasPrefix(test.part, "abis")
		path := strings.Replace(GetPathToConfig(withChain)+test.part, user.HomeDir, "$HOME", -1)
		if test.expected == "" {
			fmt.Println("")
		} else {
			test.expected = strings.Replace(test.expected, "{CHAIN}/", GetChain(test.chain), -1)
			if path != test.expected {
				t.Error(index, "Paths don't match (", test.expected, path, ")")
			} else {
				test.Report(path)
			}
		}
	}
}

var testSet1 = []PathTest{
	{
		f:        "GetConfigPath",
		xdg:      "",
		os:       "linux",
		chain:    "",
		part:     "trueBlocks.toml",
		expected: "$HOME/.local/share/trueblocks/trueBlocks.toml",
	},
	{
		f:        "GetConfigPath",
		xdg:      "xdg",
		os:       "linux",
		chain:    "",
		part:     "blockScrape.toml",
		expected: "$HOME/.local/share/trueblocks/config/{CHAIN}/blockScrape.toml",
	},
	{
		f:        "GetConfigPath",
		xdg:      "xdg",
		os:       "linux",
		chain:    "",
		part:     "blockScrape.toml",
		expected: "$HOME/.local/share/trueblocks/config/{CHAIN}/blockScrape.toml",
	},
	{
		f:        "GetConfigPath",
		xdg:      "/xdg",
		os:       "linux",
		chain:    "",
		part:     "trueBlocks.toml",
		expected: "/xdg/trueBlocks.toml",
	},
	{
		f:        "GetConfigPath",
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
		f:        "GetConfigPath",
		xdg:      "",
		os:       "linux",
		chain:    "mainnet",
		part:     "trueBlocks.toml",
		expected: "$HOME/.local/share/trueblocks/trueBlocks.toml",
	},
	{
		f:        "GetConfigPath",
		xdg:      "xdg",
		os:       "linux",
		chain:    "mainnet",
		part:     "blockScrape.toml",
		expected: "$HOME/.local/share/trueblocks/config/{CHAIN}/blockScrape.toml",
	},
	{
		f:        "GetConfigPath",
		xdg:      "/xdg",
		os:       "linux",
		chain:    "xdai",
		part:     "trueBlocks.toml",
		expected: "/xdg/trueBlocks.toml",
	},
	{
		expected: "",
	},
	{
		f:        "GetConfigPath",
		xdg:      "",
		os:       "darwin",
		chain:    "",
		part:     "trueBlocks.toml",
		expected: "$HOME/Library/Application Support/TrueBlocks/trueBlocks.toml",
	},
	{
		f:        "GetConfigPath",
		xdg:      "xdg",
		os:       "darwin",
		chain:    "",
		part:     "blockScrape.toml",
		expected: "$HOME/Library/Application Support/TrueBlocks/config/{CHAIN}/blockScrape.toml",
	},
	{
		f:        "GetConfigPath",
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
		f:        "GetConfigPath",
		xdg:      "",
		os:       "darwin",
		chain:    "xdai",
		part:     "trueBlocks.toml",
		expected: "$HOME/Library/Application Support/TrueBlocks/trueBlocks.toml",
	},
	{
		f:        "GetConfigPath",
		xdg:      "xdg",
		os:       "darwin",
		chain:    "xdai",
		part:     "blockScrape.toml",
		expected: "$HOME/Library/Application Support/TrueBlocks/config/{CHAIN}/blockScrape.toml",
	},
	{
		f:        "GetConfigPath",
		xdg:      "/xdg",
		os:       "darwin",
		chain:    "mainnet",
		part:     "trueBlocks.toml",
		expected: "/xdg/trueBlocks.toml",
	},
	{
		expected: "",
	},
}
