package chunksPkg

import (
	"context"
	"fmt"
	"io/fs"
	"os"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/usage"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

// TODO: We need to make sure, when we truncate, that we truncate the corresponding maps files as well.

func (opts *ChunksOptions) HandleTruncate(blockNums []uint64) error {
	chain := opts.Globals.Chain

	if opts.Globals.TestMode {
		logger.Warn("Truncate option not tested.")
		return nil
	}

	if !opts.Globals.IsApiMode() && !usage.QueryUser(strings.Replace(warning, "{0}", fmt.Sprintf("%d", opts.Truncate), -1), "Not truncating") {
		return nil
	}

	_ = index.CleanTempIndexFolders(chain, []string{"ripe", "unripe", "staging"})

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {

		// First, we will remove the chunks and update the manifest. We do this separately for
		// each chunk, so that if we get interrupted, we have a relatively sane state (although,
		// we will have to manually repair the index with chifra init --all if this fails. Keep track
		// of the last chunks remaining.
		latestChunk := uint64(0)
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

			testRange := base.FileRange{First: opts.Truncate, Last: utils.NOPOS}
			if rng.Intersects(testRange) {
				if err = manifest.RemoveChunk(chain, index.ToBloomPath(path), index.ToIndexPath(path)); err != nil {
					return false, err
				}
				nChunksRemoved++
				if opts.Globals.Verbose {
					logger.Info(colors.Red, "Removing chunk at "+rng.String(), "max:", latestChunk, colors.Off)
				}
			} else {
				// We did not remove the chunk, so we need to keep track of where the truncated index ends
				latestChunk = utils.Max(latestChunk, rng.Last)
				if opts.Globals.Verbose {
					logger.Info(colors.Green, "Not removing chunk at "+rng.String(), "max:", latestChunk, colors.Off)
				}
			}

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
			// We've made it this far (removed chunks and updated manifest) now we need to remove appearances
			// from any monitors that may exist which happen after the truncated block. Also, update the monitors'
			// header to reflect this new lastScanned block.
			nMonitorsTruncated := 0
			truncateMonitor := func(path string, info fs.FileInfo, err error) error {
				if err != nil {
					return err
				}
				if !info.IsDir() {
					addr, _ := base.AddressFromPath(path, ".mon.bin")
					if !addr.IsZero() {
						mon := monitor.NewMonitor(chain, addr, false /* create */)
						var removed bool
						if removed, err = mon.TruncateTo(chain, uint32(latestChunk)); err != nil {
							return err
						}
						if removed {
							nMonitorsTruncated++
						}
					}
				}
				return nil
			}
			_ = filepath.Walk(config.GetPathToCache(chain)+"monitors", truncateMonitor)

			// All that's left to do is report on what happened.
			fin := "."
			if nChunksRemoved > 0 {
				fin = ", the manifest was updated."
			}
			msg1 := fmt.Sprintf("Truncated index to block %d (the latest full chunk).", latestChunk)
			msg2 := fmt.Sprintf("%d chunks removed, %d monitors truncated%s", nChunksRemoved, nMonitorsTruncated, fin)
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
