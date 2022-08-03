// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package logger

import (
	"fmt"
	"os"
	"strings"
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

// TestLog is used to print command line options to the screen during testing only
func TestLog(notDefault bool, a ...interface{}) {
	testMode := os.Getenv("TEST_MODE") == "true"
	apiMode := os.Getenv("API_MODE") == "true"
	if !testMode || apiMode {
		// If we're not testing or in apiMode
		return
	}

	if notDefault {
		Log(Test, a...)
	}
}

var logTimingOffSet = false
var logTiming = true

func getLogTiming() bool {
	if !logTimingOffSet {
		logTimingOffSet = true
		logTiming = os.Getenv("LOG_TIMING_OFF") == "" && os.Getenv("TEST_MODE") != "true"
	}
	return logTiming
}

// Log prints `a` to stderr with a label corresponding to the severity level
// prepended (e.g. <INFO>, <ERROR>, etc.)
func Log(sev severity, a ...interface{}) {

	timeDatePart := "DATE|TIME"
	if getLogTiming() {
		now := time.Now()
		timeDatePart = now.Format("02-01|15:04:05.000")
	} else {
		timeDatePart = "DATE|TIME"
	}

	fmt.Fprintf(os.Stderr, "%s[%s] ", severityToLabel[sev], timeDatePart)
	fmt.Fprintln(os.Stderr, a...)
}

// Fatal prints its arguments to stderr and calls os.Exit(1)
func Fatal(a ...interface{}) {
	Log(ErrorFatal, a...)
	os.Exit(1)
}

//var logMutex sync.Mutex

func Enter(msg string, a ...interface{}) {
	marker := strings.Repeat("-", 20)
	//logMutex.Lock()
	Log(Progress, marker, "In", msg, a, marker)
	//logMutex.Unlock()
}

func Exit(msg string, a ...interface{}) {
	marker := strings.Repeat("-", 20)
	//logMutex.Lock()
	Log(Progress, marker, "Out", msg, a, marker)
	//logMutex.Unlock()
}

func ExitError(msg string, a ...interface{}) {
	marker := strings.Repeat("-", 20)
	//logMutex.Lock()
	Log(Error, marker, "Out with Error", msg, a, marker)
	//logMutex.Unlock()
}
