// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package progress

import (
	"fmt"
	"os"
	"strings"
	"syscall"
	"unsafe"
)

type ScanBar struct {
	Visited uint64
	Found   uint64
	Wanted  uint64
	Max     uint64
	Freq    uint64
}

func NewScanBar(want, freq, max uint64) *ScanBar {
	sp := &ScanBar{}
	sp.Wanted = want
	sp.Freq = freq
	sp.Max = max
	return sp
}

func (v *ScanBar) Satisfied() bool {
	return v.Visited > v.Max || v.Found == v.Wanted
}

func (v *ScanBar) Pct() float64 {
	return (float64(v.Visited) / float64(v.Max))
}

func min(a, b int) int {
	if a < b {
		return a
	}
	return b
}
func (v *ScanBar) Report(target *os.File, action, msg string) {
	v.Visited++
	if v.Visited%v.Freq != 0 {
		return
	}
	width := 40
	done := int(float64(width) * v.Pct())
	remains := width - done
	if remains < 0 {
		remains = 0
	}
	w := int(screenWidth()) - 2
	x := fmt.Sprintf("%s [%s%s] %s", action, strings.Repeat(".", done), strings.Repeat(" ", remains), msg)
	x = x[0:min(len(x), w)]
	e := w - len(x)
	var endPad string = strings.Repeat("-", e)
	fmt.Fprintf(target, "%s%s\r", x, endPad) // , w, e, len(x))
}

type winsize struct {
	Row    uint16
	Col    uint16
	Xpixel uint16
	Ypixel uint16
}

func screenWidth() uint {
	ws := &winsize{}
	retCode, _, errno := syscall.Syscall(syscall.SYS_IOCTL,
		uintptr(syscall.Stdin),
		uintptr(syscall.TIOCGWINSZ),
		uintptr(unsafe.Pointer(ws)))

	if int(retCode) == -1 {
		panic(errno)
	}
	return uint(ws.Col)
}
