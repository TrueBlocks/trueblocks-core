package logger

import (
	"fmt"
	"os"
	"sync/atomic"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
)

type ProgressBar struct {
	prefix    string // the string to display first
	percent   int64  // progress percentage
	cur       int64  // current progress
	total     int64  // total value for progress
	graph     string // the actual progress bar to be printed
	ch        string // the fill value for progress bar
	enabled   bool   // enable progress bar
	expanding bool   // the bar doesn't know how many object there will be, so grows
	startTime time.Time
}

func NewBar(prefix string, enabled bool, total int64) (bar *ProgressBar) {
	return NewBarWithGraphic(prefix, enabled, false, total, ".")
}

func NewExpandingBar(prefix string, enabled bool, total int64) (bar *ProgressBar) {
	return NewBarWithGraphic(prefix, enabled, true, total, ".")
}

func NewBarWithStart(prefix string, enabled bool, start, total int64) (bar *ProgressBar) {
	bar = NewBar(prefix, enabled, total)
	bar.cur = start
	bar.startTime = time.Now()
	return bar
}

func NewBarWithGraphic(prefix string, enabled bool, of bool, total int64, ch string) (bar *ProgressBar) {
	if ch == "" {
		ch = "."
	}

	bar = new(ProgressBar)
	bar.prefix = prefix
	bar.cur = 0
	bar.total = total
	bar.ch = ch
	bar.expanding = of
	bar.percent = int64(float32(bar.cur) * 100 / float32(bar.total))
	for i := 0; i < int(bar.percent); i += 2 {
		bar.graph += bar.ch // initial progress position
	}
	bar.enabled = enabled
	return bar
}

func (bar *ProgressBar) Tick() {
	atomic.AddInt64(&bar.cur, 1)
	if bar.expanding && bar.cur >= bar.total {
		bar.total = bar.total * 2 // grow by 50% but at least 100
		bar.cur = bar.cur / 2
		bar.percent = int64(float32(bar.cur) * 100 / float32(bar.total))
		bar.graph = ""
		for i := 0; i < int(bar.percent); i += 2 {
			bar.graph += bar.ch // initial progress position
		}
	}
	bar.display()
}

func (bar *ProgressBar) Finish(newLine bool) time.Duration {
	if bar.enabled {
		if bar.expanding {
			bar.total = (bar.total / 2) + bar.cur
			bar.cur = bar.total
			bar.percent = 100
		}
		bar.graph = ""
		for i := 0; i < 100; i += 2 {
			bar.graph += bar.ch
		}
		bar.display()
		if newLine {
			fmt.Fprintf(os.Stderr, "\n")
		}
	}
	return time.Since(bar.startTime)
}

func (bar *ProgressBar) display() {
	if !bar.enabled {
		return
	}

	last := bar.percent
	if bar.total == 0 {
		bar.percent = 99
	} else {
		bar.percent = int64(float32(bar.cur) * 100 / float32(bar.total))
	}
	if bar.percent != last && bar.percent%2 == 0 {
		bar.graph += bar.ch
	}
	if bar.total == 0 {
		bar.percent = 100
	}
	timeDatePart := "DATE|TIME"
	if timingMode {
		now := time.Now()
		timeDatePart = now.Format("02-01|15:04:05.000")
	}
	ofMarker := ""
	if bar.expanding {
		ofMarker = "?"
	}
	fmt.Fprintf(os.Stderr, "\r%s[%s] [%s%-50s%s]%3d%% %5d/% 5d %s\r",
		severityToLabel[progress],
		timeDatePart,
		colors.BrightGreen,
		bar.graph,
		colors.Off,
		bar.percent,
		bar.cur,
		bar.total,
		colors.BrightGreen+ofMarker+bar.prefix+colors.Off)
}
