// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package namesPkg

import (
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/types"
)

// HandleCount handles the chifra abis --count command.
func (opts *NamesOptions) HandleCount(rCtx *output.RenderCtx) error {
	testMode := opts.Globals.TestMode
	chain := opts.Globals.Chain

	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		s := types.Count{}
		if testMode {
			s.Count = 199
		} else {
			configPath := config.MustGetPathToChainConfig(chain)
			t := opts.getType()
			if t&types.Custom != 0 {
				if n, err := file.WordCount(filepath.Join(configPath, "names_custom.tab"), true); err != nil {
					errorChan <- err
					return
				} else {
					s.Count += uint64(n)
				}
			}
			if t&types.Prefund != 0 {
				if n, err := file.WordCount(filepath.Join(configPath, "allocs.csv"), true); err != nil {
					errorChan <- err
					return
				} else {
					s.Count += uint64(n)
				}
			}
			if t&types.Regular != 0 {
				if n, err := file.WordCount(filepath.Join(configPath, "names.tab"), true); err != nil {
					errorChan <- err
					return
				} else {
					s.Count += uint64(n)
				}
			}
			if t&types.Baddress != 0 {
				s.Count += uint64(names.BaddressCount())
			}
		}
		modelChan <- &s
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOpts())
}
