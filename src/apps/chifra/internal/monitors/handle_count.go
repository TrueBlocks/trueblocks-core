// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package monitorsPkg

import (
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

// HandleCount handles the chifra abis --count command.
func (opts *MonitorsOptions) HandleCount(rCtx *output.RenderCtx) error {
	testMode := opts.Globals.TestMode
	chain := opts.Globals.Chain

	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		count := uint64(0)
		if testMode {
			count = 199
		} else {
			vFunc := func(fn string, vP any) (bool, error) {
				_, name := filepath.Split(fn)
				if !strings.Contains(fn, "staging") && strings.HasSuffix(name, ".bin") {
					count++
				}
				return true, nil
			}
			path := filepath.Join(config.PathToCache(chain), "monitors")
			_ = walk.ForEveryFileInFolder(path, vFunc, errorChan)
		}

		s := types.Count{
			Count: count,
		}

		modelChan <- &s
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOpts())
}
