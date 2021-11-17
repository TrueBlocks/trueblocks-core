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
package logger

import (
	"fmt"
	"os"
	"time"
)

type severity int

const (
	Progress severity = iota
	Info
	Test
	Warning
	Error
	ErrorFatal
)

var severityToLabel = map[severity]string{
	Progress:   "PROG",
	Info:       "INFO",
	Test:       "TEST",
	Warning:    "WARNG",
	Error:      "ERROR",
	ErrorFatal: "FATAL",
}

// I don't love this, but we can't call into the os every time we Log
// We can't use utils. because it creates a cyclical import
var testMode bool = os.Getenv("TEST_MODE") == "true"
var apiMode bool = os.Getenv("API_MODE") == "true"

// TestLog is used for testing and prints it's single input but only if it's true when in testMode
func TestLog(isDefault bool, a ...interface{}) {
	if !isDefault || !testMode || apiMode {
		return
	}
	Log(Test, a...)
}

// Log prints `a` to stderr with a label corresponding to the severity level
// prepended (e.g. <INFO>, <ERROR>, etc.)
func Log(sev severity, a ...interface{}) {

	timeDatePart := "DATE|TIME"
	if !testMode {
		now := time.Now()
		timeDatePart = now.Format("02-01|15:04:05.000")
	}

	fmt.Fprintf(os.Stderr, "%s[%s] ", severityToLabel[sev], timeDatePart)
	fmt.Fprintln(os.Stderr, a...)
}

// Fatal prints its arguments to stderr and calls os.Exit(1)
func Fatal(a ...interface{}) {
	Log(ErrorFatal, a...)
	os.Exit(1)
}
