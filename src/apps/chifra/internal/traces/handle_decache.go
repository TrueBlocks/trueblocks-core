// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package tracesPkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/decache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *TracesOptions) HandleDecache() error {
	chain := opts.Globals.Chain
	opts.Conn = rpcClient.NewReadOnlyConnection(chain)

	itemsToRemove, err := decache.TransactionLocationsFromIds(opts.Conn, opts.TransactionIds)
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
