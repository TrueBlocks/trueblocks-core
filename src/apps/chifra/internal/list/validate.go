// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package listPkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *ListOptions) validateList() error {
	chain := opts.Globals.Chain

	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if opts.LastBlock == 0 {
		opts.LastBlock = utils.NOPOS
	}

	if opts.MaxRecords == 0 {
		opts.MaxRecords = 250
	}

	if opts.FirstBlock >= opts.LastBlock {
		msg := fmt.Sprintf("first_block (%d) must be strictly earlier than last_block (%d).", opts.FirstBlock, opts.LastBlock)
		return validate.Usage(msg)
	}

	if opts.LastBlock != utils.NOPOS && !opts.Globals.TestMode {
		latest := opts.Conn.GetLatestBlockNumber()
		if opts.LastBlock > latest {
			msg := fmt.Sprintf("latest block (%d) must be before the chain's latest block (%d).", opts.LastBlock, latest)
			return validate.Usage(msg)
		}
	}

	if opts.Globals.TestMode && opts.Unripe {
		return validate.Usage("--unripe is disabled for testing.")
	}

	if opts.Count && opts.MaxRecords != 250 {
		return validate.Usage("The {0} option is not available with the {1}-{2} option.", "--count", "--max_records", fmt.Sprintf("%d", opts.MaxRecords))
	}

	if opts.NoZero && !opts.Count {
		return validate.Usage("The {0} option is only available with the {1} option.", "--no_zero", "--count")
	}

	if len(opts.Globals.File) == 0 {
		err := validate.ValidateAtLeastOneAddr(opts.Addrs)
		if err != nil {
			return err
		}
	}

	// Note that this does not return if the index is not initialized
	if err := index.IndexIsInitialized(chain); err != nil {
		if opts.Globals.IsApiMode() {
			return err
		} else {
			logger.Fatal(err)
		}
	}

	return nil
}
