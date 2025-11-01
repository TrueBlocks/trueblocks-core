package chunksPkg

import (
	"fmt"
	"os"
	"path/filepath"
	"sort"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/pinning"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/ranges"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/usage"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/walk"
)

func (opts *ChunksOptions) HandlePin(rCtx *output.RenderCtx, blockNums []base.Blknum) error {
	chain := opts.Globals.Chain
	if opts.Globals.TestMode && !opts.DryRun {
		logger.Warn("Pinning option not tested.")
		return nil
	}

	if !opts.Globals.IsApiMode() && usage.QueryUser(pinWarning, "Check skipped") {
		if err := opts.doCheck(rCtx, blockNums); err != nil {
			rCtx.Cancel()
			return err
		}
	}

	firstBlock := base.MustParseBlknum(os.Getenv("TB_CHUNKS_PINFIRSTBLOCK"))
	lastBlock := base.MustParseBlknum(os.Getenv("TB_CHUNKS_PINLASTBLOCK"))
	if lastBlock == 0 {
		lastBlock = base.NOPOSN
	}

	outPath := filepath.Join(config.PathToCache(chain), "tmp", "manifest.json")
	if opts.Rewrite {
		outPath = config.PathToManifestFile(chain)
	}

	man, err := manifest.LoadManifest(chain, opts.PublisherAddr, manifest.LocalCache)
	if err != nil {
		rCtx.Cancel()
		return err
	}

	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		hash := base.BytesToHash(config.HeaderHash(config.ExpectedVersion()))
		report := types.ChunkPin{
			Version:  config.VersionTags[hash.Hex()],
			Chain:    chain,
			SpecHash: base.IpfsHash(manifest.Specification()),
		}

		fileList := make([]string, 0, len(man.Chunks))
		listFiles := func(walker *walk.CacheWalker, path string, first bool) (bool, error) {
			_ = walker
			_ = first
			rng, err := ranges.RangeFromFilenameE(path)
			if err != nil {
				return false, err
			}
			if rng.Last < firstBlock || rng.First > lastBlock {
				logger.Progress(true, "Skipping", path)
				return true, nil
			}
			if path != index.ToBloomPath(path) {
				return false, fmt.Errorf("should not happen in pinChunk")
			}
			if opts.Deep || len(blockNums) > 0 || man.ChunkMap[rng.String()] == nil {
				fileList = append(fileList, path)
			}
			return true, nil
		}

		walker := walk.NewCacheWalker(
			chain,
			opts.Globals.TestMode,
			100, /* maxTests */
			listFiles,
		)

		// Only walk and pin chunks/blooms if --metadata flag is not set
		if !opts.Metadata {
			if err := walker.WalkBloomFilters(blockNums); err != nil {
				errorChan <- err
				rCtx.Cancel()
				return
			}
		}

		sort.Slice(fileList, func(i, j int) bool {
			rng1, _ := ranges.RangeFromFilenameE(fileList[i])
			rng2, _ := ranges.RangeFromFilenameE(fileList[j])
			return rng1.First < rng2.First
		})

		failCnt := 1.0
		chunksProcessed := 0
		for i := 0; i < len(fileList); i++ {
			sleep := opts.Sleep

			path := fileList[i]
			if opts.Globals.Verbose {
				logger.Info("pinning path:", path)
			}

			if opts.DryRun {
				rng := ranges.RangeFromFilename(path)
				logger.Info("DRY RUN: Would pin chunk:", rng.String())
				if opts.Rewrite && chunksProcessed == 0 {
					logger.Info("DRY RUN: Manifest would have been saved after each chunk pin to:", outPath)
				}
				chunksProcessed++
				continue
			}

			local, remote, err := pinning.PinOneChunk(chain, path, opts.Remote, fmt.Sprintf("%d-%d", chunksProcessed+1, len(fileList)))
			if err != nil {
				errorChan <- err
				logger.Error("Pin failed:", path, err)
				failCnt *= 2.
				sleep = failCnt
				i-- // try again after sleeping for a bit
				logger.Info(colors.Yellow, "Sleeping for", sleep, "seconds then trying again.", colors.Off)

			} else {
				chunksProcessed++
				progressInfo := fmt.Sprintf("%d-%d", chunksProcessed, len(fileList))
				blMatches, idxMatches := matches(&local, &remote)
				opts.matchReport(blMatches, local.BloomHash, remote.BloomHash, progressInfo)
				opts.matchReport(idxMatches, local.IndexHash, remote.IndexHash, progressInfo)

				if opts.Remote {
					man.Chunks = append(man.Chunks, remote)
				} else {
					man.Chunks = append(man.Chunks, local)
				}
				if !opts.DryRun {
					_ = man.SaveManifest(chain, outPath)
				}

				if opts.Globals.Verbose {
					if opts.Remote {
						fmt.Println("result.Remote:", remote.String())
					} else {
						fmt.Println("result.Local:", local.String())
					}
				}
			}

			if sleep > 0 {
				ms := time.Duration(sleep*1000) * time.Millisecond
				if !opts.Globals.TestMode {
					logger.Info("Sleeping for", sleep, "seconds")
				}
				time.Sleep(ms)
			}
		}

		if (len(blockNums) == 0 && firstBlock == 0 && lastBlock == base.NOPOSN) || opts.Metadata {
			tsPath := config.PathToTimestamps(chain)
			if opts.DryRun {
				logger.Info("DRY RUN: Would pin timestamps file:", tsPath)
			} else {
				if localHash, remoteHash, err := pinning.PinOneFile(chain, "timestamps", tsPath, opts.Remote); err != nil {
					errorChan <- err
					logger.Error("Pin failed:", tsPath, err)
				} else {
					opts.matchReport(localHash == remoteHash, localHash, remoteHash, "timestamps")
					report.TimestampHash = localHash
				}
			}

			manPath := config.PathToManifestFile(chain)
			if opts.Deep {
				manPath = outPath
			}
			if opts.DryRun {
				logger.Info("DRY RUN: Would pin manifest file:", manPath)
			} else {
				if localHash, remoteHash, err := pinning.PinOneFile(chain, "manifest", manPath, opts.Remote); err != nil {
					errorChan <- err
					logger.Error("Pin failed:", manPath, err)
				} else {
					opts.matchReport(localHash == remoteHash, localHash, remoteHash, "manifest")
					report.ManifestHash = localHash
				}
			}
		}

		logger.Info("The new manifest was written to", colors.BrightGreen+outPath+colors.Off, len(man.Chunks), "chunks")

		modelChan <- &report
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOpts())
}

// matches returns true if the Result has both local and remote hashes for both the index and the bloom and they match
func matches(local, remote *types.ChunkRecord) (bool, bool) {
	return local.BloomHash == remote.BloomHash, local.IndexHash == remote.IndexHash
}

func (opts *ChunksOptions) matchReport(matches bool, localHash, remoteHash base.IpfsHash, progressInfo string) {
	_ = remoteHash // linter
	if !opts.Remote || !config.IpfsRunning() {
		return // if we're not pinning in two places, don't report on matches
	}

	if matches {
		logger.Info(colors.BrightGreen+"["+progressInfo+"] Matches "+localHash.String(), " ", localHash, colors.Off)
	} else {
		logger.Warn("["+progressInfo+"] Pins mismatch", localHash.String(), " ", localHash)
	}
}

func (opts *ChunksOptions) doCheck(rCtx *output.RenderCtx, blockNums []base.Blknum) error {
	if err, ok := opts.check(rCtx, blockNums, false /* silent */); err != nil {
		return err
	} else if !ok {
		return fmt.Errorf("checks failed")
	}
	return nil
}

var pinWarning = `Do you want to run --check first (Yn)? `
