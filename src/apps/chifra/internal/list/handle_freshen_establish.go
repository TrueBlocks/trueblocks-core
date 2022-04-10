package listPkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/progress"
)

// TODO: BOGUS - This could use some integration tests
// establishIndexChunk accpets cache.FileRange, converts it to file name and finds
// correspoding CID (hash) entry in the manifest. Next, it downloads the bloom filter.
func establishIndexChunk(chain string, indexFilename string) (bool, error) {
	if file.FileExists(indexFilename) {
		return true, nil
	}

	// Open manifest
	localManifest, err := manifest.FromLocalFile(chain)
	if err != nil {
		return false, err
	}

	fileRange, _ := cache.RangeFromFilename(indexFilename)

	// Find bloom filter's CID
	var matchedPin manifest.PinDescriptor
	for _, pin := range localManifest.NewPins {
		if pin.FileName == cache.FilenameFromRange(fileRange, "") {
			matchedPin = pin
			break
		}
	}
	if matchedPin.FileName == "" {
		return false, fmt.Errorf("filename not found in pins: %s", indexFilename)
	}

	logger.Log(logger.Info, "Downloading", colors.Blue, fileRange, colors.Off, "from IPFS.")

	// Start downloading the filter
	pins := []manifest.PinDescriptor{matchedPin}
	chunkPath := cache.NewCachePath(chain, cache.Index_Final)
	progressChannel := make(chan *progress.Progress)

	go func() {
		index.GetChunksFromRemote(chain, pins, &chunkPath, progressChannel)
		close(progressChannel)
	}()

	for event := range progressChannel {
		exists := file.FileExists(indexFilename)
		switch event.Event {
		case progress.AllDone:
			return exists, nil
		case progress.Cancelled:
			return exists, nil
		case progress.Error:
			return exists, fmt.Errorf("error while downloading: %s", event.Message)
		}
	}
	return file.FileExists(indexFilename), nil
}
