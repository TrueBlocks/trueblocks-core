package utils

/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/

import (
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

// GetParam returns a single the 'key' parameter in the URL
func GetParam(key string, def string, r *http.Request) (string, bool) {
	values, exists := r.URL.Query()[key]
	log.Println("key: ", key, "len(values): ", len(values), " exists: ", exists)
	if exists {
		if len(values) < 1 {
			log.Println("len(values) < 1")
			return def, true
		}
		if values[0] == "" {
			log.Println("values[0] == \"\"")
			return def, true
		}
		log.Println("returning values[0]: ", values[0])
		return values[0], true
	}
	return def, false
}
