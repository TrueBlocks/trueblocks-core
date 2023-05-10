package chunksPkg

import (
	"context"
	"fmt"
	"io/fs"
	"os"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/user"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ChunksOptions) HandleTruncate(blockNums []uint64) error {
	chain := opts.Globals.Chain

	if opts.Globals.TestMode {
		logger.Warn("Truncate option not tested.")
		return nil
	}

	if !user.QueryUser(strings.Replace(warning, "{0}", fmt.Sprintf("%d", opts.Truncate), -1), "Not truncating") {
		return nil
	}

	indexPath := config.GetPathToIndex(opts.Globals.Chain)
	index.CleanTemporaryFolders(indexPath, true)

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {

		// First, we will remove the chunks and update the manifest. We do this separately for
		// each chunk, so that if we get interrupted, we have a relatively sane state (although,
		// we will have to manually repair the index with chifra init --all if this fails. Keep track
		// of the last chunks remaining.
		latestChunk := uint64(0)
		nChunksRemoved := 0
		truncateIndex := func(walker *index.CacheWalker, path string, first bool) (bool, error) {
			if path != cache.ToBloomPath(path) {
				logger.Fatal("should not happen ==> we're spinning through the bloom filters")
			}

			if strings.HasSuffix(path, ".gz") {
				os.Remove(path)
				return true, nil
			}

			rng, err := base.RangeFromFilenameE(path)
			if err != nil {
				return false, err
			}

			testRange := base.FileRange{First: opts.Truncate, Last: utils.NOPOS}
			if rng.Intersects(testRange) {
				// TODO: We should make backups of these files and only if the manifest and both files
				// are removed and the monitors are cleared should we proceed. Otherwise, replace these
				// backups and the manifest backup.
				if err = manifest.RemoveChunk(chain, path); err != nil {
					return false, err
				}
				nChunksRemoved++
			} else {
				latestChunk = utils.Max(latestChunk, rng.Last)
			}

			return true, nil
		}

		walker := index.NewCacheWalker(
			opts.Globals.Chain,
			opts.Globals.TestMode,
			100, /* maxTests */
			truncateIndex,
		)
		if err := walker.WalkBloomFilters(blockNums); err != nil {
			errorChan <- err
			cancel()
		} else {
			// Now that we know the max block in the index, we must adjust the monitors so they
			// don't think they have already seen chunks we just removed.
			nMonitorsTruncated := 0
			truncateMonitor := func(path string, info fs.FileInfo, err error) error {
				if err != nil {
					return err
				}
				if !info.IsDir() {
					addr, _ := monitor.PathToAddress(path)
					if len(addr) > 0 {
						mon := monitor.NewMonitor(chain, addr, false /* create */)
						if err = mon.TruncateTo(uint32(latestChunk)); err != nil {
							return err
						}
						nMonitorsTruncated++
					}
				}
				return nil
			}
			filepath.Walk(config.GetPathToCache(chain)+"monitors", truncateMonitor)

			msg1 := fmt.Sprintf("Truncated index to block %d (the latest full chunk).", latestChunk)
			msg2 := fmt.Sprintf("%d chunks removed, %d monitors truncated, the manifest was updated.", nChunksRemoved, nMonitorsTruncated)
			if opts.Globals.Format == "json" {
				s := types.SimpleMessage{
					Msg: msg1 + " " + msg2,
				}
				modelChan <- &s
			} else {
				logger.Info(msg1)
				logger.Info(msg2)
			}
		}
	}

	opts.Globals.NoHeader = true
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}

var warning = `Are sure you want to remove index chunks after and including block {0} (Yy)? `
