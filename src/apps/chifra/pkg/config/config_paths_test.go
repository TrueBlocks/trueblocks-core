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
	xdg      string
	os       string
	chain    string
	part     string
	expected string
}

func Test_GetConfigPath(t *testing.T) {
	for _, test := range testSet1 {
		os.Setenv("XDG_DATA_HOME", test.xdg)
		os.Setenv("TEST_OS", test.os)
		os.Setenv("TEST_CHAIN", test.chain)
		user, _ := user.Current()
		path := strings.Replace(GetPathToChainConfig(test.part), user.HomeDir, "$HOME", -1)
		if path != test.expected {
			fmt.Println("returned: ", path, " expected: ", test.expected)
			t.Error("Paths don't match (", path, ", ", test.expected, ")")
		} else {
			fmt.Println("Pass: ", path)
		}
	}
}

var testSet1 = []PathTest{
	{
		xdg:      "",
		os:       "linux",
		chain:    "",
		part:     "trueBlocks.toml",
		expected: "$HOME/.local/share/trueblocks/trueBlocks.toml",
	},
	{
		xdg:      "xdg",
		os:       "linux",
		chain:    "",
		part:     "blockScrape.toml",
		expected: "$HOME/.local/share/trueblocks/blockScrape.toml",
	},
	{
		xdg:      "/xdg",
		os:       "linux",
		chain:    "",
		part:     "trueBlocks.toml",
		expected: "/xdg/trueBlocks.toml",
	},
	{
		xdg:      "",
		os:       "linux",
		chain:    "mainnet",
		part:     "trueBlocks.toml",
		expected: "$HOME/.local/share/trueblocks/trueBlocks.toml",
	},
	{
		xdg:      "xdg",
		os:       "linux",
		chain:    "mainnet",
		part:     "blockScrape.toml",
		expected: "$HOME/.local/share/trueblocks/blockScrape.toml",
	},
	{
		xdg:      "/xdg",
		os:       "linux",
		chain:    "mainnet",
		part:     "trueBlocks.toml",
		expected: "/xdg/trueBlocks.toml",
	},
	{
		xdg:      "",
		os:       "darwin",
		chain:    "",
		part:     "trueBlocks.toml",
		expected: "$HOME/Library/Application Support/TrueBlocks/trueBlocks.toml",
	},
	{
		xdg:      "xdg",
		os:       "darwin",
		chain:    "",
		part:     "blockScrape.toml",
		expected: "$HOME/Library/Application Support/TrueBlocks/blockScrape.toml",
	},
	{
		xdg:      "/xdg",
		os:       "darwin",
		chain:    "",
		part:     "trueBlocks.toml",
		expected: "/xdg/trueBlocks.toml",
	},
	{
		xdg:      "",
		os:       "darwin",
		chain:    "mainnet",
		part:     "trueBlocks.toml",
		expected: "$HOME/Library/Application Support/TrueBlocks/trueBlocks.toml",
	},
	{
		xdg:      "xdg",
		os:       "darwin",
		chain:    "mainnet",
		part:     "blockScrape.toml",
		expected: "$HOME/Library/Application Support/TrueBlocks/blockScrape.toml",
	},
	{
		xdg:      "/xdg",
		os:       "darwin",
		chain:    "mainnet",
		part:     "trueBlocks.toml",
		expected: "/xdg/trueBlocks.toml",
	},
}
