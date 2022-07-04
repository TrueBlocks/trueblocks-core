package scrapePkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"errors"
	"fmt"
	"os"
	"strconv"
	"strings"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinata"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/scraper"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/unchained"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
)

var IndexScraper scraper.Scraper

func (opts *ScrapeOptions) RunIndexScraper(wg *sync.WaitGroup) {
	defer wg.Done()

	progress, _ := rpcClient.GetMetaData(opts.Globals.Chain, opts.Globals.TestMode)

	var s *scraper.Scraper = &IndexScraper
	s.ChangeState(true)

	for {
		if !s.Running {
			s.Pause()

		} else {
			err := opts.Globals.PassItOn("blockScrape", opts.Globals.Chain, opts.ToCmdLine(), opts.GetEnvStr())
			if err != nil {
				fmt.Println("Call to blockScrape errored:", err)
			} else {
				err = opts.publishManifest(progress)
				if err != nil {
					fmt.Println("Call to publishManifest errored:", err)
				}
			}

			e := os.Getenv("TEST_END_SCRAPE")
			ee, _ := strconv.ParseUint(e, 10, 32)
			if ee != 0 && progress.Finalized > ee {
				return
			}

			if s.Running {
				// We sleep under two conditions
				//   1) the user has told us an explicit amount of time to Sleep
				//   2) we're close enough to the head that we should sleep because there
				//      are no new blocks (UnripeDist defaults to 28 blocks)
				//
				// If we're closeEnough and the user specified a sleep value less than
				// 14 seconds, there's not reason to not sleep
				// TODO: Multi-chain specific
				var distanceFromHead uint64 = 13
				meta, err := rpcClient.GetMetaData(opts.Globals.Chain, false)
				if err != nil {
					fmt.Println("Error from node:", err)
				} else {
					distanceFromHead = meta.Latest - meta.Staging
				}
				closeEnough := distanceFromHead <= (2 * opts.UnripeDist)
				// TODO: Multi-chain specific
				if closeEnough && opts.Sleep < 13 {
					// TODO: Multi-chain specific
					opts.Sleep = 13
				}
				// TODO: Multi-chain specific
				isDefault := opts.Sleep == 14 || opts.Sleep == 13
				if !isDefault || closeEnough {
					if closeEnough {
						fmt.Println("Close enough to head. Sleeping for", opts.Sleep, "seconds -", distanceFromHead, "away from head.")
					} else {
						fmt.Println("Sleeping for", opts.Sleep, "seconds -", distanceFromHead, "away from head.")
					}
					s.Pause()
				}
			}
		}
	}
}

// TODO: BOGUS - MANIFEST WRITING THE MANIFEST
func (opts *ScrapeOptions) publishManifest(progressThen *rpcClient.MetaData) error {
	// If we're not pinning, do nothing
	if !opts.Pin {
		return nil
	}

	// If there's been no progress, do nothing
	progressNow, _ := rpcClient.GetMetaData(opts.Globals.Chain, opts.Globals.TestMode)
	if progressNow.Finalized <= progressThen.Finalized {
		return nil
	}

	newPinsFn := config.GetPathToCache(opts.Globals.Chain) + "tmp/pins_created.txt"
	if !file.FileExists(newPinsFn) {
		return errors.New("pins_created file not found, but there's been progress")
	}

	lines := file.AsciiFileToLines(newPinsFn)
	if len(lines) < 1 {
		return errors.New("pins_created file found, but it was empty")
	}

	for _, line := range lines {

		parts := strings.Split(line, "\t")

		record := manifest.ChunkRecord{}
		if len(parts) < 1 {
			return errors.New("Invalid record in pins_created.txt file: " + line)
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
		// fmt.Println(colors.BrightGreen, record, colors.Off)

		unchainedFolder := config.GetPathToIndex(opts.Globals.Chain)
		pathToIndex := unchainedFolder + "finalized/" + record.Range + ".bin"
		bloomPath := unchainedFolder + "blooms/" + record.Range + ".bloom"

		pina := pinata.Pinata{
			Apikey: config.ReadBlockScrape(opts.Globals.Chain).Settings.Pinata_api_key,
			Secret: config.ReadBlockScrape(opts.Globals.Chain).Settings.Pinata_secret_api_key,
		}

		bloomHash, err := pina.PinFile(bloomPath)
		if err != nil {
			return err
		}
		record.BloomHash = types.IpfsHash(bloomHash)
		// if bloomHash != record.BloomHash.String() {
		// 	msg := fmt.Sprintf("Hash mismatch between GoLang and C++ %s %s", bloomHash, record.BloomHash.String())
		// 	// return errors.New(msg)
		// 	fmt.Println(msg)
		// }

		indexHash, err := pina.PinFile(pathToIndex)
		if err != nil {
			return err
		}
		record.IndexHash = types.IpfsHash(indexHash)
		// if indexHash != record.IndexHash.String() {
		// 	msg := fmt.Sprintf("Hash mismatch between GoLang and C++ %s %s", indexHash, record.IndexHash.String())
		// 	// return errors.New(msg)
		// 	fmt.Println(msg)
		// }

		fmt.Println("In GoLang --> ", record.Range, bloomHash, indexHash)
		err = opts.UpdateManifest(record)
		if err != nil {
			return err
		}
	}
	os.Remove(newPinsFn)
	*progressThen = *progressNow
	// ipfsAvail := false
	// sh := shell.NewShell("localhost:5001")
	// if opts.Pin {
	// 	_, err := sh.Add(strings.NewReader("hello world!"))
	// 	if err != nil {
	// 		fmt.Println("IPFS daemon not found. Pinning to Pinata only.")
	// 		// os.Exit(1)
	// 	} else {
	// 		fmt.Println("IPFS daemon not found. Pinning locally and to Pinata.")
	// 		ipfsAvail = true
	// 	}
	// }
	return nil
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

func (opts *ScrapeOptions) UpdateManifest(chunk manifest.ChunkRecord) error {
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

	return tmp.RenderObject(man, true)
}

// TODO: BOGUS - DO I REALLY WANT TO TURN OFF GZIP?
// TODO: BOGUS - LOG_INFO(bBlue, "  Pinned index for blocks ", start, " to ", end, " at ", item.indexHash, cOff);
