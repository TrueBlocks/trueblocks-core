package file

import (
	"io"
	"log"
	"os"
)

// GetOutputFileWriter returns a writer which either the default one (same as passed
// to this function) or, if filePath is non-empty, one that writes to that file.
// If there is an error, log.Fatal is called, because there really is no good way
// to recover. Plus, output file is disabled in server, so it is safe to exit.
func GetOutputFileWriter(filePath string, defaultWriter io.Writer) io.Writer {
	if filePath == "" {
		// If there's no file path, we can just return the default writer
		return defaultWriter
	}

	// Otherwise, let's try to create the file
	file, err := os.Create(filePath)
	if err != nil {
		log.Fatal(err)
	}
	return file
}
