// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package exportPkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/node"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *ExportOptions) validateExport() error {
	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if opts.Neighbors {
		return validate.Usage("The {0} option is currenlty disabled.", "--neighbors")
	}

	if len(opts.Globals.File) == 0 {
		if err := validate.ValidateAtLeastOneAddr(opts.Addrs); err != nil {
			return err
		}
	}

	if opts.Globals.TestMode && opts.Unripe {
		return validate.Usage("--unripe is disabled for testing.")
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

	if opts.Count {
		if opts.Logs || opts.Receipts || opts.Traces || opts.Neighbors {
			return validate.Usage("The {0} option is only available with transactional options.", "--count")
		}
		if opts.MaxRecords > 0 && opts.MaxRecords != 250 {
			return validate.Usage("The {0} option is not available with the {1} option.", "--count", "--max_records")
		}
	}

	if !opts.Logs && len(opts.Emitter) > 0 {
		return validate.Usage("The {0} option is only available with the {1} option.", "--emitter", "--logs")
	}

	if !opts.Logs && len(opts.Topics) > 0 {
		return validate.Usage("The {0} option is only available with the {1} option.", "--topic", "--logs")
	}

	if !opts.Traces && opts.Factory {
		return validate.Usage("The {0} option is only available with the {1} option.", "--factory", "--traces")
	}

	if len(opts.Fourbytes) > 0 && (opts.Logs || opts.Receipts || opts.Appearances) {
		return validate.Usage("The {0} option is only available with the {1} option.", "--fourbyte", "no option or the --accounting")
	}

	if opts.Accounting {
		if len(opts.Addrs) != 1 {
			return validate.Usage("The {0} option is allows with only a single address.", "--accounting")
		}

		if opts.Appearances || opts.Logs || opts.Receipts || opts.Traces || opts.Neighbors {
			return validate.Usage("The {0} option is not available with other options.", "--accounting")
		}

		if opts.Globals.Chain != "mainnet" {
			logger.Warn("The --accounting option reports a spotPrice of one for all assets on non-mainnet chains.")
		}

		if opts.Statements {
			if len(opts.Flow) > 0 {
				if err := validate.ValidateEnum("--flow", opts.Flow, "[in|out|zero]"); err != nil {
					return err
				}
			}

		} else {
			if len(opts.Flow) > 0 {
				return validate.Usage("The {0} option is only available with {1} option.", "--flow", "--statements")
			}

			if len(opts.Asset) > 0 {
				return validate.Usage("The {0} option is only available with the {1} option.", "--asset", "--statements")
			}
		}

		if !node.IsArchiveNode(opts.Globals.Chain) {
			return validate.Usage("The {0} option requires {1}.", "--accounting", "an archive node")
		}

	} else {
		if opts.Statements {
			return validate.Usage("The {0} option is only available with the {1} option.", "--statements", "--accounting")
		}

		if opts.Globals.Format == "ofx" {
			return validate.Usage("The {0} option is only available with the {1} option.", "--fmt ofx", "--accounting")
		}
	}

	if !validate.CanArticulate(opts.Articulate) {
		return validate.Usage("The {0} option requires an Etherscan API key.", "--articulate")
	}

	// Note that this does not return if the index is not initialized
	if err := index.IndexIsInitialized(opts.Globals.Chain); err != nil {
		if opts.Globals.IsApiMode() {
			return err
		} else {
			logger.Fatal(err)
		}
	}

	return opts.Globals.Validate()
	// if err != nil && strings.Contains(err.Error(), "option (ofx) must be one of") {
	// 	// not an error
	// 	err = nil
	// }
	// return err
}
