package index

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"fmt"
	"os"
	"path"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/paths"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/progress"
)

// EstablishIndexChunk a filename to an index portion, finds the correspoding CID (hash)
// entry in the manifest, and downloads the index chunk to the local drive
func EstablishIndexChunk(chain string, fileRange paths.FileRange) (bool, error) {
	exists, fileName := fileRange.RangeToFilename(chain, paths.Index_Final)

	chunkManifest, err := manifest.ReadManifest(chain, manifest.FromCache)
	if err != nil {
		return exists, err
	}

	// Find bloom filter's CID
	var matchedPin manifest.ChunkRecord
	for _, pin := range chunkManifest.Chunks {
		if strings.Contains(fileName, pin.Range) {
			matchedPin = pin
			break
		}
	}
	if matchedPin.Range == "" {
		return exists, fmt.Errorf("filename not found in chunks: %s", fileRange)
	}

	logger.Log(logger.Info, "Downloading", colors.Blue, fileRange, colors.Off, "from IPFS.")

	// Start downloading the filter
	chunks := []manifest.ChunkRecord{matchedPin}
	progressChannel := make(chan *progress.Progress)

	go func() {
		DownloadChunks(chain, chunks, paths.Index_Final, 4 /* poolSize */, progressChannel)
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

// CleanTemporaryFolders removes any files that may be partial or incomplete
func CleanTemporaryFolders(indexPath string, incStaging bool) error {
	folders := []string{"ripe", "unripe", "maps", "staging"}
	if !incStaging {
		folders = folders[:len(folders)-2]
	}

	for _, f := range folders {
		folder := path.Join(indexPath, f)
		err := os.RemoveAll(folder)
		if err != nil {
			return err
		}
	}

	return nil
}
