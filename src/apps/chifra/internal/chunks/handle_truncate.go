package chunksPkg

import (
	"context"
	"fmt"
	"io/fs"
	"os"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/usage"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

// TODO: We need to make sure, when we truncate, that we truncate the corresponding maps files as well.

func (opts *ChunksOptions) HandleTruncate(blockNums []base.Blknum) error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	if opts.Globals.TestMode {
		logger.Warn("Truncate option not tested.")
		return nil
	}

	if !opts.Globals.IsApiMode() &&
		!usage.QueryUser(strings.Replace(truncateWarning, "{0}", fmt.Sprintf("%d", opts.Truncate), -1), "Not truncated") {
		return nil
	}

	_ = file.CleanFolder(chain, config.PathToIndex(chain), []string{"ripe", "unripe", "maps", "staging"})

	bar := logger.NewBar(logger.BarOptions{
		Enabled: !testMode, // do not uncomment !logger.IsTerminal(),
		Total:   128,
		Type:    logger.Expanding,
	})

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.Message], errorChan chan error) {

		// First, we will remove the chunks and update the manifest. We do this separately for
		// each chunk, so that if we get interrupted, we have a relatively sane state (although,
		// we will have to manually repair the index with chifra init --all if this fails. Keep track
		// of the last chunks remaining.
		latestChunk := base.Blknum(0)
		nChunksRemoved := 0
		truncateIndex := func(walker *walk.CacheWalker, path string, first bool) (bool, error) {
			if path != index.ToBloomPath(path) {
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

			testRange := base.FileRange{First: opts.Truncate, Last: base.NOPOSN}
			if rng.Intersects(testRange) {
				if err = manifest.RemoveChunk(chain, opts.PublisherAddr, index.ToBloomPath(path), index.ToIndexPath(path)); err != nil {
					return false, err
				}
				bar.Prefix = fmt.Sprintf("Removing %s     ", rng)
				nChunksRemoved++
			} else {
				// We did not remove the chunk, so we need to keep track of where the truncated index ends
				latestChunk = base.Max(latestChunk, rng.Last)
				bar.Prefix = fmt.Sprintf("Not removing %s", rng)
			}
			bar.Tick()
			return true, nil
		}

		walker := walk.NewCacheWalker(
			chain,
			opts.Globals.TestMode,
			100, /* maxTests */
			truncateIndex,
		)
		if err := walker.WalkBloomFilters(blockNums); err != nil {
			errorChan <- err
			cancel()

		} else {
			bar.Prefix = fmt.Sprintf("Truncated to %d                    ", opts.Truncate)
			bar.Finish(true /* newLine */)
			bar = logger.NewBar(logger.BarOptions{
				Enabled: !testMode && !logger.IsTerminal(),
				Total:   20,
				Type:    logger.Expanding,
			})

			// We've made it this far (removed chunks and updated manifest) now we need to remove appearances
			// from any monitors that may exist which happen after the truncated block. Also, update the monitors'
			// header to reflect this new lastScanned block.
			nMonitorsTruncated := 0
			truncateMonitor := func(path string, info fs.FileInfo, err error) error {
				if err != nil {
					return err
				}
				if !info.IsDir() && strings.HasSuffix(path, ".mon.bin") {
					addr, _ := base.AddressFromPath(path, ".mon.bin")
					bar.Prefix = fmt.Sprintf("Truncating monitor for %s", addr.Hex())
					if !addr.IsZero() {
						mon, _ := monitor.NewMonitor(chain, addr, false /* create */)
						var removed bool
						if removed, err = mon.TruncateTo(chain, uint32(latestChunk)); err != nil {
							return err
						}
						if removed {
							nMonitorsTruncated++
						}
					}
					bar.Tick()
				}
				return nil
			}
			_ = filepath.Walk(config.PathToCache(chain)+"monitors", truncateMonitor)
			bar.Prefix = fmt.Sprintf("Truncated monitors to %d                                        ", opts.Truncate)
			bar.Finish(true /* newLine */)

			// All that's left to do is report on what happened.
			fin := "."
			if nChunksRemoved > 0 {
				fin = ", the manifest was updated."
			}
			msg1 := fmt.Sprintf("Truncated index to block %d (the latest full chunk).", latestChunk)
			msg2 := fmt.Sprintf("%d chunks removed, %d monitors truncated%s", nChunksRemoved, nMonitorsTruncated, fin)
			if opts.Globals.Format == "json" {
				s := types.Message{
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

var truncateWarning = `Are sure you want to remove index chunks after and including block {0} (Yn)? `
