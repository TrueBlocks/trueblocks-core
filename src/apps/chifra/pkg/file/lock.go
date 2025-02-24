// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package file

import (
	"errors"
	"os"
	"syscall"
	"time"
)

var maxSecondsLock = 3

var DefaultOpenFlags = os.O_RDWR | os.O_CREATE
var DefaultOpenFlagsAppend = os.O_RDWR | os.O_CREATE | os.O_APPEND

// WaitOnLock tries to lock a file for maximum `maxSecondsLock`
func WaitOnLock(filePath string, openFlags int) (file *os.File, err error) {
	file, err = os.OpenFile(filePath, openFlags, 0666)
	if err != nil {
		return
	}

	for i := 0; i < maxSecondsLock; i++ {
		err = Lock(file)
		if err == nil {
			return
		}
		if err == syscall.EAGAIN || err == syscall.EACCES {
			// Another process has already locked the file, we wait
			time.Sleep(500 * time.Millisecond)
			continue
		}
		return file, err
	}
	return file, errors.New("lock timeout")
}
