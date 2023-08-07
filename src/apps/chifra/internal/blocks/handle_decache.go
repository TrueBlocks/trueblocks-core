// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package blocksPkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/decache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *BlocksOptions) HandleDecache() error {
	chain := opts.Globals.Chain
	opts.Conn = rpc.NewReadOnlyConnection(chain)

	itemsToRemove, err := decache.BlockLocationsFromIds(opts.Conn, opts.BlockIds)
	if err != nil {
		return err
	}

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {
		if msg, err := decache.Decache(opts.Conn, itemsToRemove, opts.Globals.TestMode, opts.Globals.Verbose); err != nil {
			errorChan <- err
		} else {
			s := types.SimpleMessage{
				Msg: msg,
			}
			modelChan <- &s
		}
	}

	opts.Globals.NoHeader = true
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}
