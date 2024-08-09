//go:build !windows
// +build !windows

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package progress

import (
	"syscall"
	"unsafe"
)

func screenWidth() uint {
	ws := &winsize{}
	retCode, _, _ := syscall.Syscall(syscall.SYS_IOCTL,
		uintptr(syscall.Stdin),
		uintptr(syscall.TIOCGWINSZ),
		uintptr(unsafe.Pointer(ws)))

	if int(retCode) == -1 {
		// This is okay if we're debugging in VSCode for example
		// logger.Error("System call to syscall.SYS_IOCTL returned: ", errno)
		return 120 // default reasonably
	}
	return uint(ws.Col)
}
