package file

import (
	"io"
	"os"
	"syscall"
)

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

func Unlock(file *os.File) error {
	lockConfig := &syscall.Flock_t{
		Type:   syscall.F_UNLCK,
		Whence: int16(io.SeekStart),
		Start:  0,
		Len:    0,
	}

	return changeLock(file, lockConfig)
}

func changeLock(file *os.File, lockConfig *syscall.Flock_t) error {
	// We use fcntl lock, which locks a file to a specific process
	//
	// More: https://stackoverflow.com/questions/29611352/what-is-the-difference-between-locking-with-fcntl-and-flock
	return syscall.FcntlFlock(file.Fd(), syscall.F_SETLK, lockConfig)
}
