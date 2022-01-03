// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import (
	"fmt"
	"path"
	"testing"
)

type PathTest struct {
	xdg      string
	os       string
	part     string
	expected string
}

func Test_CetConfigPath(t *testing.T) {
	xdgs := []string{"", "~/should_fail", "/Users/user/.customized"}
	oses := []string{"", "linux", "darwin", "windows"}
	configs := []string{""}

	tests := []PathTest{}
	for _, config := range configs {
		for _, xdg := range xdgs {
			for _, os := range oses {
				test := PathTest{xdg: xdg, os: os, part: "part"}
				if len(config) > 0 {
					test.expected = path.Join(config, "part")
				} else {
					if len(xdg) > 0 {
						test.expected = path.Join(xdg, "part")
					} else {
						test.expected = path.Join(OsToPath[os], "part")
					}
				}
				tests = append(tests, test)
			}
		}
	}
	for _, test := range tests {
		os.SetEnv("XDG_DATA_HOME=")
		if test.
		os.SetEnv("XDG_DATA_HOME=")
		fmt.Println(test)
	}
	// tests := {
	// 	PathTest{
	// 		xdf_data_home: "",
	// 	}
	// }
	// var expectedDir string

	// switch runtime.GOOS {
	// case "linux":
	// 	expectedDir = ".local/share/trueblocks"
	// case "darwin":
	// 	expectedDir = "Library/Application Support/TrueBlocks"
	// }

	// result := GetPathToConfig("some_file.toml")

	// if !strings.Contains(result, expectedDir) {
	// 	t.Error("Expected directory not present", result)
	// }

	// if !strings.Contains(result, "some_file.toml") {
	// 	t.Error("Expected filename not present", result)
	// }
}
