// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package logger

import (
	"fmt"
	"log"
	"os"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
)

type severity int

const (
	Progress severity = iota
	infoC             // colored table
	info
	test
	warning
	err
)

var severityToLabel = map[severity]string{
	Progress: "PROG",
	infoC:    "INFO",
	info:     "INFO",
	test:     "TEST",
	warning:  "WARN",
	err:      "EROR",
}

var (
	testModeSet     = false
	testMode        = false
	logTimingOffSet = false
	logTiming       = true
)

// TestLog is used to print command line options to the screen during testing only
func TestLog(notDefault bool, a ...interface{}) {
	if !testModeSet {
		testModeSet = true
		testMode = os.Getenv("TEST_MODE") == "true"
	}

	if !testMode {
		return
	}

	if notDefault {
		Log(test, a...)
	}
}

func getLogTiming() bool {
	if !logTimingOffSet {
		logTimingOffSet = true
		logTiming = os.Getenv("LOG_TIMING_OFF") == "" && os.Getenv("TEST_MODE") != "true"
	}
	return logTiming
}

// Log prints `a` to stderr with a label corresponding to the severity level
// prepended (e.g. <INFO>, <EROR>, etc.)
func Log(sev severity, a ...interface{}) {

	timeDatePart := "DATE|TIME"
	if getLogTiming() {
		now := time.Now()
		timeDatePart = now.Format("02-01|15:04:05.000")
	}

	fmt.Fprintf(os.Stderr, "%s[%s] ", severityToLabel[sev], timeDatePart)
	if sev == Progress {
		for _, aa := range a {
			fmt.Fprint(os.Stderr, aa)
		}
		fmt.Fprint(os.Stderr, "\r")

	} else if sev == infoC {
		fmt.Fprintf(os.Stderr, "%s%s%s ", colors.Green, a[0], colors.Off)
		for _, aa := range a[1:] {
			fmt.Fprintf(os.Stderr, "%s", aa)
		}
		fmt.Fprintln(os.Stderr, "")

	} else {
		fmt.Fprintln(os.Stderr, a...)
	}
}

func InfoTable(v ...any) {
	Log(infoC, v...)
}

func Info(v ...any) {
	Log(info, v...)
}

func Warn(v ...any) {
	Log(warning, v...)
}

func Error(v ...any) {
	Log(err, v...)
}

func Fatal(v ...any) {
	log.Fatal(v...)
}

func Panic(v ...any) {
	log.Panic(v...)
}
