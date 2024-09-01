package abisPkg

import (
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

func (opts *AbisOptions) ForEveryAbi(known bool, vFunc walk.ForEveryFunc, errorChan chan error) {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode

	paths := make([]string, 0, 2)
	if known {
		paths = append(paths, filepath.Join(config.PathToRootConfig(), "abis"))
	}

	if !testMode {
		paths = append(paths, filepath.Join(config.PathToCache(chain), "abis"))
	}

	for _, path := range paths {
		_ = walk.ForEveryFileInFolder(path, vFunc, errorChan)
	}
}
