package file

import (
	"archive/tar"
	"compress/gzip"
	"fmt"
	"io"
	"os"
	"strings"
)

// CreateArchive creates new Writers for gzip and tar. These writers are chained. Writing
// to the tar writer will write to the gzip writer which in turn will write to the "buf" writer
func CreateArchive(files []string, buf io.Writer, remove bool, relativeTo string) error {
	gw := gzip.NewWriter(buf)
	defer gw.Close()
	tw := tar.NewWriter(gw)
	defer tw.Close()

	// Iterate over files and add them to the tar archive
	for _, fn := range files {
		err := addToArchive(tw, fn, relativeTo)
		if err != nil {
			return err
		}
	}

	for _, fn := range files {
		if remove && !Remove(fn) {
			return fmt.Errorf("could not remove file %s", fn)
		}
	}

	return nil
}

func addToArchive(tw *tar.Writer, filename, relativeTo string) error {
	// Open the file which will be written into the archive
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	// Get FileInfo about our file providing file size, mode, etc.
	info, err := file.Stat()
	if err != nil {
		return err
	}

	// Create a tar Header from the FileInfo data
	header, err := tar.FileInfoHeader(info, info.Name())
	if err != nil {
		return err
	}

	// Use full path as name (FileInfoHeader only takes the basename)
	// If we don't do this the directory strucuture would not be preserved
	// https://golang.org/src/archive/tar/common.go?#L626. If we're told
	// where to write it, write it there
	header.Name = filename
	if len(relativeTo) > 0 {
		header.Name = strings.Replace(filename, relativeTo, "", -1)
	}

	// Write file header to the tar archive
	err = tw.WriteHeader(header)
	if err != nil {
		return err
	}

	// Copy file content to tar archive
	_, err = io.Copy(tw, file)
	if err != nil {
		return err
	}

	return nil
}
