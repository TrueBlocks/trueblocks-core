package chunksPkg

import (
	"context"
	"fmt"
	"os"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinning"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/unchained"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

func (opts *ChunksOptions) HandlePin(blockNums []uint64) error {
	firstBlock := mustParseUint(os.Getenv("TB_CHUNKS_PINFIRSTBLOCK"))

	chain := opts.Globals.Chain
	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {
		report := simpleChunkPinReport{
			Version:       version.ManifestVersion,
			Chain:         chain,
			Specification: unchained.Specification,
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
			if report.ManifestHash, _, err = pinning.PinOneFile(chain, "manifest", manPath, config.IpfsRunning(), opts.Remote); err != nil {
				errorChan <- err
				cancel()
				return
			}
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

				if !pinning.Matches(&local, &remote) {
					logger.Warn("Local and remote pins do not match")
					logger.Warn(colors.Yellow+local.BloomHash.String(), "-", local.IndexHash, colors.Off)
					logger.Warn(colors.Yellow+remote.BloomHash.String(), "-", remote.IndexHash, colors.Off)
					logger.Fatal("IPFS hashes between local and remote do not match")
				} else if opts.Remote && config.IpfsRunning() {
					logger.Info(colors.BrightGreen+"Matches: "+remote.BloomHash.String(), "-", remote.IndexHash, colors.Off)
				}

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

		modelChan <- &report
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}
