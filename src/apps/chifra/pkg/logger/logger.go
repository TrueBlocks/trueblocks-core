// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package logger

import (
	"fmt"
	"io"
	"math"
	"os"
	"strings"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"golang.org/x/term"
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

func SetTestMode(onOff bool) {
	testMode = onOff
	testModeSet = true
}

// TestLog is used to print log lines during testing only
var TestLog = func(notDefault bool, a ...interface{}) {
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
	isTestMode    = false
	timingModeSet = false
	timingMode    = true
	decorationOff = false
)

func LogTimerOn() bool {
	return timingMode
}

func ToggleDecoration() {
	decorationOff = !decorationOff
}

var loggerWriter io.Writer = nil

func init() {
	if !timingModeSet {
		on := os.Getenv("TB_LOGTIMER_OFF") == ""
		isTestMode = os.Getenv("TEST_MODE") == "true"
		timingMode = on && !isTestMode
		timingModeSet = true
	}
	loggerWriter = os.Stderr
}

// SetLoggerWriter sets the writer to which log messages are written. Do not send
// nil as a writer if you wish to disable logging. Instead, use
//
//	w := logger.GetLoggerWriter()
//	defer logger.SetLoggerWriter(w)
//	logger.SetLoggerWriter(io.Discard)
//
// Setting to `nil` will core dump.
func SetLoggerWriter(w io.Writer) {
	loggerWriter = w
}

func GetLoggerWriter() io.Writer {
	return loggerWriter
}

// toLog prints `a` to stderr with a label corresponding to the severity level
// prepended (e.g. <INFO>, <EROR>, etc.)
func toLog(sev severity, a ...interface{}) {
	if loggerWriter == nil {
		return
	}

	timeDatePart := "DATE|TIME"
	if timingMode {
		now := time.Now()
		timeDatePart = now.Format("02-01|15:04:05.000")
	}

	if !decorationOff {
		fmt.Fprintf(loggerWriter, "%s[%s] ", severityToLabel[sev], timeDatePart)
	}
	switch sev {
	case progress:
		for index, aa := range a {
			if index > 0 {
				fmt.Fprint(loggerWriter, " ")
			}
			fmt.Fprint(loggerWriter, aa)
		}
		fmt.Fprint(loggerWriter, "\r")

	case infoC:
		fmt.Fprintf(loggerWriter, "%s%s%s ", colors.Green, a[0], colors.Off)
		for _, aa := range a[1:] {
			fmt.Fprintf(loggerWriter, "%s", aa)
		}
		fmt.Fprintln(loggerWriter, "")

	case warning:
		defer fmt.Fprint(loggerWriter, colors.Off)
		fmt.Fprint(loggerWriter, colors.Yellow)
		fmt.Fprintln(loggerWriter, a...)

	case err:
		defer fmt.Fprint(loggerWriter, colors.Off)
		fmt.Fprint(loggerWriter, colors.Red)
		fmt.Fprintln(loggerWriter, a...)

	default:
		fmt.Fprintln(loggerWriter, a...)
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

func ShouldNotHappen(v ...any) {
	args := append([]interface{}{"SHOULD NOT HAPPEN ==> IMPLEMENTATION ERROR:"}, v...)
	Panic(args...)
}

func Panicf(format string, v ...any) {
	s := fmt.Sprintf(format, v...)
	toLog(err, s)
	panic(s)
}

func Panic(v ...any) {
	s := fmt.Sprint(v...)
	toLog(err, s)
	panic(s)
}

func Fatalf(format string, v ...any) {
	s := fmt.Sprintf(format, v...)
	toLog(err, s)
	os.Exit(1)
}

func Fatal(v ...any) {
	s := fmt.Sprint(v...)
	toLog(err, s)
	os.Exit(1)
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

func Progress(tick bool, v ...any) {
	if isTestMode || !tick {
		return
	}
	toLog(progress, v...)
}

func IsTerminal() bool {
	return term.IsTerminal(int(os.Stdout.Fd()))
}
