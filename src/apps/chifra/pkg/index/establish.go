package index

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"errors"
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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/unchained"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

// EstablishIndexChunk a filename to an index portion, finds the correspoding CID (hash)
// entry in the manifest, and downloads the index chunk to the local drive
func EstablishIndexChunk(chain string, fileRange base.FileRange) (bool, error) {
	exists, fileName := fileRange.RangeToFilename(chain)

	chunkManifest, err := manifest.ReadManifest(chain, base.ZeroAddr, manifest.FromCache)
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
		return exists, fmt.Errorf("filename path missing in chunks: %s", fileRange)
	}

	logger.Info("Bloom filter hit, downloading index portion", (colors.Yellow + fileRange.String() + colors.Off), "from IPFS.")

	// Start downloading the filter
	matchedPin.BloomHash = "" // we want to download only the index chunk
	chunks := []manifest.ChunkRecord{matchedPin}
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
	indexPath := config.GetPathToIndex(chain)

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

func IndexIsInitialized(chain string) error {
	CheckBackLevelIndex(chain)

	path := config.GetPathToIndex(chain) + "blooms/000000000-000000000.bloom"
	if !file.FileExists(path) {
		msg := strings.Replace(IndexNotInitialized, "{0}", "{v0.40.0-beta}", -1)
		msg = strings.Replace(msg, "[{VERSION}]", version.LibraryVersion, -1)
		msg = strings.Replace(msg, "[{PATH}]", path, -1)
		msg = strings.Replace(msg, "{", colors.Green, -1)
		msg = strings.Replace(msg, "}", colors.Off, -1)
		return fmt.Errorf(msg)
	}

	return nil
}

const IndexNotInitialized string = `

	  The Unchained Index does not appear to be initialized. You must run 'chifra init'
	  (and allow it to complete) or 'chifra scrape' before using this command.
	  
	  Path: [{PATH}]

	  [{VERSION}]

	`

func CheckBackLevelIndex(chain string) {
	fileName := config.GetPathToIndex(chain) + "finalized/000000000-000000000.bin"
	if !file.FileExists(fileName) {
		return
	}
	ok, err := HasValidIndexHeader(fileName)
	if ok && err == nil {
		return
	}

	msg := strings.Replace(BackLevelVersion, "{0}", "{v0.40.0-beta}", -1)
	msg = strings.Replace(msg, "[{VERSION}]", version.LibraryVersion, -1)
	msg = strings.Replace(msg, "[{FILE}]", fileName, -1)
	msg = strings.Replace(msg, "{", colors.Green, -1)
	msg = strings.Replace(msg, "}", colors.Off, -1)
	logger.Fatal(msg)
}

const BackLevelVersion string = `

	  An older version of an index file was found at

	    {[{FILE}]}

	  Please carefully follow all migrations up to and including {0}
	  before proceeding.

	  See https://github.com/TrueBlocks/trueblocks-core/blob/develop/MIGRATIONS.md

	  [{VERSION}]

	`

// TODO: There is a header validator in the validate package. Can we use that instead?

func HasValidIndexHeader(fileName string) (bool, error) {
	header, err := ReadChunkHeader(fileName, true)
	if err != nil {
		return false, err
	}

	rng := base.RangeFromFilename(fileName)
	if header.Magic != file.MagicNumber {
		msg := fmt.Sprintf("%s: Magic number expected (0x%x) got (0x%x)", rng, header.Magic, file.MagicNumber)
		return false, errors.New(msg)

	} else if header.Hash.Hex() != unchained.HeaderMagicHash {
		msg := fmt.Sprintf("%s: Header hash expected (%s) got (%s)", rng, header.Hash.Hex(), unchained.HeaderMagicHash)
		return false, errors.New(msg)
	}

	return true, nil
}
