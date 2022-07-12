package scrapePkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"errors"
	"fmt"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinning"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/unchained"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
)

// TODO: BOGUS - MANIFEST WRITING THE MANIFEST
func (opts *ScrapeOptions) Y_4_postScrape(progressThen *rpcClient.MetaData) (bool, error) {

	// If we're not pinning, do nothing
	if !opts.Pin {
		return true, nil
	}

	// If there's been no progress, do nothing
	progressNow, err := rpcClient.GetMetaData(opts.Globals.Chain, opts.Globals.TestMode)
	if err != nil {
		return false, err
	}
	defer func() {
		if progressNow != nil {
			*progressThen = *progressNow
		}
	}()

	if progressNow.Finalized <= progressThen.Finalized {
		return true, nil
	}

	newPinsFn := config.GetPathToCache(opts.Globals.Chain) + "tmp/chunks_created.txt"
	if !file.FileExists(newPinsFn) {
		return true, errors.New("chunks_created file not found, but there's been progress")
	}

	// TODO: BOGUS - TESTING SCRAPING
	rel := strings.Replace(newPinsFn, config.GetPathToCache(opts.Globals.Chain), "$CACHE", -1)
	fmt.Println()
	fmt.Println("----------------------------------------------------------------------------------------------")
	logger.Log(logger.Info, "PostScrape", rel, file.FileSize(newPinsFn))

	lines := file.AsciiFileToLines(newPinsFn)
	if len(lines) < 1 {
		return true, errors.New("chunks_created file found, but it was empty")
	}

	var pathToIndex string
	for _, line := range lines {

		parts := strings.Split(line, "\t")

		record := manifest.ChunkRecord{}
		if len(parts) < 1 {
			return true, errors.New("Invalid record in chunks_created.txt file: " + line)
		}
		if len(parts) > 0 {
			record.Range = parts[0]
		}
		if len(parts) > 1 {
			record.BloomHash = types.IpfsHash(parts[1])
		}
		if len(parts) > 2 {
			record.IndexHash = types.IpfsHash(parts[2])
		}

		unchainedFolder := config.GetPathToIndex(opts.Globals.Chain)
		pathToIndex = unchainedFolder + "finalized/" + record.Range + ".bin"
		bloomPath := unchainedFolder + "blooms/" + record.Range + ".bloom"

		settings := config.GetBlockScrapeSettings(opts.Globals.Chain)
		key, secret := settings.Pinata_api_key, settings.Pinata_secret_api_key
		pina := pinning.Service{
			Local:  true,
			Apikey: key,
			Secret: secret,
		}

		bloomHash, err := pina.PinFile(bloomPath)
		if err != nil {
			return true, err
		}
		record.BloomHash = types.IpfsHash(bloomHash)

		indexHash, err := pina.PinFile(pathToIndex)
		if err != nil {
			return true, err
		}
		record.IndexHash = types.IpfsHash(indexHash)

		logger.Log(logger.Info, "Pinned:", record.Range, bloomHash, indexHash)
		err = opts.Z_7_updateManifest(record)
		if err != nil {
			return true, err
		}

		// ipfsAvail := pinning.LocalDaemonRunning()
	}

	// TODO: BOGUS - TESTING SCRAPING
	index.TestWrite(opts.Globals.Chain, pathToIndex, &opts.Globals)
	os.Remove(newPinsFn)
	return true, nil
}

func unique(chunks []manifest.ChunkRecord) []manifest.ChunkRecord {
	inResult := make(map[string]bool)
	var result []manifest.ChunkRecord
	for _, chunk := range chunks {
		if _, ok := inResult[chunk.Range]; !ok {
			inResult[chunk.Range] = true
			result = append(result, chunk)
		}
	}
	return result
}

func (opts *ScrapeOptions) Z_7_updateManifest(chunk manifest.ChunkRecord) error {
	man, err := manifest.ReadManifest(opts.Globals.Chain, manifest.FromCache)
	if err != nil {
		if strings.Contains(err.Error(), "Could not find") {
			man = &manifest.Manifest{
				Version:   version.ManifestVersion,
				Chain:     opts.Globals.Chain,
				Schemas:   unchained.Schemas,
				Databases: unchained.Databases,
				Chunks:    []manifest.ChunkRecord{},
			}

		} else {
			return err
		}
	}
	man.Chunks = unique(append(man.Chunks, chunk))

	// TODO: BOGUS DOES THIS DESTROY THE FILE ON DISC BEFORE WRITING TO IT? I THINK IT DOES.
	fileName := config.GetPathToChainConfig(opts.Globals.Chain) + "manifest.json"
	w, err := os.Create(fileName)
	if err != nil {
		return fmt.Errorf("creating file: %s", err)
	}
	defer w.Close()
	err = file.Lock(w)
	if err != nil {
		return fmt.Errorf("locking file: %s", err)
	}

	tmp := opts.Globals
	tmp.Format = "json"
	tmp.Writer = w
	tmp.NoHeader = false
	tmp.ApiMode = false
	logger.Log(logger.Info, "Updated manifest with", len(man.Chunks), "chunks")
	return tmp.RenderObject(man, true)
}

// TODO: BOGUS - DO I REALLY WANT TO TURN OFF GZIP?
// TODO: BOGUS - LOG_INFO(bBlue, "  Pinned index for blocks ", start, " to ", end, " at ", item.indexHash, cOff);
