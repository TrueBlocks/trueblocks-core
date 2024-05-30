package logger

import (
	"fmt"
	"sync/atomic"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
)

type BarType int

const (
	Fixed BarType = iota
	Expanding
)

type BarOptions struct {
	Enabled     bool    // enable progress bar
	Type        BarType // if Expanding, the bar doesn't know how many object there will be, so grows
	Prefix      string  // the string to display first
	Fill        string  // the fill value for progress bar
	Start       int64   // the starting value (defaults to zero)
	Total       int64   // the total (possibly estimated for Expanding) number of objects
	PrefixColor string  // the color of the prefix
}

type ProgressBar struct {
	BarOptions
	graphic   string // the actual progress bar to be printed
	percent   int64  // progress percentage
	cur       int64  // current progress
	startTime time.Time
}

func NewBar(opts BarOptions) (bar *ProgressBar) {
	if opts.Fill == "" {
		opts.Fill = "."
	}

	bar = new(ProgressBar)
	bar.Prefix = opts.Prefix
	bar.Fill = opts.Fill
	bar.Enabled = opts.Enabled
	bar.Type = opts.Type
	bar.Start = opts.Start
	bar.Total = opts.Total

	bar.cur = opts.Start
	bar.percent = int64(float32(bar.cur) * 100 / float32(bar.Total))
	for i := 0; i < int(bar.percent); i += 2 {
		bar.graphic += bar.Fill // initial progress position
	}

	if opts.Type == Expanding && bar.Total == 0 {
		bar.Total = 1
	}

	return bar
}

func (bar *ProgressBar) Bump() {
	atomic.AddInt64(&bar.cur, 1)
}

func (bar *ProgressBar) Tick() {
	atomic.AddInt64(&bar.cur, 1)
	if bar.Type == Expanding && bar.cur >= bar.Total {
		bar.Total = bar.Total * 2 // grow by 50% but at least 100
		bar.cur = bar.cur / 2
		bar.percent = int64(float32(bar.cur) * 100 / float32(bar.Total))
		bar.graphic = ""
		for i := 0; i < int(bar.percent); i += 2 {
			bar.graphic += bar.Fill // initial progress position
		}
	}
	bar.display()
}

func (bar *ProgressBar) Finish(newLine bool) time.Duration {
	if bar.Enabled && loggerWriter != nil {
		atomic.StoreInt64(&bar.Total, bar.cur)
		if bar.Type == Expanding {
			bar.cur = bar.Total
			bar.percent = 100
		}
		bar.graphic = ""
		for i := 0; i < 100; i += 2 {
			bar.graphic += bar.Fill
		}
		bar.display()
		if newLine {
			fmt.Fprintf(loggerWriter, "\n")
		}
	}
	return time.Since(bar.startTime)
}

func (bar *ProgressBar) display() {
	if bar.Enabled && loggerWriter != nil {
		last := bar.percent
		if bar.Total == 0 {
			bar.percent = 99
		} else {
			bar.percent = int64(float32(bar.cur) * 100 / float32(bar.Total))
		}
		if bar.percent != last && bar.percent%2 == 0 {
			bar.graphic += bar.Fill
		}
		if bar.Total == 0 {
			bar.percent = 100
		}
		timeDatePart := "DATE|TIME"
		if timingMode {
			now := time.Now()
			timeDatePart = now.Format("02-01|15:04:05.000")
		}
		ofMarker := ""
		fmt.Fprintf(loggerWriter, "\r%s[%s] [%s%-50s%s]%3d%% %5d/% 5d %s\r",
			severityToLabel[progress],
			timeDatePart,
			colors.BrightGreen,
			bar.graphic,
			colors.Off,
			bar.percent,
			bar.cur,
			bar.Total,
			colors.BrightGreen+ofMarker+bar.Prefix+colors.Off)
	}
}
