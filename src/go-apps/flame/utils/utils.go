package utils

/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/

import (
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
