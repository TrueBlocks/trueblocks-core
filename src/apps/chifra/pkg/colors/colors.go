// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package colors

import "strings"

var None = ""
var Off = "\033[0m"
var Red = "\033[31m"
var Green = "\033[32m"
var Yellow = "\033[33m"
var Blue = "\033[34m"
var Magenta = "\033[35m"
var Cyan = "\033[36m"
var White = "\033[37m"
var Black = "\033[30m"
var Bright = "\033[1m"
var BrightRed = (Bright + Red)
var BrightGreen = (Bright + Green)
var BrightYellow = (Bright + Yellow)
var BrightBlue = (Bright + Blue)
var BrightMagenta = (Bright + Magenta)
var BrightCyan = (Bright + Cyan)
var BrightWhite = (Bright + White)
var BrightBlack = (Bright + Black)

func ColorsOff() {
	Off = ""
	Red = ""
	Green = ""
	Yellow = ""
	Blue = ""
	Magenta = ""
	Cyan = ""
	White = ""
	Black = ""
	Bright = ""
	BrightRed = ""
	BrightGreen = ""
	BrightYellow = ""
	BrightBlue = ""
	BrightMagenta = ""
	BrightCyan = ""
	BrightWhite = ""
	BrightBlack = ""
}

func Colored(s string) string {
	s = strings.Replace(s, "{", Green, -1)
	s = strings.Replace(s, "@", BrightYellow, -1)
	s = strings.Replace(s, "}", Off, -1)
	return s
}

func ColoredWith(s string, c string) string {
	s = c + s
	s = strings.Replace(s, "{", Green, -1)
	s = strings.Replace(s, "@", BrightYellow, -1)
	s = strings.Replace(s, "}", c, -1)
	s += Off
	return s
}
