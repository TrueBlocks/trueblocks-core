package chunksPkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinning"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/unchained"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
)

func (opts *ChunksOptions) pinChunk(ctx *WalkContext, path string, first bool) (bool, error) {
	result, err := pinning.PinChunk(opts.Globals.Chain, path, opts.Remote)
	if err != nil {
		return false, err
	}

	if ctx.Data != nil {
		m, castOk := ctx.Data.(*types.SimpleManifest)
		if !castOk {
			return true, fmt.Errorf("could not cast manifest")
		}
		if pinning.LocalDaemonRunning() {
			m.Chunks = append(m.Chunks, result.Local)
		} else {
			m.Chunks = append(m.Chunks, result.Remote)
		}
	}
	logger.Log(logger.Progress, "Pinning: ", result.Range)

	return true, nil
}

func (opts *ChunksOptions) HandlePinManifest(blockNums []uint64) error {
	defer opts.Globals.RenderFooter()
	err := opts.Globals.RenderHeader(types.SimpleManifest{}, &opts.Globals.Writer, opts.Globals.Format, opts.Globals.ApiMode, opts.Globals.NoHeader, true)
	if err != nil {
		return err
	}

	m := types.SimpleManifest{
		Version: version.ManifestVersion,
		Chain:   opts.Globals.Chain,
		Schemas: unchained.Schemas,
	}

	tsPath := config.GetPathToIndex(opts.Globals.Chain) + "ts.bin"
	if pinning.LocalDaemonRunning() {
		localService, _ := pinning.NewPinningService(opts.Globals.Chain, pinning.Local)
		if m.Databases, err = localService.PinFile(tsPath, true); err != nil {
			return err
		}
	}
	if opts.Remote {
		remoteService, _ := pinning.NewPinningService(opts.Globals.Chain, pinning.Pinata)
		if m.Databases, err = remoteService.PinFile(tsPath, true); err != nil {
			return err
		}
	}

	ctx := WalkContext{
		VisitFunc: opts.pinChunk,
		Data:      &m,
	}
	if err := opts.WalkIndexFiles(&ctx, cache.Index_Bloom, blockNums); err != nil {
		return err
	}

	return opts.Globals.RenderObject(m, true)
}
