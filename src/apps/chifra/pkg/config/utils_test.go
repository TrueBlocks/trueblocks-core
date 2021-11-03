/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
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
