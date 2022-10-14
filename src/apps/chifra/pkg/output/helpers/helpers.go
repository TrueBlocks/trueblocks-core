package outputHelpers

import (
	"io"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/spf13/cobra"
)

func PreRunWithJsonWriter(getOptions func() *globals.GlobalOptions) func(cmd *cobra.Command, args []string) {
	return func(cmd *cobra.Command, args []string) {
		opts := getOptions()
		var outputWriter io.Writer
		outputWriter = os.Stdout
		if opts.OutputFn != "" {
			outputWriter = opts.GetOutputFileWriter()
		}

		if opts.Format == "json" || opts.ShowRaw {
			jw := output.NewDefaultJsonWriter(outputWriter, !opts.ShowRaw)
			opts.Writer = jw

		} else {
			opts.Writer = outputWriter
		}
	}
}

func PostRunWithJsonWriter(getOptions func() *globals.GlobalOptions) func(cmd *cobra.Command, args []string) {
	return func(cmd *cobra.Command, args []string) {
		opts := getOptions()
		w := opts.Writer
		jw, ok := w.(*output.JsonWriter)
		if !ok || opts.MixedFormats {
			return
		}
		jw.Close()
	}
}

func CloseJsonWriterIfNeeded(getOptions func() *globals.GlobalOptions) {
	opts := getOptions()
	if (opts.Format == "json" || opts.ShowRaw) && opts.MixedFormats {
		// If MixedFormats is set, we don't want to pass JsonWriter to the next line of --file.
		// We want to Close it here, instead
		opts.Writer.(*output.JsonWriter).Close()
	}
}
