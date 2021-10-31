package logger

import (
	"fmt"
	"os"
)

type severity int

const (
	Progress severity = iota
	Info
	Warning
	Error
	ErrorFatal
)

var severityToLabel = map[severity]string{
	Progress:   "PROG",
	Info:       "INFO",
	Warning:    "WARNG",
	Error:      "ERROR",
	ErrorFatal: "FATAL",
}

// Log prints `a` to stderr with a label corresponding to the severity level
// prepended (e.g. <INFO>, <ERROR>, etc.)
func Log(sev severity, a ...interface{}) {
	fmt.Fprintf(os.Stderr, "<%s> : ", severityToLabel[sev])
	fmt.Fprintln(os.Stderr, a...)
}

// Fatal prints its arguments to stderr and calls os.Exit(1)
func Fatal(a ...interface{}) {
	Log(ErrorFatal, a...)
	os.Exit(1)
}
