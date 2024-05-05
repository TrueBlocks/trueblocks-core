// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// CheckInternal reads the header of each chunk on disc looking for the Magic number and
// the hash of the spec version for expected values.
func (opts *ChunksOptions) CheckVersion(fileNames []string, blockNums []base.Blknum, report *types.ReportCheck) error {
	for _, fileName := range fileNames {
		opts.checkIndexChunkInternal(fileName, true /* checkVersion */, report)
		// opts.checkBloomInternal(testId, fileName, report)
	}
	return nil
}
