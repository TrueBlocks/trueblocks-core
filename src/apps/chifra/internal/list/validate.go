// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package listPkg

import (
	"errors"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *ListOptions) validateList() error {
	chain := opts.Globals.Chain

	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if !config.IsChainConfigured(chain) {
		return validate.Usage("chain {0} is not properly configured.", chain)
	}

	if opts.LastBlock == 0 {
		opts.LastBlock = base.NOPOSN
	}

	if opts.MaxRecords == 0 {
		opts.MaxRecords = 250
	}

	if opts.FirstBlock >= opts.LastBlock {
		msg := fmt.Sprintf("first_block (%d) must be strictly earlier than last_block (%d).", opts.FirstBlock, opts.LastBlock)
		return validate.Usage(msg)
	}

	if opts.LastBlock != base.NOPOSN && !opts.Globals.TestMode {
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
		err := validate.ValidateAtLeastOneNonSentinal(opts.Addrs)
		if err != nil {
			return err
		}
	}

	if len(opts.Publisher) > 0 {
		err := validate.ValidateExactlyOneAddr([]string{opts.Publisher})
		if err != nil {
			return err
		}
	}

	if err := index.IsInitialized(chain, config.ExpectedVersion()); err != nil {
		if (errors.Is(err, index.ErrNotInitialized) || errors.Is(err, index.ErrIncorrectHash)) && !opts.Globals.IsApiMode() {
			logger.Fatal(err)
		}
		return err
	}

	return nil
}
