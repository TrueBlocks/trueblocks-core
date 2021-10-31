package utils

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

import (
	"io/ioutil"
	"log"
	"net/http"
	"os"
)

// FileExists help text todo
func FileExists(filename string) bool {
	info, err := os.Stat(filename)
	if os.IsNotExist(err) {
		return false
	}
	return !info.IsDir()
}

// FolderExists help text todo
func FolderExists(path string) bool {
	info, err := os.Stat(path)
	if os.IsNotExist(err) {
		return false
	}
	return info.IsDir()
}

// IsApiMode return true if we are running in api mode
func IsApiMode() bool {
	return os.Getenv("API_MODE") == "true"
}

// GetParam returns a single the 'key' parameter in the URL
func GetParam(key string, def string, r *http.Request) (string, bool) {
	values, exists := r.URL.Query()[key]
	if exists {
		if len(values) < 1 {
			return def, true
		}
		if values[0] == "" {
			return def, true
		}
		return values[0], true
	}
	return def, false
}

func AsciiFileToString(fn string) string {
	if !FileExists(fn) {
		return ""
	}

	contents, err := ioutil.ReadFile(fn)
	if err != nil {
		log.Println(err)
		return ""
	}
	return string(contents)
}

// maximum uint64
const NOPOS = ^uint64(0)
