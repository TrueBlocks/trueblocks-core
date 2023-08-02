// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package whenPkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *WhenOptions) HandleTimestampCount() error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode

	// TODO: Why does this have to dirty the caller?
	settings := rpcClient.DefaultRpcOptionsSettings{
		Chain: chain,
		Opts:  opts,
	}
	opts.Conn = settings.DefaultRpcOptions()
	if !opts.Conn.Store.ReadOnly() {
		opts.Conn.LatestBlockTimestamp = opts.Conn.GetBlockTimestamp(chain, nil)
	}

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {
		if count, err := tslib.NTimestamps(chain); err != nil {
			errorChan <- err
			return

		} else {
			if testMode {
				count = 5000000
			}

			s := simpleTimestampCount{
				Count: count,
			}

			modelChan <- &s
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}
