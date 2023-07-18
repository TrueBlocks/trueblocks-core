package logger

import (
	"os"
	"strings"
	"time"
)

var perfTiming bool

func init() {
	perfTiming = os.Getenv("TB_TIMER_ON") == "true"
}

type Timer struct {
	start   time.Time
	verbose bool
}

func NewTimer() Timer {
	return Timer{
		start:   time.Now(),
		verbose: false,
	}
}

func (t Timer) Report(msg string) {
	if !perfTiming {
		return
	}
	since := time.Since(t.start)
	if t.verbose {
		Info(msg, "start", t.start)
		Info(msg, "stop", time.Now())
	}
	Info(msg+" timer:", since.Milliseconds(), "ms", strings.Repeat(" ", 80))
}
