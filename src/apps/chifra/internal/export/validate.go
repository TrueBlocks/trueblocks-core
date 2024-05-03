// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package exportPkg

import (
	"errors"
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *ExportOptions) validateExport() error {
	chain := opts.Globals.Chain
	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if opts.LastBlock == 0 {
		opts.LastBlock = base.NOPOS
	}

	if opts.MaxRecords == 0 {
		opts.MaxRecords = 250
	}

	if !config.IsChainConfigured(chain) {
		return validate.Usage("chain {0} is not properly configured.", chain)
	}

	key := config.GetKey("trueblocks").License
	if opts.Neighbors && !strings.Contains(key, "+neighbors") {
		return validate.Usage("The {0} option requires a license key. Please contact us in our discord.", "--neighbors")
	}
	if opts.Accounting && !strings.Contains(key, "+accounting") {
		return validate.Usage("The {0} option requires a license key. Please contact us in our discord.", "--accounting")
	}

	if len(opts.Load) > 0 {
		// See https://pkg.go.dev/plugin
		return validate.Usage("The {0} option is currenlty disabled.", "--load")
	}

	if opts.tooMany() {
		return validate.Usage("Please choose only a single mode (--appearances, --logs, etc.)")
	}

	if opts.Traces && !opts.Conn.IsNodeTracing() {
		return validate.Usage("{0} requires tracing, err: {1}", "chifra export --traces", rpc.ErrTraceBlockMissing)
	}

	if opts.Count {
		if opts.Logs || opts.Traces || opts.Neighbors {
			return validate.Usage("The {0} option is not available{1}.", "--count", " with --logs, --traces, or --neighbors")
		}
	}

	if err := validate.ValidateAtLeastOneAddr(opts.Addrs); err != nil {
		for _, a := range opts.Addrs {
			if !base.IsValidAddress(a) {
				logger.Error("Invalid address: {0}", a)
			}
		}
		return err
	}

	for _, a := range opts.Addrs {
		if !base.IsValidAddress(a) {
			if len(a) < 10 {
				return validate.Usage("Invalid fourbyte: {0}", a)
			} else if len(a) > 60 {
				return validate.Usage("Invalid hash: {0}", a)
			} else {
				return validate.Usage("Invalid address: {0}", a)
			}
		}
	}

	if opts.Globals.TestMode && opts.Unripe {
		return validate.Usage("--unripe is disabled for testing.")
	}

	if opts.LastBlock == 0 {
		opts.LastBlock = base.NOPOS
	}

	if opts.FirstBlock >= opts.LastBlock {
		msg := fmt.Sprintf("first_block (%d) must be strictly earlier than last_block (%d).", opts.FirstBlock, opts.LastBlock)
		return validate.Usage(msg)
	}

	if opts.LastBlock != base.NOPOS {
		latest := opts.Conn.GetLatestBlockNumber()
		if opts.LastBlock > latest {
			msg := fmt.Sprintf("latest block (%d) must be before the chain's latest block (%d).", opts.LastBlock, latest)
			return validate.Usage(msg)
		}
	}

	if opts.Logs {
		for _, e := range opts.Emitter {
			if !base.IsValidAddress(e) {
				return validate.Usage("Invalid emitter: {0}", e)
			}
		}
		for _, t := range opts.Topics {
			if !validate.IsValidHash(t) {
				return validate.Usage("Invalid topic: {0}", t)
			}
		}
		for _, t := range opts.Topic {
			if !validate.IsValidHash(t) {
				return validate.Usage("Invalid topic: {0}", t)
			}
		}
	} else {
		if len(opts.Emitter) > 0 {
			return validate.Usage("The {0} option is only available with the {1} option.", "--emitter", "--logs")
		}
		if !opts.Receipts && len(opts.Topics) > 0 {
			return validate.Usage("You may only provide topics with the {0} option.", "--logs")
		}
		if len(opts.Topic) > 0 {
			return validate.Usage("The {0} option is only available with the {1} option.", "--topic", "--logs")
		}
		if opts.Relevant {
			return validate.Usage("The {0} option is only available with the {1} option.", "--relevant", "--logs")
		}
	}

	if !opts.Traces {
		if opts.Factory {
			return validate.Usage("The {0} option is only available with the {1} option.", "--factory", "--traces")
		}
	}

	if len(opts.Fourbytes) > 0 {
		if opts.Logs || opts.Receipts || opts.Appearances {
			return validate.Usage("The {0} option is only available {1} option.", "--fourbyte", "when exporting or with the --accounting")
		}
		for _, t := range opts.Fourbytes {
			if !validate.IsValidFourByte(t) {
				return validate.Usage("Invalid four byte: {0}", t)
			}
		}
	}

	if opts.Accounting {
		if len(opts.Addrs) != 1 {
			return validate.Usage("The {0} option is allows with only a single address.", "--accounting")
		}

		if !opts.Conn.IsNodeArchive() {
			return validate.Usage("The {0} option requires {1}.", "--accounting", "an archive node")
		}

		if opts.Globals.Chain != "mainnet" {
			logger.Warn("The --accounting option reports a spotPrice of one for all assets on non-mainnet chains.")
		}

		if len(opts.Flow) > 0 {
			if err := validate.ValidateEnum("--flow", opts.Flow, "[in|out|zero]"); err != nil {
				return err
			}

			if !opts.Statements {
				return validate.Usage("The {0} option is only available with the {1} option.", "--flow", "--statements")
			}
		}

	} else {
		if opts.Statements {
			return validate.Usage("The {0} option is only available with the {1} option.", "--statements", "--accounting")
		}

		if opts.Globals.Format == "ofx" {
			return validate.Usage("The {0} option is only available with the {1} option.", "--fmt ofx", "--accounting")
		}
	}

	if len(opts.Asset) > 0 && !opts.Statements {
		return validate.Usage("The {0} option is only available with the {1} option.", "--asset", "--statements")
	}

	if !validate.HasArticulationKey(opts.Articulate) {
		return validate.Usage("The {0} option requires an Etherscan API key.", "--articulate")
	}

	if err := index.IsInitialized(chain, config.ExpectedVersion()); err != nil {
		if (errors.Is(err, index.ErrNotInitialized) || errors.Is(err, index.ErrIncorrectHash)) && !opts.Globals.IsApiMode() {
			logger.Fatal(err)
		}
		return err
	}

	return opts.Globals.Validate()
	// if err != nil && strings.Contains(err.Error(), "option (ofx) must be one of") {
	// 	// not an error
	// 	err = nil
	// }
	// return err
}

func (opts *ExportOptions) tooMany() bool {
	cnt := 0
	if opts.Appearances {
		cnt++
	}
	if opts.Receipts {
		cnt++
	}
	if opts.Logs {
		cnt++
	}
	if opts.Traces {
		cnt++
	}
	if opts.Neighbors {
		cnt++
	}
	if opts.Accounting {
		cnt++
	}
	if opts.Withdrawals {
		cnt++
	}
	return cnt > 1
}
