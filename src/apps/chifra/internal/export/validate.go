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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *ExportOptions) validateExport() error {
	chain := opts.Globals.Chain
	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if opts.LastBlock == 0 {
		opts.LastBlock = base.NOPOSN
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

	if which, tooMany := opts.tooMany(); tooMany {
		return validate.Usage("Please choose only a single mode ({0}, etc.)", strings.Join(which, ", "))
	}

	if opts.Traces {
		err, ok := opts.Conn.IsNodeTracing()
		if !ok {
			return validate.Usage("{0} requires tracing, err: {1}", "chifra export --traces", err.Error())
		}
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
		opts.LastBlock = base.NOPOSN
	}

	if opts.FirstBlock >= opts.LastBlock {
		msg := fmt.Sprintf("first_block (%d) must be strictly earlier than last_block (%d).", opts.FirstBlock, opts.LastBlock)
		return validate.Usage(msg)
	}

	if opts.LastBlock != base.NOPOSN {
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
		if opts.Nfts {
			return validate.Usage("The {0} option is only available with the {1} option.", "--nfts", "--logs")
		}
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
			return validate.Usage("The {0} option allows only a single address.", "--accounting")
		}

		if !opts.Conn.IsNodeArchive() {
			return validate.Usage("The {0} option requires {1}.", "--accounting", "an archive node")
		}

		if opts.Globals.Chain != "mainnet" {
			logger.Warn("The --accounting option reports a spotPrice of one for all assets on non-mainnet chains.")
		}

		if opts.Statements && opts.Transfers ||
			opts.Statements && opts.Assets ||
			opts.Transfers && opts.Assets {
			return validate.Usage("Choose only one of {0}, {1}, or {2}.", "--statements", "--transfers", "--assets")
		}

		if len(opts.Flow) > 0 {
			if err := validate.ValidateEnum("--flow", opts.Flow, "[in|out|zero]"); err != nil {
				return err
			}

			if !opts.Statements && !opts.Transfers && !opts.Assets {
				return validate.Usage("The {0} option is only available with the {1} option.", "--flow", "--statements, --transfers, or --asset")
			}
		}

	} else {
		if opts.Statements {
			return validate.Usage("The {0} option is only available with the {1} option.", "--statements", "--accounting")
		}

		if opts.Transfers {
			return validate.Usage("The {0} option is only available with the {1} option.", "--transfers", "--accounting")
		}

		if opts.Assets {
			return validate.Usage("The {0} option is only available with the {1} option.", "--assets", "--accounting")
		}

		if opts.Globals.Format == "ofx" {
			return validate.Usage("The {0} option is only available with the {1} option.", "--fmt ofx", "--accounting")
		}
	}

	if len(opts.Asset) > 0 && (!opts.Statements && !opts.Transfers && !opts.Balances) {
		return validate.Usage("The {0} option is only available with the {1} option.", "--asset", "--transfers, --balances, or --statements")
	} else if opts.Balances && len(opts.Asset) == 0 {
		opts.Asset = []string{base.FAKE_ETH_ADDRESS.Hex()}
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

func (opts *ExportOptions) tooMany() ([]string, bool) {
	cnt := 0
	which := []string{}
	if opts.Appearances {
		which = append(which, "--appearances")
		cnt++
	}
	if opts.Receipts {
		which = append(which, "--receipts")
		cnt++
	}
	if opts.Logs {
		which = append(which, "--logs")
		cnt++
	}
	if opts.Traces {
		which = append(which, "--traces")
		cnt++
	}
	if opts.Neighbors {
		which = append(which, "--neighbors")
		cnt++
	}
	if opts.Accounting {
		which = append(which, "--accounting")
		cnt++
	}
	if opts.Withdrawals {
		which = append(which, "--withdrawals")
		cnt++
	}

	if len(which) > 2 {
		which = which[:2]
	}

	m := map[string]bool{}
	for _, w := range which {
		m[w] = true
	}

	if len(which) == 2 && m["--accounting"] && m["--traces"] {
		return []string{}, false
	}

	return which, cnt > 1
}
