// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package initPkg

import (
	"errors"
	"fmt"
	"os"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/paths"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

// prepareDownloadList returns a list of chunks (or partial chunks) that need to be updated. Upon return, if a chunk is in this list,
// its bloomHash, its indexHash, or both contains the IPFS hash that needst to be download. Any chunks (or portions thereof) that are
// not in the list are valid and remain on disc. That means that the chunk is the manifest, has the right file size, has a valid header,
// and is of the rigth version.
func (opts *InitOptions) prepareDownloadList(chain string, man *manifest.Manifest, chunkType paths.CacheType, blockNums []uint64) ([]manifest.ChunkRecord, int) {
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

	for _, chunk := range man.ChunkMap {
		if chunk != nil {
			if !opts.All {
				chunk.IndexHash = ""
				chunk.IndexSize = 0
			}
			// TODO: BOGUS - REMOVE THIS
			file.AppendToAsciiFile("./one/notes", fmt.Sprintf("1--The chunk needs to be downloaded %v\n", chunk))
			fmt.Println(colors.BrightBlack, "1--The chunk needs to be downloaded", colors.Off, chunk)
			chunksNeeded = append(chunksNeeded, *chunk)
		}
	}
	return chunksNeeded, len(chunksNeeded)
}

// cleanIndex checks that the file is in the manifest, has a valid header, is of the correct version, and is of the right size. If any of those
// are not true, we remove that file and add it to the download list.
//
// Important note: we only walk the bloom filter folder because the bloom filters are required. If the user has specified `--all`, we insist that
// the corresponding index portion is also present and valid. If the user has not specified `--all`, then we check the index portion, only if it
// exists, and then only for the correct header and file size. If a bloom filter is present on disc, but not in the manifest, then we delete
// both the bloom filte and the corresponding index portion if it exists.
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
		logger.Fatal("should not happen ==> we only spin across the bloom filters")
	}

	indexPath := paths.ToIndexPath(path)

	rng := paths.RangeFromFilename(path)
	chunk := man.ChunkMap[rng.String()]
	if chunk != nil {
		if !opts.All {
			// If the user did not use --all, we won't be downloading these hashes, so clear them out
			chunk.IndexHash = ""
			chunk.IndexSize = 0
		}

		// The file exists, check for validity
		valid, err := chunk.CheckValidHeader(path, opts.All)
		if !valid {
			p := path
			if errors.Is(err, validate.ErrInvalidMagicNumber) || errors.Is(err, validate.ErrInvalidHeaderHash) {
				p = indexPath
			}
			fmt.Println(colors.BrightMagenta, "The file is in the manifest, but", err, colors.Off, p)
			return true, nil
		}

		sizeOk, err := chunk.CheckExpectedSize(path, opts.All)
		if !sizeOk {
			p := path
			if errors.Is(err, manifest.ErrIndexMissing) || errors.Is(err, manifest.ErrIndexWrongSize) {
				p = indexPath
			}
			fmt.Println(colors.BrightMagenta, "The file is in the manifest, but", err, colors.Off, p)
			return true, nil
		}

		if opts.All && !file.FileExists(indexPath) {
			fmt.Println(colors.BrightYellow, "The bloom file is okay, but the index is not.", colors.Off, indexPath)
		} else {
			fmt.Println(colors.BrightYellow, "The file is okay. Not downloading.", colors.Off, path)
			man.ChunkMap[rng.String()] = nil
		}
		return true, nil

	} else {
		// The file is on disc, but shouldn't be, remove it
		fmt.Println(colors.BrightRed, "Removing", colors.Off, path)
		if err := os.Remove(path); err != nil {
			return false, err
		}
		indexPath := paths.ToIndexPath(path)
		if file.FileExists(indexPath) {
			fmt.Println(colors.BrightRed, "Removing", colors.Off, indexPath)
			if err := os.Remove(indexPath); err != nil {
				return false, err
			}
		}

		return true, nil
	}
}
