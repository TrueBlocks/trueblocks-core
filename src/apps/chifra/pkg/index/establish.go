package index

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"fmt"
	"os"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/progress"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

// EstablishIndexChunk a filename to an index portion, finds the correspoding CID (hash)
// entry in the manifest, and downloads the index chunk to the local drive
func EstablishIndexChunk(chain string, publisher base.Address, fileRange base.FileRange) (bool, error) {
	exists, fileName := fileRange.RangeToFilename(chain)

	chunkManifest, err := manifest.ReadManifest(chain, publisher, manifest.FromCache)
	if err != nil {
		return exists, err
	}

	// Find bloom filter's CID
	var matchedPin types.SimpleChunkRecord
	for _, pin := range chunkManifest.Chunks {
		if strings.Contains(fileName, pin.Range) {
			matchedPin = pin
			break
		}
	}
	if matchedPin.Range == "" {
		return exists, fmt.Errorf("filename path missing in chunks: %s", fileRange)
	}

	logger.Info("Bloom filter hit, downloading index portion", (colors.Yellow + fileRange.String() + colors.Off), "from IPFS.")

	// Start downloading the filter
	matchedPin.BloomHash = "" // we want to download only the index chunk
	chunks := []types.SimpleChunkRecord{matchedPin}
	progressChannel := make(chan *progress.ProgressMsg)

	go func() {
		DownloadChunks(chain, chunks, walk.Index_Final, 4 /* poolSize */, progressChannel)
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

// CleanEphemeralIndexFolders removes files in ripe and unripe
func CleanEphemeralIndexFolders(chain string) error {
	return CleanTempIndexFolders(chain, []string{"ripe", "unripe"})
}

// CleanTempIndexFolders removes any files that may be partial or incomplete
func CleanTempIndexFolders(chain string, subFolders []string) error {
	indexPath := config.PathToIndex(chain)

	for _, f := range subFolders {
		folder := filepath.Join(indexPath, f)
		// We want to remove whatever is there...
		err := os.RemoveAll(folder)
		if err != nil {
			return err
		}
		// ...but put it back
		_ = file.EstablishFolder(folder)
	}

	return nil
}

func IsIndexInitialized(chain string) error {
	fileName := config.PathToIndex(chain) + "finalized/000000000-000000000.bin"
	if !file.FileExists(fileName) {
		const indexNotInitialized string = `

	  The Unchained Index does not appear to be initialized. You must run 'chifra init'
	  (and allow it to complete) or 'chifra scrape' before using this command.
	  
	  Path: [{PATH}]

	  [{VERSION}]

	`
		msg := strings.Replace(indexNotInitialized, "{0}", "{v0.40.0-beta}", -1)
		msg = strings.Replace(msg, "[{VERSION}]", version.LibraryVersion, -1)
		msg = strings.Replace(msg, "[{PATH}]", fileName, -1)
		msg = strings.Replace(msg, "{", colors.Green, -1)
		msg = strings.Replace(msg, "}", colors.Off, -1)
		return fmt.Errorf(msg)
	}

	if ff, err := os.OpenFile(fileName, os.O_RDONLY, 0); err != nil {
		return err

	} else {
		defer ff.Close()
		if _, err = X_ReadIndexHeader(ff, config.GetUnchained().HeaderMagic, false /* unused */); err != nil {
			msg := fmt.Sprintf("%s: %s", err, fileName)
			logger.Fatal(msg)
		}

		return nil
	}
}
