// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	shell "github.com/ipfs/go-ipfs-api"
)

// CheckHashes looks at all the hashes in both the locally cached manifest and the manifest retrieved from
// the smart contract and simply checks the lengths of the bloom and index IPFS hashes.
func (opts *ChunksOptions) CheckHashes(cacheMan *manifest.Manifest, contractMan *manifest.Manifest, report *simpleReportCheck) error {
	if err := opts.checkHashes("cache", cacheMan, report); err != nil {
		return err
	}

	if err := opts.checkHashes("contract", contractMan, report); err != nil {
		return err
	}

	return nil
}

func (opts *ChunksOptions) checkHashes(which string, man *manifest.Manifest, report *simpleReportCheck) error {

	var sh *shell.Shell
	if opts.Deep {
		sh = shell.NewShell("localhost:5001")
	}
	canDigDeep := sh != nil && opts.Deep && which == "cache"

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
			if canDigDeep {
				report.VisitedCnt++
				report.CheckedCnt++
				key, size, err := sh.BlockStat(chunk.BloomHash.String())
				if err != nil {
					msg := fmt.Sprintf("%s: BloomHash (%s) not pinned", chunk.Range, chunk.BloomHash)
					report.MsgStrings = append(report.MsgStrings, msg)
				} else {
					report.PassedCnt++
				}
				logger.Progress(true, chunk.Range, key, size)
			}
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
			if canDigDeep {
				report.VisitedCnt++
				report.CheckedCnt++
				key, size, err := sh.BlockStat(chunk.IndexHash.String())
				if err != nil {
					msg := fmt.Sprintf("%s: IndexHash (%s) not pinned", chunk.Range, chunk.IndexHash)
					report.MsgStrings = append(report.MsgStrings, msg)
				} else {
					report.PassedCnt++
				}
				logger.Progress(true, chunk.Range, key, size)
			}
		}
	}

	return nil
}
