// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package abisPkg

import (
	"os"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

// HandleList handles the chifra monitors --list command.
func (opts *AbisOptions) HandleList(rCtx *output.RenderCtx) error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	paths := []string{
		filepath.Join(config.PathToRootConfig(), "abis"),
		filepath.Join(config.PathToCache(chain), "abis"),
	}
	if testMode {
		paths = paths[:1]
	}

	abiArray := make([]types.AbiFile, 0, 100)
	vFunc := func(fn string, vP any) (bool, error) {
		if strings.HasSuffix(fn, ".json") {
			info, _ := os.Stat(fn)
			date := info.ModTime().Format("2006-01-02 15:04:05")
			if testMode {
				date = "--date--"
			}
			abiFile := types.AbiFile{
				FileSize:    file.FileSize(fn),
				LastModDate: date,
			}
			abiFile.Path, abiFile.Name = filepath.Split(fn)
			if testMode {
				abiFile.Path = strings.ReplaceAll(abiFile.Path, config.PathToRootConfig(), "./")
			}
			if len(abiFile.Name) > 0 {
				abiArray = append(abiArray, abiFile)
			}
		}
		return true, nil
	}

	for _, path := range paths {
		_ = walk.ForEveryFileInFolder(path, vFunc, nil)
	}

	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		for _, abi := range abiArray {
			modelChan <- &abi
		}
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOpts())
}
