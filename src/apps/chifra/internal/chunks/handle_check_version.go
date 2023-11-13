// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

// CheckInternal reads the header of each chunk on disc looking for the Magic number and
// the hash of the spec version for expected values.
func (opts *ChunksOptions) CheckVersion(fileNames []string, blockNums []uint64, report *simpleReportCheck) error {
	for _, fileName := range fileNames {
		opts.checkIndexChunkInternal(fileName, true /* checkVersion */, report)
		// opts.checkBloomInternal(testId, fileName, report)
	}
	return nil
}
