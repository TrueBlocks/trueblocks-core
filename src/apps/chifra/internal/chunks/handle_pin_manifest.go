package chunksPkg

import (
	"strings"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/paths"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinning"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/unchained"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
)

func pinChunk(walker *index.IndexWalker, path string, first bool) (bool, error) {
	var castOk bool
	var opts *ChunksOptions
	if opts, castOk = walker.GetOpts().(*ChunksOptions); !castOk {
		logger.Fatal("should not happen ==> cannot cast ChunksOptions in pinChunk")
		return false, nil
	}

	var man *types.SimpleManifest
	if man, castOk = walker.GetData().(*types.SimpleManifest); !castOk {
		logger.Fatal("should not happen ==> cannot cast manifest.Manifest in cleanIndex")
		return false, nil
	}

	result, err := pinning.PinChunk(opts.Globals.Chain, path, opts.Remote)
	if err != nil {
		return false, err
	}

	if pinning.LocalDaemonRunning() {
		man.Chunks = append(man.Chunks, result.Local)
		logger.Log(logger.Progress, "Pinning: ", result.Local, spaces)
	} else {
		man.Chunks = append(man.Chunks, result.Remote)
		logger.Log(logger.Progress, "Pinning: ", result.Remote, spaces)
	}

	if opts.Globals.Verbose {
		logger.Log(logger.Progress, "Pinning", path)
	}
	if opts.Sleep > 0 {
		time.Sleep(time.Duration(opts.Sleep) * time.Second)
	}

	return true, nil
}

func (opts *ChunksOptions) HandlePinManifest(blockNums []uint64) error {
	defer opts.Globals.RenderFooter()
	err := opts.Globals.RenderHeader(types.SimpleManifest{}, &opts.Globals.Writer, opts.Globals.Format, opts.Globals.NoHeader, true)
	if err != nil {
		return err
	}

	m := types.SimpleManifest{
		Version: version.ManifestVersion,
		Chain:   opts.Globals.Chain,
		Schemas: unchained.Schemas,
	}

	if err = pinning.PinTimestamps(opts.Globals.Chain, opts.Remote); err != nil {
		return err
	}

	walker := index.NewIndexWalker(
		opts.Globals.Chain,
		opts.Globals.TestMode,
		100, /* maxTests */
		opts,
		pinChunk,
		&m,
	)
	if err := walker.WalkIndexFiles(paths.Index_Bloom, blockNums); err != nil {
		return err
	}

	return opts.Globals.RenderObject(m, true)
}

var spaces = strings.Repeat(" ", 20)
