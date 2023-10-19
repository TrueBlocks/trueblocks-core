package chunksPkg

import (
	"context"
	"fmt"
	"os"
	"path/filepath"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinning"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

func (opts *ChunksOptions) HandlePin(blockNums []uint64) error {
	chain := opts.Globals.Chain
	firstBlock := mustParseUint(os.Getenv("TB_CHUNKS_PINFIRSTBLOCK"))
	outPath := filepath.Join(config.PathToCache(chain), "tmp", "manifest.json")
	if opts.Rewrite {
		outPath = config.PathToManifest(chain)
	}

	man := manifest.Manifest{
		Version:       version.ManifestVersion,
		Chain:         chain,
		Specification: config.Specification,
		Config:        config.GetScrape(chain),
	}

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {
		report := simpleChunkPinReport{
			Version:       version.ManifestVersion,
			Chain:         chain,
			Specification: config.Specification,
		}

		if len(blockNums) != 0 || opts.Deep {
			pinChunk := func(walker *walk.CacheWalker, path string, first bool) (bool, error) {
				rng, err := base.RangeFromFilenameE(path)
				if err != nil {
					return false, err
				}
				if rng.First < firstBlock {
					logger.Info("Skipping", path)
					return true, nil
				}

				if path != index.ToBloomPath(path) {
					return false, fmt.Errorf("should not happen in pinChunk")
				}

				local, remote, err := pinning.PinOneChunk(chain, index.ToBloomPath(path), index.ToIndexPath(path), config.IpfsRunning(), opts.Remote)
				if err != nil {
					errorChan <- err
					cancel() // keep going...
					return true, nil
				}

				if !matches(&local, &remote) {
					logger.Warn("Local and remote pins do not match")
					logger.Warn(local.BloomHash, "-", local.IndexHash)
					logger.Warn(remote.BloomHash, "-", remote.IndexHash)
					logger.Fatal("IPFS hashes between local and remote do not match")
				} else if opts.Remote && config.IpfsRunning() {
					logger.Info(colors.BrightGreen+"Matches: "+remote.BloomHash.String(), "-", remote.IndexHash, colors.Off)
				}

				if opts.Remote {
					man.Chunks = append(man.Chunks, remote)
				} else {
					man.Chunks = append(man.Chunks, local)
				}
				_ = man.SaveManifest(chain, outPath)

				if opts.Globals.Verbose {
					if opts.Remote {
						fmt.Println("result.Remote:", remote.String())
					} else {
						fmt.Println("result.Local:", local.String())
					}
				}

				sleep := opts.Sleep
				if sleep > 0 {
					ms := time.Duration(sleep*1000) * time.Millisecond
					if !opts.Globals.TestMode {
						logger.Info(fmt.Sprintf("Sleeping for %g seconds", sleep))
					}
					time.Sleep(ms)
				}

				return true, nil
			}

			walker := walk.NewCacheWalker(
				chain,
				opts.Globals.TestMode,
				100, /* maxTests */
				pinChunk,
			)

			if err := walker.WalkBloomFilters(blockNums); err != nil {
				errorChan <- err
				cancel()
				return
			}
		}

		if len(blockNums) == 0 {
			var err error
			tsPath := config.PathToTimestamps(chain)
			if report.TsHash, _, err = pinning.PinOneFile(chain, "timestamps", tsPath, config.IpfsRunning(), opts.Remote); err != nil {
				errorChan <- err
				cancel()
				return
			}

			manPath := config.PathToManifest(chain)
			if opts.Deep {
				manPath = outPath
			}
			if report.ManifestHash, _, err = pinning.PinOneFile(chain, "manifest", manPath, config.IpfsRunning(), opts.Remote); err != nil {
				errorChan <- err
				cancel()
				return
			}
		}

		if opts.Deep {
			logger.Info("The new manifest was written to", colors.BrightGreen, outPath, colors.Off)
		}

		modelChan <- &report
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}

// matches returns true if the Result has both local and remote hashes for both the index and the bloom and they match
func matches(local, remote *types.SimpleChunkRecord) bool {
	hasLocal := local.IndexHash != "" && local.BloomHash != ""
	hasRemote := remote.IndexHash != "" && remote.BloomHash != ""
	if hasLocal && hasRemote {
		return local.IndexHash == remote.IndexHash && local.BloomHash == remote.BloomHash
	}
	return true
}
