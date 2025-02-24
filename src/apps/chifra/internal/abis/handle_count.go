// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package abisPkg

import (
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// HandleCount handles the chifra abis --count command.
func (opts *AbisOptions) HandleCount(rCtx *output.RenderCtx) error {
	testMode := opts.Globals.TestMode

	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		_ = errorChan
		count := uint64(0)
		if testMode {
			count = 199
		} else {
			vFunc := func(fn string, vP any) (bool, error) {
				_ = vP
				_, name := filepath.Split(fn)
				if strings.HasSuffix(name, ".json") {
					count++
				}
				return true, nil
			}
			opts.ForEveryAbi(true, vFunc, nil)
		}

		s := types.Count{
			Count: count,
		}

		modelChan <- &s
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOpts())
}
