// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package listPkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *ListOptions) validateList() error {
	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if opts.LastBlock == 0 {
		opts.LastBlock = utils.NOPOS
	}

	if opts.FirstBlock >= opts.LastBlock {
		msg := fmt.Sprintf("first_block (%d) must be strictly earlier than last_block (%d).", opts.FirstBlock, opts.LastBlock)
		return validate.Usage(msg)
	}

	if opts.LastBlock != utils.NOPOS {
		provider := config.GetRpcProvider(opts.Globals.Chain)
		latest := rpcClient.BlockNumber(provider)
		if opts.LastBlock > latest {
			msg := fmt.Sprintf("latest block (%d) must be before the chain's latest block (%d).", opts.LastBlock, latest)
			return validate.Usage(msg)
		}
	}

	if len(opts.Globals.File) == 0 {
		err := validate.ValidateAtLeastOneAddr(opts.Addrs)
		if err != nil {
			return err
		}
	}

	// Note that this does not return if the index is not initialized
	if err := index.IndexIsInitialized(opts.Globals.Chain); err != nil {
		if opts.Globals.IsApiMode() {
			return err
		} else {
			logger.Fatal(err)
		}
	}

	return nil
}
