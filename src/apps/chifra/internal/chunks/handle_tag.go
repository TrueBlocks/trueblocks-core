package chunksPkg

import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ranges"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/sigintTrap"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/usage"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

func (opts *ChunksOptions) HandleTag(rCtx *output.RenderCtx, blockNums []base.Blknum) error {
	chain := opts.Globals.Chain
	if opts.Globals.TestMode {
		logger.Warn("Tag option not tested.")
		return nil
	}

	if !opts.Globals.IsApiMode() && !usage.QueryUser(usage.Replace(tagWarning, opts.Tag), "Not taagged") {
		return nil
	}

	_ = file.CleanFolder(chain, config.PathToIndex(chain), []string{"ripe", "unripe", "maps", "staging"})

	userHitCtrlC := false

	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		nChunksTagged := 0
		man, err := manifest.LoadManifest(chain, opts.PublisherAddr, manifest.LocalCache)
		if err != nil {
			return
		}
		showProgress := opts.Globals.ShowProgress()
		bar := logger.NewBar(logger.BarOptions{
			Enabled: showProgress,
			Total:   int64(len(man.Chunks)),
		})
		tagIndex := func(walker *walk.CacheWalker, path string, first bool) (bool, error) {
			_ = walker
			_ = first
			if !strings.HasSuffix(path, ".bloom") {
				logger.Fatal("should not happen ==> walked to a non-bloom file")
			} else if path != index.ToBloomPath(path) {
				logger.Fatal("should not happen ==> we're spinning through the bloom filters")
			}

			var chunk index.Chunk
			if err := chunk.Tag(opts.Tag, path); err != nil {
				return false, err
			}

			nChunksTagged++
			if opts.Globals.Verbose {
				rng := ranges.RangeFromFilename(path)
				logger.Info(colors.Green+"Tagging chunk at "+rng.String()+" with "+opts.Tag+strings.Repeat(" ", 20), colors.Off)
			}
			bar.Tick()

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
			rCtx.Cancel()

		} else {
			bar.Finish(true /* newLine */)

			man.Version = opts.Tag
			man.Specification = base.IpfsHash(config.SpecTags[opts.Tag])
			_ = man.SaveManifest(chain, config.PathToManifestFile(chain))

			// All that's left to do is report on what happened.
			msg := fmt.Sprintf("%d chunks were retagged with %s.", nChunksTagged, opts.Tag)
			if userHitCtrlC {
				msg += colors.Yellow + "Finishing work. please wait..." + colors.Off
			}
			if opts.Globals.Format == "json" {
				s := types.Message{
					Msg: msg,
				}
				modelChan <- &s
			} else {
				logger.Info(msg)
			}
		}
	}

	cleanOnQuit := func() {
		userHitCtrlC = true
		logger.Error("Tagging did not complete. The index is in an unknown state. Rerun the command (and")
		logger.Error("allow it to complete), or run chifra init to correct the inconsistency.")
	}
	trapChannel := sigintTrap.Enable(rCtx.Ctx, rCtx.Cancel, cleanOnQuit)
	defer sigintTrap.Disable(trapChannel)

	opts.Globals.NoHeader = true
	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOpts())
}

var tagWarning = `Tag the index with version {0}? This is a non-recoverable operation. (Yn)? `
