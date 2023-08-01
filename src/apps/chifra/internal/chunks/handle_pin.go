package chunksPkg

import (
	"context"
	"fmt"
	"os"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinning"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/unchained"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
)

func (opts *ChunksOptions) HandlePin(blockNums []uint64) error {
	firstBlock := mustParseUint(os.Getenv("TB_CHUNK_PIN_FIRST_BLOCK"))

	chain := opts.Globals.Chain
	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {
		man := simpleChunkPinReport{
			Version: version.ManifestVersion,
			Chain:   chain,
			Schemas: unchained.Schemas,
		}

		var err error
		tsPath := config.GetPathToIndex(chain) + "ts.bin"
		if man.TsHash, err = pinning.PinItem(chain, "timestamps", tsPath, opts.Remote); err != nil {
			errorChan <- err
			cancel()
			return
		}

		manPath := config.GetPathToChainConfig(chain) + "manifest.json"
		if man.ManifestHash, err = pinning.PinItem(chain, "manifest", manPath, opts.Remote); err != nil {
			errorChan <- err
			cancel()
			return
		}

		if opts.Deep {
			pinChunk := func(walker *index.CacheWalker, path string, first bool) (bool, error) {
				rng, err := base.RangeFromFilenameE(path)
				if err != nil {
					return false, err
				}
				if rng.First < firstBlock {
					logger.Info("Skipping", path)
					return true, nil
				}

				if path != cache.ToBloomPath(path) {
					return false, fmt.Errorf("should not happen in pinChunk")
				}

				result, err := pinning.PinChunk(chain, path, opts.Remote)
				if err != nil {
					return false, err
				}

				if pinning.LocalDaemonRunning() {
					man.Pinned = append(man.Pinned, result.Local.BloomHash)
					man.Pinned = append(man.Pinned, result.Local.IndexHash)
				}

				if opts.Remote {
					man.Pinned = append(man.Pinned, result.Remote.BloomHash)
					man.Pinned = append(man.Pinned, result.Remote.IndexHash)
				}

				if !result.Matches {
					logger.Warn("Local and remote pins do not match")
					logger.Warn(colors.Yellow+result.Local.BloomHash.String(), "-", result.Local.IndexHash, colors.Off)
					logger.Warn(colors.Yellow+result.Remote.BloomHash.String(), "-", result.Remote.IndexHash, colors.Off)
					logger.Fatal("Failed")
				} else if opts.Remote && pinning.LocalDaemonRunning() {
					logger.Info(colors.BrightGreen+"Matches: "+result.Remote.BloomHash.String(), "-", result.Remote.IndexHash, colors.Off)
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

			walker := index.NewCacheWalker(
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

		modelChan <- &man
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}
