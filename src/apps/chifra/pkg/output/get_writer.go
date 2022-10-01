package output

import (
	"io"
	"log"
	"net/http"
	"os"
)

// GetOutputFileWriter returns a writer which either the default one (same as passed
// to this function) or, if filePath is non-empty, one that writes to that file.
// If there is an error, log.Fatal is called, because there really is no good way
// to recover. Plus, output file is disabled in server, so it is safe to exit.
func (opts *OutputOptions) GetOutputFileWriter() io.Writer {
	if opts.OutputFn == "" || opts.IsApiMode() {
		// If there's no file path, we can just return the default writer
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
		// TODO:(dshlacta) - what happens in the server case?
		log.Fatal(err)
	}
	return file
}

// IsApiMode tries to cast `w` into `http.ResponseWriter`
// and returns true if the cast was successful
func (opts *OutputOptions) IsApiMode() bool {
	_, ok := opts.Writer.(http.ResponseWriter)
	return ok
}
