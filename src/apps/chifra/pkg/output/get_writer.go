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
		// If there's no file path, we can just return the default writer. Note that
		// even in the API mode, the OutputFn may be non-empty. In that case (i.e. --to_file)
		// we write to a file and return the file name.
		return opts.Writer
	}

	// Otherwise, let's try to create the file
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

// IsApiMode return true if `w` is successfully case into a `http.ResponseWriter`
func (opts *OutputOptions) IsApiMode() bool {
	return utils.IsServerWriter(opts.Writer)
}
