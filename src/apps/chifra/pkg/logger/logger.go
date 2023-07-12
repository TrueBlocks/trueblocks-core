// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package logger

import (
	"fmt"
	"log"
	"math"
	"os"
	"strings"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

type severity int

const (
	progress severity = iota
	infoC             // colored table
	info
	test
	warning
	err
)

var severityToLabel = map[severity]string{
	progress: "PROG",
	infoC:    "INFO",
	info:     "INFO",
	test:     "TEST",
	warning:  "WARN",
	err:      "EROR",
}

var (
	testModeSet = false
	testMode    = false
)

// TestLog is used to print log lines during testing only
func TestLog(notDefault bool, a ...interface{}) {
	if !testModeSet {
		testModeSet = true
		testMode = os.Getenv("TEST_MODE") == "true"
	}

	if !testMode {
		return
	}

	if notDefault {
		toLog(test, a...)
	}
}

var (
	timingModeSet = false
	timingMode    = true
)

// toLog prints `a` to stderr with a label corresponding to the severity level
// prepended (e.g. <INFO>, <EROR>, etc.)
func toLog(sev severity, a ...interface{}) {
	if !timingModeSet {
		timingModeSet = true
		timingMode = os.Getenv("LOG_TIMING_OFF") == "" && os.Getenv("TEST_MODE") != "true"
	}

	timeDatePart := "DATE|TIME"
	if timingMode {
		now := time.Now()
		timeDatePart = now.Format("02-01|15:04:05.000")
	}

	fmt.Fprintf(os.Stderr, "%s[%s] ", severityToLabel[sev], timeDatePart)
	if sev == progress {
		for index, aa := range a {
			if index > 0 {
				fmt.Fprint(os.Stderr, " ")
			}
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
	toLog(infoC, v...)
}

func Info(v ...any) {
	toLog(info, v...)
}

func Warn(v ...any) {
	toLog(warning, v...)
}

func Error(v ...any) {
	toLog(err, v...)
}

func Fatal(v ...any) {
	log.Fatal(v...)
}

func Panic(v ...any) {
	log.Panic(v...)
}

func Progress(tick bool, v ...any) {
	if !utils.IsTerminal() || !tick {
		return
	}
	toLog(progress, v...)
}

func CleanLine() {
	// \033[K is escape sequence meaning "erase to end of line"
	fmt.Print("\r\033[K")
}

func PctProgress(done int32, total int, tick int32) {
	if done%tick != 0 {
		return
	}

	percentage := math.Round(float64(done) / float64(total) * 100)
	toLog(progress, fmt.Sprintf("\r\t\t\t Processing: %.f%% (%d of %d)%s", percentage, done, total, strings.Repeat(" ", 40)))
}
