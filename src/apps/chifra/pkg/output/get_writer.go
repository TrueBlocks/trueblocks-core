package output

import (
	"io"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// GetOutputFileWriter returns a writer which either the default one (same as passed
// to this function) or, if filePath is non-empty, one that writes to that file.
// If there is an error, logger.Panic is called, because there really is no good way
// to recover. Plus, output file is disabled in server, so it is safe to exit.
func (opts *OutputOptions) GetOutputFileWriter() io.Writer {
	if opts.OutputFn == "" {
		// If there's no output filename, we return the default writer (standard out).
		return opts.Writer
	}

	var file *os.File
	var err error
	if opts.Append {
		file, err = os.OpenFile(opts.OutputFn, os.O_WRONLY|os.O_CREATE|os.O_APPEND, 0644)
	} else {
		file, err = os.OpenFile(opts.OutputFn, os.O_WRONLY|os.O_CREATE|os.O_TRUNC, 0644)
	}
	if err != nil {
		logger.Panic(err)
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
