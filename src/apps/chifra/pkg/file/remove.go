// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package file

import (
	"os"
	"sync"
)

func Remove(fileName string) bool {
	if !FileExists(fileName) {
		return true
	}
	mutex := sync.Mutex{}
	mutex.Lock()
	os.Remove(fileName)
	mutex.Unlock()
	return !FileExists(fileName)
}
