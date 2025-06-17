package index

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/progress"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ranges"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

// DownloadOneChunk a filename to an index portion, finds the corresponding CID (hash)
// entry in the manifest, and downloads the index chunk to the local drive
func DownloadOneChunk(chain string, man *manifest.Manifest, fileRange ranges.FileRange) error {
	// Find bloom filter's CID
	matchedPin := man.ChunkMap[fileRange.String()]
	if matchedPin == nil || matchedPin.Range == "" {
		return fmt.Errorf("filename path missing in chunks: %s", fileRange)
	}

	logger.Info("Bloom filter hit, downloading index portion", (colors.Yellow + fileRange.String() + colors.Off), "from IPFS.")

	// Start downloading the filter
	matchedPin.BloomHash = "" // we want to download only the index chunk
	chunks := []types.ChunkRecord{*matchedPin}
	progressChannel := make(chan *progress.ProgressMsg)

	go func() {
		DownloadChunks(chain, chunks, walk.Index_Final, 4 /* poolSize */, progressChannel)
		close(progressChannel)
	}()

	for event := range progressChannel {
		switch event.Event {
		case progress.AllDone:
			return nil
		case progress.Cancelled:
			return nil
		case progress.Error:
			return fmt.Errorf("error while downloading: %s", event.Message)
		}
	}
	return nil
}
