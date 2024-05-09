package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoChunks tests the chunks sdk function
func DoChunks() {
	file.EstablishFolder("sdkFuzzer-output/chunks")
	opts := sdk.ChunksOptions{
		FirstBlock: 0,
		LastBlock:  base.NOPOSN,
		MaxAddrs:   base.NOPOS,
	}

	// BlockIds   []string     `json:"blocks,omitempty"`
	// Check      bool         `json:"check,omitempty"`
	// Pin        bool         `json:"pin,omitempty"`
	// Publish    bool         `json:"publish,omitempty"`
	// Publisher  base.Address `json:"publisher,omitempty"`
	// Remote     bool         `json:"remote,omitempty"`
	// Belongs    []string     `json:"belongs,omitempty"`
	// Deep       bool         `json:"deep,omitempty"`
	// Rewrite    bool         `json:"rewrite,omitempty"`
	// List       bool         `json:"list,omitempty"`
	// Unpin      bool         `json:"unpin,omitempty"`
	// Count      bool         `json:"count,omitempty"`
	// Sleep      float64      `json:"sleep,omitempty"`
	ShowHeader("ChunksManifest", &opts)

	baseFn := "chunks/chunks"
	fn := getFilename(baseFn, &opts.Globals)
	TestChunks("manifest", fn, &opts)
	TestChunks("index", fn, &opts)
	// TestChunks("blooms", fn, &opts)
	// TestChunks("pins", fn, &opts)
	// TestChunks("addresses", fn, &opts)
	// TestChunks("appearances", fn, &opts)
	TestChunks("stats", fn, &opts)

	// func (opts *ChunksOptions) ChunksTruncate(val base.Blknum) ([]types.Message, *types.MetaData, error) {
	// func (opts *ChunksOptions) ChunksDiff() ([]types.Message, *types.MetaData, error) {
	// func (opts *ChunksOptions) ChunksTag(val string) ([]types.Message, *types.MetaData, error) {
}

func TestChunks(which, fn string, opts *sdk.ChunksOptions) {
	fn += "-" + which
	logger.Info("-------------", fn, "-------------")
	switch which {
	case "manifest":
		if manifest, _, err := opts.ChunksManifest(); err != nil {
			ReportError(fn, err)
		} else {
			if err := SaveToFile[types.ChunkManifest](fn, manifest); err != nil {
				ReportError(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "index":
		if index, _, err := opts.ChunksIndex(); err != nil {
			ReportError(fn, err)
		} else {
			if err := SaveToFile[types.ChunkIndex](fn, index); err != nil {
				ReportError(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "blooms":
		if blooms, _, err := opts.ChunksBlooms(); err != nil {
			ReportError(fn, err)
		} else {
			if err := SaveToFile[types.ChunkBloom](fn, blooms); err != nil {
				ReportError(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "pins":
		// TODO: Not sure this is included in the SDK
		opts.List = true
		if pins, _, err := opts.ChunksPins(); err != nil {
			ReportError(fn, err)
		} else {
			if err := SaveToFile[types.ChunkPin](fn, pins); err != nil {
				ReportError(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
		opts.List = false
	case "addresses":
		if addresses, _, err := opts.ChunksAddresses(); err != nil {
			ReportError(fn, err)
		} else {
			if err := SaveToFile[types.ChunkAddress](fn, addresses); err != nil {
				ReportError(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "appearances":
		if appearances, _, err := opts.ChunksAppearances(); err != nil {
			ReportError(fn, err)
		} else {
			if err := SaveToFile[types.ChunkAppearance](fn, appearances); err != nil {
				ReportError(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "stats":
		if stats, _, err := opts.ChunksStats(); err != nil {
			ReportError(fn, err)
		} else {
			if err := SaveToFile[types.ChunkStats](fn, stats); err != nil {
				ReportError(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	}
}
