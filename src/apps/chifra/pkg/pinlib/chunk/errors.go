// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunk

import "fmt"

// Errors that can happen when validating and saving a file

type ErrSavingCorruptedDownload struct {
	fileName string
	expected int64
	read     int64
}

func (err *ErrSavingCorruptedDownload) Error() string {
	return fmt.Sprintf("file corrupted: %s (expected %d but got %d bytes)", err.fileName, err.expected, err.read)
}

type ErrSavingCannotDecompress struct {
	fileName string
	reason   error
}

func (err *ErrSavingCannotDecompress) Error() string {
	return fmt.Sprintf("error unpacking %s: %s", err.fileName, err.reason)
}

type ErrSavingCreateFile struct {
	fileName string
	reason   error
}

func (err *ErrSavingCreateFile) Error() string {
	return fmt.Sprintf("error creating file %s: %s", err.fileName, err.reason)
}

type ErrSavingCopy struct {
	fileName string
	reason   error
}

func (err *ErrSavingCopy) Error() string {
	return fmt.Sprintf("error copying %s: %s", err.fileName, err.reason)
}
