// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package initPkg

import (
	"fmt"
	"os"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/paths"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

var verbose bool

// prepareDownloadList returns a list of chunks (or partial chunks) that need to be updated. Upon return, if a chunk is in this list,
// its bloomHash, its indexHash, or both contains the IPFS hash that needst to be download. Any chunks (or portions thereof) that are
// not in the list are valid and remain on disc. That means that the chunk is the manifest, has the right file size, has a valid header,
// and is of the rigth version.
func (opts *InitOptions) prepareDownloadList(chain string, man *manifest.Manifest, blockNums []uint64) ([]manifest.ChunkRecord, int) {
	verbose = opts.Globals.Verbose
	walker := index.NewIndexWalker(
		opts.Globals.Chain,
		opts.Globals.TestMode,
		10, /* maxTests */
		opts,
		cleanIndex,
		man,
	)

	if err := walker.WalkIndexFiles(paths.Index_Bloom, blockNums); err != nil {
		return man.Chunks, len(man.Chunks)
	}

	chunksNeeded := make([]manifest.ChunkRecord, 0, len(man.Chunks))
	defer func() {
		// Sort the downloads in reverse order, so the process happens from latest block to earliest
		sort.Slice(chunksNeeded, func(i, j int) bool {
			return chunksNeeded[i].Range > chunksNeeded[j].Range
		})
	}()

	nCorrections := 0
	for _, chunk := range man.ChunkMap {
		if chunk != nil {
			rng := paths.RangeFromRangeString(chunk.Range)
			_, indexPath := rng.RangeToFilename(opts.Globals.Chain, paths.Index_Final)
			if !opts.All && !file.FileExists(indexPath) {
				chunk.IndexHash = ""
				chunk.IndexSize = 0
			}
			if chunk.BloomHash != "" {
				nCorrections++
			}
			if chunk.IndexHash != "" {
				nCorrections++
			}

			chunksNeeded = append(chunksNeeded, *chunk)
			if opts.Globals.Verbose {
				// TODO: BOGUS - REMOVE NEXT TWO LINES
				file.AppendToAsciiFile("./one/notes", fmt.Sprintf("1--The chunk needs to be downloaded %v\n", chunk))
				fmt.Println(colors.BrightBlack, "1--The chunk needs to be downloaded", colors.Off, chunk)
			}
		}
	}

	return chunksNeeded, nCorrections
}

// cleanIndex checks that the file is in the manifest, has a valid header, is of the correct version, and is of the right size. If any of those
// are not true, we remove that file and add it to the download list. Important note: we only walk the bloom filter folder because the bloom
// filters are required. If the user has specified `--all`, we insist that the corresponding index portion is also present and valid. If the
// user has not specified `--all`, then we check the index portion, only if it exists, and then only for the correct header and file size. If a
// bloom filter is present on disc, but not in the manifest, then we delete both the bloom filte and the corresponding index portion if it exists.
func cleanIndex(walker *index.IndexWalker, path string, first bool) (bool, error) {
	var castOk bool
	var opts *InitOptions
	if opts, castOk = walker.GetOpts().(*InitOptions); !castOk {
		logger.Fatal("should not happen ==> cannot cast ChunksOptions in cleanIndex")
		return false, nil
	}

	var man *manifest.Manifest
	if man, castOk = walker.GetData().(*manifest.Manifest); !castOk {
		logger.Fatal("should not happen ==> cannot cast manifest.Manifest in cleanIndex")
		return false, nil
	}

	if path != paths.ToBloomPath(path) {
		logger.Fatal("should not happen ==> we're spinning through the bloom filters")
	}

	rng := paths.RangeFromFilename(path)
	chunk := man.ChunkMap[rng.String()]
	if chunk != nil {
		ch := validate.ChunkSizes{BloomSize: chunk.BloomSize, IndexSize: chunk.IndexSize}
		bloomStatus, indexStatus, err := validate.IsValidChunk(path, ch, opts.All)
		switch bloomStatus {
		case validate.OKAY:
			chunk.BloomHash = "" // we don't have to download it
			chunk.BloomSize = 0
			reportReason("The bloom file", bloomStatus, path)
		case validate.FILE_ERROR:
			return false, err // bubble the error up
		case validate.FILE_MISSING:
			fallthrough
		case validate.WRONG_SIZE:
			fallthrough
		case validate.WRONG_MAGIC:
			fallthrough
		case validate.WRONG_HASH:
			reportReason("The bloom file", bloomStatus, path)
		default:
			logger.Fatal("should not happen ==> unknown return from IsValidChunk")
		}

		switch indexStatus {
		case validate.OKAY:
			chunk.IndexHash = "" // we don't have to download it
			chunk.IndexSize = 0
			if file.FileExists(paths.ToIndexPath(path)) {
				reportReason("The index file", indexStatus, paths.ToIndexPath(path))
			}
		case validate.FILE_ERROR:
			return false, err // bubble the error up
		case validate.FILE_MISSING:
			fallthrough
		case validate.WRONG_SIZE:
			fallthrough
		case validate.WRONG_MAGIC:
			fallthrough
		case validate.WRONG_HASH:
			reportReason("The index file", indexStatus, paths.ToIndexPath(path))
		default:
			logger.Fatal("should not happen ==> unknown return from IsValidChunk")
		}

		if chunk.BloomHash == "" && chunk.IndexHash == "" {
			man.ChunkMap[rng.String()] = nil
		}

		return true, nil

	} else {
		reportReason("The bloom file", validate.NOT_IN_MANIFEST, path)
		if err := os.Remove(path); err != nil {
			return false, err
		}

		indexPath := paths.ToIndexPath(path)
		if file.FileExists(indexPath) {
			reportReason("The index file", validate.NOT_IN_MANIFEST, indexPath)
			if err := os.Remove(indexPath); err != nil {
				return false, err
			}
		}

		return true, nil
	}
}

var reasons = map[validate.ErrorType]string{
	validate.OKAY:            "okay",
	validate.FILE_ERROR:      "file error",
	validate.FILE_MISSING:    "is file missing",
	validate.WRONG_SIZE:      "has the wrong size",
	validate.WRONG_MAGIC:     "has the wrong magic number",
	validate.WRONG_HASH:      "has the wrong header hash",
	validate.NOT_IN_MANIFEST: "is not in the manifest",
}

func reportReason(prefix string, status validate.ErrorType, path string) {
	if !verbose {
		return
	}

	col := colors.BrightGreen
	if status != validate.OKAY {
		if status == validate.FILE_ERROR || status == validate.NOT_IN_MANIFEST {
			col = colors.BrightRed
		} else if strings.Contains(path, "/blooms/") {
			col = colors.BrightYellow
		} else {
			col = colors.BrightMagenta
		}
	}

	msg := fmt.Sprintf("%s%s %-30.30s%s%s", col, prefix, reasons[status], colors.Off, path)
	logger.Log(logger.Warning, msg)
}
