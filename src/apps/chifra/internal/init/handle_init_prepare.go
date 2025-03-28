// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package initPkg

import (
	"encoding/binary"
	"fmt"
	"os"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ranges"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

// prepareDownloadList returns a list of chunks that need to be modified in some way. There are three cases:
//
//  1. The chunk is on disc and agrees with the manifest in fileSize, magic number, and hash. In this
//     case, we do nothing.
//  2. The chunk is on disc but does not agree with the manifest for one of the above reasons. In this
//     case, we delete the chunk from disc and add it to the download list.
//  3. The chunk is not on disc. In this case, we add it to the download list.
//
// Note that in some cases, one part of a chunk may be valid while another part is not. For example,
// the index portion of a chunk may be valid, but the bloom filter may not be. In this case, we delete
// the entire chunk from disc and add it to the download list.
//
// If DryRun is true, then we do not delete anything from disc, nor do we add anything to the download list,
// but we do report what would have happened.
//
// Upon return, if a chunk is in the download list, then either its indexHash, its bloomHash, or both contains
// the IPFS hash that needs to be downloaded. Any chunks that are not in the download list are valid and remain
// on disc.
func (opts *InitOptions) prepareDownloadList(chain string, man *manifest.Manifest, blockNums []base.Blknum) ([]types.ChunkRecord, int, int, error) {
	// The list of files on disc that need to be removed because they are invalid in some way or not in the manifest
	deleteMap := make(map[ranges.FileRange]InitReason, len(man.Chunks))

	// The list of files in the manifest but not on disc so they need to be downloaded
	downloadMap := make(map[ranges.FileRange]InitReason, len(man.Chunks))

	// The list of files that are on disc and later than the latest entry in the manifest. These are
	// okay and should not be deleted.
	afterMap := make(map[ranges.FileRange]InitReason, len(man.Chunks))

	// We assume we're going to have download everything...
	for _, chunk := range man.Chunks {
		downloadMap[ranges.RangeFromRangeString(chunk.Range)] = FILE_MISSING
	}

	// Visit each chunk on disc. If the chunk belongs and is of the right size and shape, mark it as OKAY,
	// otherwise mark it with its reason for being invalid.
	cleanIndex := func(walker *walk.CacheWalker, path string, first bool) (bool, error) {
		_ = walker
		_ = first
		// sanity...
		if path != index.ToBloomPath(path) {
			logger.Fatal("should not happen ==> we're spinning through the bloom filters")
		}

		// Is the on-disc chunk in the manifest?
		rng := ranges.RangeFromFilename(path)
		chunk := man.ChunkMap[rng.String()]

		if chunk != nil {
			// Is it valid?
			bloomStatus, indexStatus, err := isValidChunk(path, chunk.BloomSize, chunk.IndexSize, opts.All)
			if err != nil {
				if bloomStatus != FILE_ERROR && indexStatus != FILE_ERROR {
					logger.Fatal("should not happen ==> implementation error in cleanIndex")
				}
				return false, err // bubble the error up
			}

			if bloomStatus == OKAY && indexStatus == OKAY {
				// The chunk is valid. We don't need to download it or delete it
				downloadMap[rng] = OKAY
				return true, nil
			} else {
				// one or the other of them is invalid. We need to delete it and download it
				// Note: we don't need to delete it, it will get downloaded and overwritten
				if bloomStatus != OKAY {
					deleteMap[rng] = bloomStatus
					downloadMap[rng] = bloomStatus
				} else {
					deleteMap[rng] = indexStatus
					downloadMap[rng] = indexStatus
				}
			}

			return true, nil

		} else {
			lastInManifest := ranges.FileRange{}
			if len(man.Chunks) > 0 {
				lastChunk := man.Chunks[len(man.Chunks)-1]
				lastInManifest = ranges.RangeFromRangeString(lastChunk.Range)
			}

			// The chunk is on disc but not in the manifest. We need to delete it
			// unless it's after the latest chunk in the manifest, in which case
			// the user has presembled scraped it and we should leave it alone.
			if !rng.LaterThan(lastInManifest) {
				deleteMap[rng] = NOT_IN_MANIFEST
			} else {
				afterMap[rng] = AFTER_MANIFEST
			}
			return true, nil
		}
	}

	walker := walk.NewCacheWalker(
		chain,
		opts.Globals.TestMode,
		10, /* maxTests */
		cleanIndex,
	)

	if err := walker.WalkBloomFilters(blockNums); err != nil {
		return nil, 0, 0, err
	}

	nDeleted := 0
	for rng, reason := range deleteMap {
		indexPath := rng.RangeToFilename(chain)
		bloomPath := index.ToBloomPath(indexPath)
		indexExists := file.FileExists(indexPath)
		bloomExists := file.FileExists(bloomPath)
		if !opts.DryRun {
			if indexExists {
				logger.Info("Removing", indexPath)
				if err := os.Remove(indexPath); err != nil {
					return nil, 0, nDeleted, err
				}
				nDeleted++
			}
			if bloomExists {
				logger.Info("Removing", bloomPath)
				if err := os.Remove(bloomPath); err != nil {
					return nil, 0, nDeleted, err
				}
				nDeleted++
			}
		}
		if bloomExists || indexExists {
			opts.reportReason("chunk deleted", reason, rng.String())
		}
	}

	downloadList := make([]types.ChunkRecord, 0, len(man.ChunkMap))
	nToDownload := 0
	for _, chunk := range man.ChunkMap {
		rng := ranges.RangeFromRangeString(chunk.Range)
		if downloadMap[rng] == OKAY || rng.Last < opts.FirstBlock {
			continue
		}
		indexPath := rng.RangeToFilename(chain)
		bloomStatus, indexStatus, err := isValidChunk(index.ToBloomPath(indexPath), chunk.BloomSize, chunk.IndexSize, opts.All)
		if err != nil {
			return nil, 0, nDeleted, err
		}
		if bloomStatus == OKAY {
			// if its okay, we don't need to download it
			chunk.BloomHash = ""
			chunk.BloomSize = 0
		} else {
			nToDownload++
		}
		if indexStatus == OKAY {
			// if its okay, we don't need to download it
			chunk.IndexHash = ""
			chunk.IndexSize = 0
		} else {
			nToDownload++
		}
		downloadList = append(downloadList, *chunk)
		opts.reportReason("chunk downloaded", downloadMap[rng], rng.String())
	}

	for rng, reason := range afterMap {
		opts.reportReason("chunk scraped", reason, rng.String())
	}

	sort.Slice(downloadList, func(i, j int) bool {
		return downloadList[i].Range > downloadList[j].Range
	})

	return downloadList, nToDownload, nDeleted, nil
}

func (opts *InitOptions) reportReason(prefix string, status InitReason, path string) {
	verbose := opts.Globals.Verbose || opts.DryRun
	if !verbose {
		return
	}

	if status == OKAY || status == AFTER_MANIFEST {
		col := colors.BrightGreen
		rng := ranges.RangeFromFilename(path)
		msg := fmt.Sprintf("%schunk %s%s %s", col, Reasons[status], colors.Off, rng)
		logger.Info(msg)
	} else {
		col := colors.BrightMagenta
		if status == FILE_ERROR || status == NOT_IN_MANIFEST {
			col = colors.BrightRed
		} else if strings.Contains(path, string(os.PathSeparator)+"blooms"+string(os.PathSeparator)) {
			col = colors.BrightYellow
		}
		rng := ranges.RangeFromFilename(path)
		msg := fmt.Sprintf("%s%s [%s]%s %s", col, prefix, Reasons[status], colors.Off, rng)
		logger.Warn(msg)
	}
}

// isValidChunk validates the bloom file's header and the index if told to do so. Note that in all cases, it resolves both.
func isValidChunk(path string, bloomSize, indexSize int64, indexRequired bool) (InitReason, InitReason, error) {
	if path != index.ToBloomPath(path) {
		logger.Fatal("should not happen ==> only process bloom folder paths in isValidChunk")
	}

	var err error
	indexPath := index.ToIndexPath(path)

	// Resolve the status of the Bloom file first
	bloom := FILE_MISSING
	if file.FileExists(path) {
		bloom = checkSize(path, bloomSize)
		if bloom == OKAY {
			bloom, err = checkHeader(path)
		}
	}
	// The bloom filter is resolved.

	// Determine the status of the index (if it exists)
	idx := OKAY
	if !file.FileExists(indexPath) {
		if indexRequired || strings.Contains(indexPath, "000000000-000000000") {
			idx = FILE_MISSING
		}
	} else {
		idx = checkSize(indexPath, indexSize)
		if idx == OKAY {
			idx, err = checkHeader(indexPath)
		}
	}

	return bloom, idx, err
}

func checkSize(path string, expected int64) InitReason {
	if !file.FileExists(path) {
		logger.Fatal("should not happen ==> file existence already checked")
	}

	if file.FileSize(path) != expected {
		return WRONG_SIZE
	}

	return OKAY
}

func checkHeader(path string) (InitReason, error) {
	if !file.FileExists(path) {
		logger.Fatal("should not happen ==> file existence already checked")
	}

	ff, err := os.OpenFile(path, os.O_RDONLY, 0644)
	if err != nil {
		return FILE_ERROR, err
	}
	defer ff.Close()

	if path == index.ToBloomPath(path) {
		var magic uint16
		err = binary.Read(ff, binary.LittleEndian, &magic)
		if err != nil {
			return FILE_ERROR, err
		}
		if magic != file.SmallMagicNumber {
			return WRONG_MAGIC, nil
		}

		var hash base.Hash
		err = binary.Read(ff, binary.LittleEndian, &hash)
		if err != nil {
			return FILE_ERROR, err
		}
		if hash != base.BytesToHash(config.HeaderHash(config.ExpectedVersion())) {
			return WRONG_HASH, nil
		}

		return OKAY, nil

	} else if path == index.ToIndexPath(path) {
		var magic uint32
		err = binary.Read(ff, binary.LittleEndian, &magic)
		if err != nil {
			return FILE_ERROR, err
		}
		if magic != file.MagicNumber {
			return WRONG_MAGIC, nil
		}

		var hash base.Hash
		err = binary.Read(ff, binary.LittleEndian, &hash)
		if err != nil {
			return FILE_ERROR, err
		}
		if hash != base.BytesToHash(config.HeaderHash(config.ExpectedVersion())) {
			return WRONG_HASH, nil
		}

		return OKAY, nil

	} else {
		logger.Fatal("should not happen ==> unknown type in hasValidHeader")
		return OKAY, nil
	}
}

type InitReason int

const (
	OKAY InitReason = iota
	FILE_MISSING
	WRONG_SIZE
	WRONG_MAGIC
	WRONG_HASH
	FILE_ERROR
	NOT_IN_MANIFEST
	AFTER_MANIFEST
)

var Reasons = map[InitReason]string{
	OKAY:            "okay",
	FILE_ERROR:      "file error",
	FILE_MISSING:    "file missing",
	WRONG_SIZE:      "wrong size",
	WRONG_MAGIC:     "wrong magic number",
	WRONG_HASH:      "wrong header hash",
	NOT_IN_MANIFEST: "not in manifest",
	AFTER_MANIFEST:  "range after manifest",
}
