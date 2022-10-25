package outputHelpers

import (
	"io"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/spf13/cobra"
)

var enabledForCmds = map[string]bool{
	"receipts": true,
}

func PreRunWithJsonWriter(cmdName string, getOptions func() *globals.GlobalOptions) func(cmd *cobra.Command, args []string) {
	return func(cmd *cobra.Command, args []string) {
		opts := getOptions()
		var outputWriter io.Writer
		outputWriter = os.Stdout
		if opts.OutputFn != "" {
			outputWriter = opts.GetOutputFileWriter()
		}

		if enabledForCmds[cmdName] && (opts.Format == "json" || opts.ShowRaw) {
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

func InitJsonWriterApi(cmdName string, w io.Writer, opts *globals.GlobalOptions) {
	if enabledForCmds[cmdName] && opts.Format == "json" {
		jw := output.NewDefaultJsonWriter(w, false)
		jw.ShouldWriteMeta = true
		jw.GetMeta = func() (*rpcClient.MetaData, error) {
			return rpcClient.GetMetaData(opts.Chain, opts.OutputOptions.TestMode)
		}
		opts.Writer = jw
	}
}

// InitJsonWriter inits JsonWriter if neccessary
func InitJsonWriter(cmdName string, opts *globals.GlobalOptions) {
	if !enabledForCmds[cmdName] {
		return
	}

	if opts.Writer == nil {
		opts.Writer = os.Stdout
	}

	// When running with --file, it may happen that the default writer
	// is not JsonWriter, but the user wants JSON output for the given
	// line of command file. In such a case, we need to create JsonWriter.
	// We also want to hook it into whatever `GetOutputFileWriter` returns
	// for --output support.
	_, ok := opts.Writer.(*output.JsonWriter)
	if !ok && (opts.Format == "json" || opts.ShowRaw) {
		w := opts.GetOutputFileWriter()
		jw := output.NewDefaultJsonWriter(w, !opts.ShowRaw)
		opts.Writer = jw
	}
	if !ok && !(opts.Format == "json" || opts.ShowRaw) {
		opts.Writer = opts.GetOutputFileWriter()
	}
}

// CloseFileJsonWriterIfNeeded closes JsonWriter before a command executes. This way,
// if we are switching formats, we will still close JSON result object
func CloseFileJsonWriterIfNeeded(cmdName string, opts *globals.GlobalOptions) {
	jw, ok := opts.Writer.(*output.JsonWriter)

	if enabledForCmds[cmdName] && opts.File != "" && !(opts.Format == "json" || opts.ShowRaw) && ok {
		jw.Close()
		// Set global writer to default
		opts.Writer = os.Stdout
		opts.Writer = opts.GetOutputFileWriter()
	}
}

func CloseJsonWriterIfNeeded(cmdName string, opts *globals.GlobalOptions) {
	if enabledForCmds[cmdName] && (opts.Format == "json" || opts.ShowRaw) && opts.MixedFormats {
		// If MixedFormats is set, we don't want to pass JsonWriter to the next line of --file.
		// We want to Close it here, instead
		opts.Writer.(*output.JsonWriter).Close()
	}
}

func CloseJsonWriterIfNeededApi(cmdName string, err error, opts *globals.GlobalOptions) {
	if enabledForCmds[cmdName] && opts.Format == "json" && err == nil {
		opts.Writer.(*output.JsonWriter).Close()
	}
}
