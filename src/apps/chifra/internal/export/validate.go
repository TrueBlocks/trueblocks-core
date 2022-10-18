// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package exportPkg

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *ExportOptions) validateExport() error {
	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if len(opts.Globals.File) == 0 {
		if err := validate.ValidateAtLeastOneAddr(opts.Addrs); err != nil {
			return err
		}
	}

	if len(opts.Flow) > 0 {
		if err := validate.ValidateEnum("--flow", opts.Flow, "[in|out|zero]"); err != nil {
			return err
		}
		if !opts.Statements {
			return validate.Usage("The {0} option is only available with {1} option.", "--flow", "--statements")
		}
	}

	if opts.Globals.TestMode && opts.Unripe {
		return validate.Usage("--unripe are disabled for testing.")
	}

	if opts.Count && (opts.Logs || opts.Receipts || opts.Traces || opts.Statements || opts.Neighbors) {
		return validate.Usage("The {0} option is only available with transactional options.", "--count")
	}

	if !opts.Logs && len(opts.Emitter) > 0 {
		return validate.Usage("The {0} option is only available with the {1} option.", "--emitter", "--logs")
	}

	if !opts.Logs && len(opts.Topics) > 0 {
		return validate.Usage("The {0} option is only available with the {1} option.", "--topic", "--logs")
	}

	if !opts.Statements && len(opts.Asset) > 0 {
		return validate.Usage("The {0} option is only available with the {1} option.", "--asset", "--statements")
	}

	if !opts.Traces && opts.Factory {
		return validate.Usage("The {0} option is only available with the {1} option.", "--factory", "--traces")
	}

	if len(opts.Fourbytes) > 0 && (opts.Logs || opts.Receipts || opts.Statements || opts.Appearances) {
		return validate.Usage("The {0} option is only available with the {1} option.", "--fourbyte", "no option or the --accounting")
	}

	if opts.Accounting && (opts.Appearances || opts.Logs || opts.Receipts || opts.Traces || opts.Statements || opts.Neighbors) {
		return validate.Usage("The {0} option is not available with other options.", "--accounting")
	}

	if opts.Accounting && len(opts.Addrs) != 1 {
		return validate.Usage("The {0} option is allows with only a single address.", "--accounting")
	}

	if opts.Accounting && opts.Globals.Chain != "mainnet" {
		logger.Log(logger.Warning, "The --accounting option reports a spotPrice of one for all assets on non-mainnet chains.")
	}

	if !opts.Count && len(opts.Addrs) == 0 {
		return validate.Usage("You must provide at least one Ethereum address for this command.")
	}

	if !validate.CanArticulate(opts.Articulate) {
		return validate.Usage("The {0} option requires an EtherScan API key.", "--articulate")
	}

	if opts.Globals.Format == "ofx" && !opts.Accounting {
		return validate.Usage("The {0} option is only available with the {1} option.", "--fmt ofx", "--accounting")
	}

	// Note that this does not return if the index is not initialized
	if err := index.IndexIsInitialized(opts.Globals.Chain); err != nil {
		if opts.Globals.IsApiMode() {
			return err
		} else {
			logger.Fatal(err)
		}
	}

	err := opts.Globals.Validate()
	if err != nil && strings.Contains(err.Error(), "option (ofx) must be one of") {
		// not an error
		err = nil
	}
	return err
}
