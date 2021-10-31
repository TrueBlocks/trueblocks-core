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

func Log(sev severity, a ...interface{}) {
	// TODO: Add timestamp here if not in TestMode (see Erigon)
	fmt.Fprintf(os.Stderr, "<%s> : ", severityToLabel[sev])
	fmt.Fprintln(os.Stderr, a...)
}

func Fatal(a ...interface{}) {
	Log(ErrorFatal, a...)
	os.Exit(1)
}
