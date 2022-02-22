// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

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

// TestLog is used to print command line options to the screen during testing only
func TestLog(notDefault bool, a ...interface{}) {
	if !testMode || apiMode {
		// If we're not testing or in apiMode
		return
	}

	if notDefault {
		Log(Test, a...)
	}
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
