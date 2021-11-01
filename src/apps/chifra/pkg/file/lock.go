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
package file

import (
	"io"
	"os"
	"syscall"
)

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
