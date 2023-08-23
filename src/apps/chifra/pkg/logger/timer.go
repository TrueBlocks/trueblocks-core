package logger

import (
	"fmt"
	"os"
	"strings"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
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

	name := os.Getenv("TB_TIMER_NAME")
	if len(name) > 0 {
		msg = strings.Replace(msg, "chifra ", "", -1) + "_" + name
	}
	// nItems := 0
	fmt.Printf("PERF,%s,%dms\n", msg, utils.Max(1, since.Milliseconds()))
}
