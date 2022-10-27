package output

import (
	"io"
	"log"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// GetOutputFileWriter returns a writer which either the default one (same as passed
// to this function) or, if filePath is non-empty, one that writes to that file.
// If there is an error, log.Fatal is called, because there really is no good way
// to recover. Plus, output file is disabled in server, so it is safe to exit.
func (opts *OutputOptions) GetOutputFileWriter() io.Writer {
	if opts.OutputFn == "" {
		// If there's no output filename, we return the default writer (standard out).
		return opts.Writer
	}

	// Note on the command line, the OutputFn value can come from either the --output option
	// (in which case the user is telling us exactly which file to use) or the `--to_file` option
	// in which case we create a temporary file. In both cases, the we return the name of the file
	// and the caller is responisbile to remove the file when it is no longer needed.
	// In API mode, --output is disallowed, but `--to_file` will work even though the
	// file may not be available (because the caller is on a different machine). I'm not sure
	// why it works this way -- if the caller is on the same machine, why can't they name thier file?
	var file *os.File
	var err error
	if opts.Append {
		file, err = os.OpenFile(opts.OutputFn, os.O_WRONLY|os.O_CREATE|os.O_APPEND, 0644)
	} else {
		file, err = os.OpenFile(opts.OutputFn, os.O_WRONLY|os.O_CREATE|os.O_TRUNC, 0644)
	}
	if err != nil {
		// TODO:(dawid) - what happens in the server case?
		log.Fatal(err)
	}
	return file
}

// IsApiMode return true if `w` is successfully cast into a `http.ResponseWriter`
func (opts *OutputOptions) IsApiMode() bool {
	w, ok := opts.Writer.(*JsonWriter)
	if !ok {
		return utils.IsServerWriter(opts.Writer)
	}

	return utils.IsServerWriter(*w.GetOutputWriter())
}
