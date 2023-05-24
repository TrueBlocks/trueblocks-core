package chunksPkg

import (
	"context"
	"fmt"
	"os"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinning"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/unchained"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
)

func (opts *ChunksOptions) HandlePinManifest(blockNums []uint64) error {
	firstBlock := mustParseUint(os.Getenv("TB_CHUNK_PIN_FIRST_BLOCK"))

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {
		man := simpleManifest{
			Version: version.ManifestVersion,
			Chain:   opts.Globals.Chain,
			Schemas: unchained.Schemas,
		}

		if err := pinning.PinTimestamps(opts.Globals.Chain, opts.Remote); err != nil {
			errorChan <- err
			cancel()
			return
		}

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

			result, err := pinning.PinChunk(opts.Globals.Chain, path, opts.Remote)
			if err != nil {
				return false, err
			}

			if pinning.LocalDaemonRunning() {
				man.Chunks = append(man.Chunks, result.Local)
				// logger.Progress(true, colors.Green+"Pinned local: ", result.Local, spaces, colors.Off)
			}

			if opts.Remote {
				man.Chunks = append(man.Chunks, result.Remote)
				// logger.Progress(true, colors.BrightBlue+"Pinned remote:", result.Remote, spaces, colors.Off)
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
			opts.Globals.Chain,
			opts.Globals.TestMode,
			100, /* maxTests */
			pinChunk,
		)

		if err := walker.WalkBloomFilters(blockNums); err != nil {
			errorChan <- err
			cancel()
			return
		}

		modelChan <- &man
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}
