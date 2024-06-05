// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package tracesPkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/decache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

func (opts *TracesOptions) HandleDecache() error {
	itemsToRemove, err := decache.LocationsFromTransactions(opts.Conn, opts.TransactionIds)
	if err != nil {
		return err
	}

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		showProgress := opts.Globals.ShowProgress()
		if msg, err := decache.Decache(opts.Conn, itemsToRemove, showProgress, walk.Cache_Traces); err != nil {
			errorChan <- err
		} else {
			s := types.Message{
				Msg: msg,
			}
			modelChan <- &s
		}
	}

	opts.Globals.NoHeader = true
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}
