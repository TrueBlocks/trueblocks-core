package chunksPkg

import (
	"context"
	"fmt"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index/bloom"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/sigintTrap"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/usage"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

func (opts *ChunksOptions) HandleTag(blockNums []uint64) error {
	chain := opts.Globals.Chain
	if opts.Globals.TestMode {
		logger.Warn("Tag option not tested.")
		return nil
	}

	tag := config.GetUnchained().Manifest
	if len(opts.Tag) > 0 {
		tag = opts.Tag
	}
	if !opts.Globals.IsApiMode() && !usage.QueryUser(strings.Replace(tagWarning, "{0}", tag, -1), "Not taagged") {
		return nil
	}

	_ = index.CleanTempIndexFolders(chain, []string{"ripe", "unripe", "maps", "staging"})

	userHitsCtrlC := false
	ctx, cancel := context.WithCancel(context.Background())

	fetchData := func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {
		nChunksTagged := 0
		tagIndex := func(walker *walk.CacheWalker, path string, first bool) (bool, error) {
			if strings.HasSuffix(path, ".backup") || strings.HasSuffix(path, ".gz") {
				logger.Fatal("should not happen ==> walked to a non-bloom file")
			}

			if path != index.ToBloomPath(path) {
				logger.Fatal("should not happen ==> we're spinning through the bloom filters")
			}

			if err := tagChunk(chain, tag, index.ToBloomPath(path), index.ToIndexPath(path)); err != nil {
				return false, err
			}

			nChunksTagged++
			if opts.Globals.Verbose {
				rng := base.RangeFromFilename(path)
				logger.Info(colors.Green+"Tagging chunk at "+rng.String(), colors.Off)
			}

			return true, nil
		}

		walker := walk.NewCacheWalker(
			chain,
			opts.Globals.TestMode,
			100, /* maxTests */
			tagIndex,
		)

		if err := walker.WalkBloomFilters(blockNums); err != nil {
			errorChan <- err
			cancel()

		} else {
			// All that's left to do is report on what happened.
			msg := fmt.Sprintf("%d chunks were retagged with %s.", nChunksTagged, tag)
			if userHitsCtrlC {
				msg += colors.Yellow + "Finishing work. please wait..." + colors.Off
			}
			if opts.Globals.Format == "json" {
				s := types.SimpleMessage{
					Msg: msg,
				}
				modelChan <- &s
			} else {
				logger.Info(msg)
			}
		}
	}

	cleanOnQuit := func() {
		userHitsCtrlC = true
		logger.Error("Tagging did not complete. The index is in an unknown state. Rerun the command (and")
		logger.Error("allow it to complete), or run chifra init to correct the inconsistency.")
	}
	trapChannel := sigintTrap.Enable(ctx, cancel, cleanOnQuit)
	defer sigintTrap.Disable(trapChannel)

	opts.Globals.NoHeader = true
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}

var tagWarning = `Tag the index with {0}? (Note: this is a non-recoverable change.) (Yy)? `

// tagChunk updates the manifest version in the chunk's header
func tagChunk(chain string, hash string, bloomFn, indexFn string) (err error) {
	indexBackup := indexFn + ".backup"
	bloomBackup := bloomFn + ".backup"

	defer func() {
		if err != nil {
			// If the backup files still exist when the function ends, something went wrong, reset everything
			if file.FileExists(indexBackup) {
				_, _ = file.Copy(indexFn, indexBackup)
				_ = os.Remove(indexBackup)
			}
			if file.FileExists(bloomBackup) {
				_, _ = file.Copy(bloomFn, bloomBackup)
				_ = os.Remove(bloomBackup)
			}
		}
	}()

	if _, err = file.Copy(indexBackup, indexFn); err != nil {
		return err
	} else if _, err = file.Copy(bloomBackup, bloomFn); err != nil {
		return err
	}

	if err := index.UpdateIndexHeader(chain, "unused", indexFn, true /* unused */); err != nil {
		return err
	}

	if err = bloom.UpdateBloomHeader(chain, "unused", bloomFn, true /* unused */); err != nil {
		return err
	}

	os.Remove(indexBackup)
	os.Remove(bloomBackup)

	return nil
}
