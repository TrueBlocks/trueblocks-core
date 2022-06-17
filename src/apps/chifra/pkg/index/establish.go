package index

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/progress"
)

// EstablishIndexChunk a filename to an index portion, finds the correspoding CID (hash)
// entry in the manifest, and downloads the index chunk to the local drive
func EstablishIndexChunk(chain string, fileRange cache.FileRange) (bool, error) {
	exists, fileName := fileRange.RangeToFilename(chain, cache.Index_Final)

	localManifest, err := manifest.FromLocalFile(chain)
	if err != nil {
		return exists, err
	}

	// Find bloom filter's CID
	// TODO(dawid): This can be a binary search since the pin list is always sorted
	var matchedPin manifest.Chunk
	for _, pin := range localManifest.Chunks {
		if strings.Contains(fileName, pin.FileName) {
			matchedPin = pin
			break
		}
	}
	if matchedPin.FileName == "" {
		return exists, fmt.Errorf("filename not found in pins: %s", fileRange)
	}

	logger.Log(logger.Info, "Downloading", colors.Blue, fileRange, colors.Off, "from IPFS.")

	// Start downloading the filter
	pins := []manifest.Chunk{matchedPin}
	progressChannel := make(chan *progress.Progress)

	go func() {
		chunkPath := cache.NewCachePath(chain, cache.Index_Final)
		GetChunksFromRemote(chain, pins, &chunkPath, progressChannel)
		close(progressChannel)
	}()

	for event := range progressChannel {
		switch event.Event {
		case progress.AllDone:
			return file.FileExists(fileName), nil
		case progress.Cancelled:
			return file.FileExists(fileName), nil
		case progress.Error:
			return file.FileExists(fileName), fmt.Errorf("error while downloading: %s", event.Message)
		}
	}
	return file.FileExists(fileName), nil
}
