package chunksPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

func (opts *ChunksOptions) HandleTag(blockNums []uint64) error {
	// chain := opts.Globals.Chain
	if opts.Globals.TestMode {
		logger.Warn("Tag option not tested.")
		return nil
	}

	// specVersion := config.GetUnchained().SpecVersion
	// if len(opts.Tag) > 0 {
	// 	specVersion = opts.Tag
	// }

	// if !opts.Globals.IsApiMode() && !usage.QueryUser(usage.Replace(tagWarning, specVersion), "Not taagged") {
	// 	return nil
	// }

	// _ = file.CleanFolder(chain, config.PathToIndex(chain), []string{"ripe", "unripe", "maps", "staging"})

	// userHitsCtrlC := false
	// ctx, cancel := context.WithCancel(context.Background())

	// fetchData := func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {
	// 	nChunksTagged := 0
	// 	man, err := manifest.ReadManifest(chain, opts.PublisherAddr, manifest.FromCache)
	// 	if err != nil {
	// 		return
	// 	}
	// 	bar := logger.NewBar(logger.BarOptions{
	// 		Enabled: !opts.Globals.TestMode,
	// 		Total:   int64(len(man.Chunks)),
	// 	})
	// 	tagIndex := func(walker *walk.CacheWalker, path string, first bool) (bool, error) {
	// 		if !strings.HasSuffix(path, ".bloom") {
	// 			logger.Fatal("should not happen ==> walked to a non-bloom file")
	// 		} else if path != index.ToBloomPath(path) {
	// 			logger.Fatal("should not happen ==> we're spinning through the bloom filters")
	// 		}

	// 		var chunk index.Chunk
	// 		if err := chunk.Tag(chain, specVersion, false /* unused */, path); err != nil {
	// 			return false, err
	// 		}

	// 		nChunksTagged++
	// 		if opts.Globals.Verbose {
	// 			rng := base.RangeFromFilename(path)
	// 			logger.Info(colors.Green+"Tagging chunk at "+rng.String()+" with "+specVersion+strings.Repeat(" ", 20), colors.Off)
	// 		}
	// 		bar.Tick()

	// 		return true, nil
	// 	}

	// 	walker := walk.NewCacheWalker(
	// 		chain,
	// 		opts.Globals.TestMode,
	// 		100, /* maxTests */
	// 		tagIndex,
	// 	)

	// 	if err := walker.WalkBloomFilters(blockNums); err != nil {
	// 		errorChan <- err
	// 		cancel()

	// 	} else {
	// 		bar.Finish(true)

	// 		// All that's left to do is report on what happened.
	// 		tag := config.HeaderTag(specVersion)
	// 		msg := fmt.Sprintf("%d chunks were retagged with %s.", nChunksTagged, tag)
	// 		if userHitsCtrlC {
	// 			msg += colors.Yellow + "Finishing work. please wait..." + colors.Off
	// 		}
	// 		if opts.Globals.Format == "json" {
	// 			s := types.SimpleMessage{
	// 				Msg: msg,
	// 			}
	// 			modelChan <- &s
	// 		} else {
	// 			logger.Info(msg)
	// 		}
	// 	}
	// }

	// cleanOnQuit := func() {
	// 	userHitsCtrlC = true
	// 	logger.Error("Tagging did not complete. The index is in an unknown state. Rerun the command (and")
	// 	logger.Error("allow it to complete), or run chifra init to correct the inconsistency.")
	// }
	// trapChannel := sigintTrap.Enable(ctx, cancel, cleanOnQuit)
	// defer sigintTrap.Disable(trapChannel)

	// opts.Globals.NoHeader = true
	// return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
	return nil
}

// var tagWarning = `Tag the index with version {0}? This is a non-recoverable operation. (Yy)? `