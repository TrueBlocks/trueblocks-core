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
	"blocks":   os.Getenv("TEST_TEST_ONLY") == "true",
	"receipts": true,
	"when":     true,
}

func SetEnabledForCmds(cmd string, enabled bool) {
	enabledForCmds[cmd] = enabled
}

func PreRunWithJsonWriter(cmdName string, getOptions func() *globals.GlobalOptions) func(cmd *cobra.Command, args []string) {
	return func(cmd *cobra.Command, args []string) {
		opts := getOptions()
		var outputWriter io.Writer
		// Prepare the default writer (stdout or --output file)
		outputWriter = os.Stdout
		if opts.OutputFn != "" {
			outputWriter = opts.GetOutputFileWriter()
		}

		// If we need to output JSON, init JsonWriter...
		if enabledForCmds[cmdName] && (opts.Format == "json" || opts.ShowRaw) {
			jw := output.NewDefaultJsonWriter(outputWriter, !opts.ShowRaw)
			opts.Writer = jw
		} else {
			// ...or set the default writer as global writer for the current command
			// invocation
			opts.Writer = outputWriter
		}
	}
}

func PostRunWithJsonWriter(getOptions func() *globals.GlobalOptions) func(cmd *cobra.Command, args []string) {
	return func(cmd *cobra.Command, args []string) {
		opts := getOptions()
		w := opts.Writer
		// Try to cast the global writer to JsonWriter
		jw, ok := w.(*output.JsonWriter)
		if !ok {
			// If it's not JsonWriter, do nothing
			return
		}
		// If it is JsonWriter, close it (print closing brackets)
		jw.Close()
	}
}

// SetWriterForCommand sets the writer for currently running command, but only if
// we are running with --file
func SetWriterForCommand(cmdName string, opts *globals.GlobalOptions) {
	// This function should be only enabled for commands using output.Stream*
	// functions for producing their output
	if !enabledForCmds[cmdName] {
		return
	}

	// Run only in --file mode. Without --file, the default setup is OK.
	// TODO: BOGUS - THIS WILL BE REMOVED TEST_TEST_ONLY
	if os.Getenv("TEST_TEST_ONLY") != "true" {
		if opts.File == "" {
			return
		}
	}

	// Try to cast the default writer to JsonWriter
	jw, ok := opts.Writer.(*output.JsonWriter)
	wantsJson := (opts.Format == "json" || opts.ShowRaw)

	// Global writer is set to JsonWriter, but this command wants to output
	// a different format. We have to close JsonWriter so that closing brackets
	// are printed and switch global writer to io.Writer
	if ok && !wantsJson {
		jw.Close()
		// Set global writer to default
		opts.Writer = os.Stdout
		opts.Writer = opts.GetOutputFileWriter()
		return
	}

	// Global writer is NOT JsonWriter, but this command wants to output
	// JSON, so we have to create new JsonWriter and set is as the global
	// writer
	if !ok && wantsJson {
		w := opts.GetOutputFileWriter()
		jw := output.NewDefaultJsonWriter(w, !opts.ShowRaw)
		opts.Writer = jw
	}

	// Global writer is not JsonWriter and the command doesn't want to
	// output JSON, so we only have to set global writer to a default one
	if !ok && !wantsJson {
		opts.Writer = opts.GetOutputFileWriter()
	}
}

// InitJsonWriterApi inits JsonWriter for API responses
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

// CloseJsonWriterIfNeededApi will close JsonWriter if the format is json
func CloseJsonWriterIfNeededApi(cmdName string, err error, opts *globals.GlobalOptions) {
	if enabledForCmds[cmdName] && opts.Format == "json" && err == nil {
		opts.Writer.(*output.JsonWriter).Close()
	}
}
