// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package file

import (
	"errors"
	"io"
	"os"
	"syscall"
	"time"
)

var maxSecondsLock = 3

// Lock asks the OS to lock a file for the current process
func Lock(file *os.File) error {
	// Lock configuration. We want both read (F_RDLCK) and
	// write (F_WRLCK) locks, and by Start to be interpreted
	// as the start of the file (Whence set to SEEK_SET).
	//
	// More: https://man7.org/linux/man-pages/man2/fcntl.2.html
	lockConfig := &syscall.Flock_t{
		Type:   syscall.F_RDLCK | syscall.F_WRLCK,
		Whence: int16(io.SeekStart),
		Start:  0,
		Len:    0,
	}

	return changeLock(file, lockConfig)
}

// Unlock removes OS-level file lock
func Unlock(file *os.File) error {
	lockConfig := &syscall.Flock_t{
		Type:   syscall.F_UNLCK,
		Whence: int16(io.SeekStart),
		Start:  0,
		Len:    0,
	}

	return changeLock(file, lockConfig)
}

// changeLock is a helper function that sends the syscall to either lock or
// unlock a file
func changeLock(file *os.File, lockConfig *syscall.Flock_t) error {
	// We use fcntl lock, which locks a file to a specific process
	//
	// More: https://stackoverflow.com/questions/29611352/what-is-the-difference-between-locking-with-fcntl-and-flock
	return syscall.FcntlFlock(file.Fd(), syscall.F_SETLK, lockConfig)
}

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
