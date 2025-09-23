package logger

import (
	"fmt"
	"os"
	"strings"
	"time"
)

var perfTiming bool

func init() {
	perfTiming = os.Getenv("TB_TIMER_ON") == "true"
}

type Timer struct {
	start      time.Time
	lastReport time.Time
	verbose    bool
	level      int
}

func NewTimer() Timer {
	now := time.Now()
	return Timer{
		start:      now,
		lastReport: now,
		verbose:    false,
		level:      0,
	}
}

func (t *Timer) Level(l int) {
	t.level = l
}

func (t *Timer) LevelUp() {
	t.level++
}

func (t *Timer) LevelDown() {
	if t.level > 0 {
		t.level--
	}
}

func (t *Timer) Report(msg string) {
	if !perfTiming {
		return
	}
	since := time.Since(t.start)
	diff := time.Since(t.lastReport)
	if t.verbose {
		Info(msg, "start", t.start)
		Info(msg, "stop", time.Now())
		Info(msg, "since", since)
		Info(msg, "diff", diff)
	}

	name := os.Getenv("TB_TIMER_NAME")
	if len(name) > 0 {
		msg = strings.ReplaceAll(msg, "chifra ", "") + "_" + name
	}

	max := func(x, y int64) int64 {
		if x > y {
			return x
		}
		return y
	}

	Info(fmt.Sprintf("PERF,%s%s,%d,%d\n", strings.Repeat("_", t.level), msg, max(1, since.Milliseconds()), max(1, diff.Milliseconds())))

	t.lastReport = time.Now()
}
