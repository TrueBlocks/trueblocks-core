package logger

import (
	"os"
	"time"
)

var perfTiming bool

func init() {
	perfTiming = os.Getenv("TB_TIMING") == "true"
}

type Timer struct {
	name  string
	start time.Time
}

func NewTimer(n string) Timer {
	return Timer{
		name:  n,
		start: time.Now(),
	}
}

func (t Timer) Report(msg string) {
	if !perfTiming {
		return
	}
	since := time.Since(t.start)
	Info(msg, "start", t.start)
	Info(msg, "stop", time.Now())
	Info(msg, "since", since.Milliseconds())
}
