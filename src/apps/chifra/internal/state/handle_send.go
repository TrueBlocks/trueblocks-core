// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package statePkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *StateOptions) HandleSend(rCtx *output.RenderCtx) error {
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		modelChan <- &types.Result{
			Name: "I am not right",
		}
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOpts())
}
