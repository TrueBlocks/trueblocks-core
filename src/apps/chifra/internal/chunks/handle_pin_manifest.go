package chunksPkg

import (
	"context"
	"fmt"
	"strings"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinning"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/unchained"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
)

func (opts *ChunksOptions) HandlePinManifest(blockNums []uint64) error {
	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawManifest], errorChan chan error) {
		man := types.SimpleManifest{
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
			if path != cache.ToBloomPath(path) {
				return false, fmt.Errorf("should not happen in pinChunk")
			}

			result, err := pinning.PinChunk(opts.Globals.Chain, path, opts.Remote)
			if err != nil {
				return false, err
			}

			if pinning.LocalDaemonRunning() {
				man.Chunks = append(man.Chunks, result.Local)
				logger.Progress(true, "Pinning:", result.Local, spaces)
			} else {
				man.Chunks = append(man.Chunks, result.Remote)
				logger.Progress(true, "Pinning:", result.Remote, spaces)
			}

			if opts.Globals.Verbose {
				logger.Progress(true, "Pinning:", path)
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

var spaces = strings.Repeat(" ", 20)
