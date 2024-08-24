// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package abisPkg

import (
	"os"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// HandleList handles the chifra abis --list command.
func (opts *AbisOptions) HandleList(rCtx *output.RenderCtx) error {
	testMode := opts.Globals.TestMode

	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		abiArray := make([]types.Abi, 0, 100)
		vFunc := func(fn string, vP any) (bool, error) {
			if strings.HasSuffix(fn, ".json") {
				info, _ := os.Stat(fn)
				abi := types.Abi{
					FileSize:    file.FileSize(fn),
					LastModDate: info.ModTime().Format("2006-01-02 15:04:05"),
					IsKnown:     strings.Contains(fn, "known"),
				}
				abi.Path, abi.Name = filepath.Split(fn)
				if len(abi.Name) > 0 {
					if strings.HasPrefix(abi.Name, "0x") {
						abi.Address = base.HexToAddress(strings.Replace(abi.Name, ".json", "", -1))
					}
					if opts.Globals.Verbose {
						contents := file.AsciiFileToString(filepath.Join(abi.Path, abi.Name))
						abi.NFunctions = int64(strings.Count(contents, "\"function\""))
						abi.NEvents = int64(strings.Count(contents, "\"event\""))
					}
					if testMode {
						abi.LastModDate = "--date--"
						abi.Path = strings.ReplaceAll(abi.Path, config.PathToRootConfig(), ".")
					}
					abiArray = append(abiArray, abi)
				}
			}
			return true, nil
		}

		opts.ForEveryAbi(true, vFunc, nil)
		// sort.Slice(abiArray,
		// 	types.Cmp(abiArray,
		// 		types.AbiBy(types.AbiIsKnown, types.Descending),
		// 		types.AbiBy(types.AbiName, types.Ascending),
		// 	),
		// )
		for _, abi := range abiArray {
			modelChan <- &abi
		}
	}

	extraOpts := map[string]any{
		"list":      true,
		"loadNames": true,
	}
	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOptsWithExtra(extraOpts))
}
