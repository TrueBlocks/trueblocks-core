// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package index

import "fmt"

// Errors that can happen when validating and saving a file

type errSavingCorruptedDownload struct {
	fileName string
	expected int64
	read     int64
}

func (err *errSavingCorruptedDownload) Error() string {
	return fmt.Sprintf("file corrupted: %s (expected %d but got %d bytes)", err.fileName, err.expected, err.read)
}

type errSavingCannotDecompress struct {
	fileName string
	reason   error
}

func (err *errSavingCannotDecompress) Error() string {
	return fmt.Sprintf("error unpacking %s: %s", err.fileName, err.reason)
}

type errSavingCreateFile struct {
	fileName string
	reason   error
}

func (err *errSavingCreateFile) Error() string {
	return fmt.Sprintf("error creating file %s: %s", err.fileName, err.reason)
}

type errSavingCopy struct {
	fileName string
	reason   error
}

func (err *errSavingCopy) Error() string {
	return fmt.Sprintf("error copying %s: %s", err.fileName, err.reason)
}
