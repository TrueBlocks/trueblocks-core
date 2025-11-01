package namesPkg

import (
	"bufio"
	"bytes"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/output"
)

func BenchmarkNamesOptions_HandleShow_CommandLine(b *testing.B) {
	b.StopTimer()
	buf := bytes.NewBuffer([]byte{})
	w := bufio.NewWriter(buf)

	opts := defaultNamesOptions
	opts.Globals.Format = "txt"
	opts.Globals.Writer = w

	b.StartTimer()
	// Emptying in-memory cache simulates calling this code multiple times
	// on the command line
	names.ClearCache()
	rCtx := output.NewRenderContext()
	err := opts.HandleShow(rCtx)
	if err != nil {
		b.Fatal(err)
	}
}

func BenchmarkNamesOptions_HandleShow_Api(b *testing.B) {
	b.StopTimer()
	buf := bytes.NewBuffer([]byte{})
	w := bufio.NewWriter(buf)

	opts := defaultNamesOptions
	opts.Globals.Format = "txt"
	opts.Globals.Writer = w

	b.StartTimer()
	// Note: we are not calling names.ClearCache() here
	rCtx := output.NewRenderContext()
	err := opts.HandleShow(rCtx)
	if err != nil {
		b.Fatal(err)
	}
}
