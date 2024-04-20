// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// CheckHashes looks at all the hashes in both the locally cached manifest and the manifest retrieved from
// the smart contract and simply checks the lengths of the bloom and index IPFS hashes.
func (opts *ChunksOptions) CheckHashes(cacheMan *manifest.Manifest, contractMan *manifest.Manifest, report *types.ReportCheck) error {
	if err := opts.checkHashes("cache", cacheMan, report); err != nil {
		return err
	}

	if err := opts.checkHashes("contract", contractMan, report); err != nil {
		return err
	}

	return nil
}

func (opts *ChunksOptions) checkHashes(which string, man *manifest.Manifest, report *types.ReportCheck) error {
	for _, chunk := range man.Chunks {
		report.VisitedCnt++
		report.CheckedCnt++
		if len(chunk.BloomHash) != 46 {
			msg := fmt.Sprintf("%s: Invalid BloomHash (%s) in %s", chunk.Range, chunk.BloomHash, which)
			if len(chunk.BloomHash) == 0 {
				msg = fmt.Sprintf("%s: Empty BloomHash in %s", chunk.Range, which)
			}
			report.MsgStrings = append(report.MsgStrings, msg)
		} else {
			report.PassedCnt++
		}

		report.VisitedCnt++
		report.CheckedCnt++
		if len(chunk.IndexHash) != 46 {
			msg := fmt.Sprintf("%s: Invalid IndexHash (%s) in %s", chunk.Range, chunk.IndexHash, which)
			if len(chunk.IndexHash) == 0 {
				msg = fmt.Sprintf("%s: Empty IndexHash in %s", chunk.Range, which)
			}
			report.MsgStrings = append(report.MsgStrings, msg)
		} else {
			report.PassedCnt++
		}
	}

	return nil
}
