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

	nItems := 0

	fmt.Printf("PERF,%s,%dms,%d\n", msg, since.Milliseconds(), nItems)
}
