package logger

import (
	"fmt"
	"os"
	"sync/atomic"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
)

type ProgressBar struct {
	prefix  string // the string to display first
	percent int64  // progress percentage
	cur     int64  // current progress
	total   int64  // total value for progress
	graph   string // the actual progress bar to be printed
	ch      string // the fill value for progress bar
	enabled bool   // enable progress bar
}

func NewBar(prefix string, enabled bool, total int64) (bar *ProgressBar) {
	return NewBarWithGraphic(prefix, enabled, total, ".")
}

func NewBarWithGraphic(prefix string, enabled bool, total int64, ch string) (bar *ProgressBar) {
	if ch == "" {
		ch = "."
	}

	bar = new(ProgressBar)
	bar.prefix = prefix
	bar.cur = 0
	bar.total = total
	bar.ch = ch
	bar.percent = int64(float32(bar.cur) * 100 / float32(bar.total))
	for i := 0; i < int(bar.percent); i += 2 {
		bar.graph += bar.ch // initial progress position
	}
	bar.enabled = enabled
	return bar
}

func (bar *ProgressBar) Tick() {
	atomic.AddInt64(&bar.cur, 1)
	bar.display()
}

func (bar *ProgressBar) Finish(newLine bool) {
	if !bar.enabled {
		return
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
	fmt.Fprintf(os.Stderr, "\r%s[%s] [%s%-50s%s]%3d%% %5d/% 5d %s\r",
		severityToLabel[progress],
		timeDatePart,
		colors.BrightGreen,
		bar.graph,
		colors.Off,
		bar.percent,
		bar.cur,
		bar.total,
		colors.BrightGreen+bar.prefix+colors.Off)
}
