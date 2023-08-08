package logger

import (
	"fmt"
	"os"
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
	fmt.Println("INFO", msg+" timer:", since.Milliseconds(), "ms")
}
