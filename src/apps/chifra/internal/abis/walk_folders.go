package abisPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

func (opts *AbisOptions) ForEveryAbi(known bool, vFunc walk.ForEveryFunc, errorChan chan error) {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode

	paths := make([]string, 0, 2)
	if known {
		paths = append(paths, abi.PathToKnownAbis(""))
	}

	if !testMode {
		paths = append(paths, abi.PathToAbisCache(chain, ""))
	}

	for _, path := range paths {
		_ = walk.ForEveryFileInFolder(path, vFunc, errorChan)
	}
}
