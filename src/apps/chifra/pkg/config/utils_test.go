// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import (
	"runtime"
	"strings"
	"testing"
)

func Test_getConfigPath(t *testing.T) {
	var expectedDir string

	switch runtime.GOOS {
	case "linux":
		expectedDir = ".local/share/trueblocks"
	case "darwin":
		expectedDir = "Library/Application Support/TrueBlocks"
	}

	result := GetConfigPath("some_file.toml")

	if !strings.Contains(result, expectedDir) {
		t.Error("Expected directory not present", result)
	}

	if !strings.Contains(result, "some_file.toml") {
		t.Error("Expected filename not present", result)
	}
}
