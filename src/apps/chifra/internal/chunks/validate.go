// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/migrate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config/scrape"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinning"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *ChunksOptions) validateChunks() error {
	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if len(opts.Mode) == 0 {
		return validate.Usage("Please choose at least one of {0}.", "[status|index|manifest|blooms|stats|addresses|appearances]")
	}

	err := validate.ValidateEnum("mode", opts.Mode, "[status|index|manifest|blooms|stats|addresses|appearances]")
	if err != nil {
		return err
	}

	if opts.Mode == "addresses" && opts.Globals.LogLevel > 0 && opts.Globals.Format == "json" {
		return validate.Usage("You may not use --format json and log_level > 0 in addresses mode")
	}

	if opts.Publish {
		return validate.Usage("The {0} option is not yet enabled", "--publish")
	}

	if opts.Mode != "index" && opts.Mode != "manifest" {
		if opts.Pin {
			return validate.Usage("The {0} option is available only in {1} mode.", "--pin", "index or manifest")
		}
		if opts.Publish {
			return validate.Usage("The {0} option is available only in {1} mode.", "--publish", "index or manifest")
		}
		if opts.Remote {
			return validate.Usage("The {0} option is available only in {1} mode.", "--remote", "index or manifest")
		}
		if opts.Check {
			return validate.Usage("The {0} option is available only in {1} mode.", "--check", "index or manifest")
		}
		if opts.Remote {
			return validate.Usage("The {0} option is only available in {1} mode", "--repair", "index or manifest")
		}
	}

	if opts.Repair {

		if opts.Globals.TestMode {
			return validate.Usage("The --repair option is not available in test mode")
		}

		if opts.Mode == "manifest" {
			if opts.Globals.Format != "json" {
				return validate.Usage("The {0} requires the {1} option", "manifest --repair", "--fmt json")
			}
		}
	}

	if opts.Pin {
		if opts.Remote {
			pinataKey, pinataSecret, estuaryKey := scrape.PinningKeys(opts.Globals.Chain)
			if (pinataKey == "" || pinataSecret == "") && estuaryKey == "" {
				return validate.Usage("The {0} option requires {1}", "--pin --remote", "an api key")
			}

		} else if !pinning.LocalDaemonRunning() {
			return validate.Usage("The {0} option requires {1}", "--pin", "a locally running IPFS daemon or --remote")

		}

		if opts.Mode != "index" {
			return validate.Usage("The {0} option is available only in {1} mode.", "--pin", "index")
		}
	}

	if opts.Belongs {
		if opts.Mode != "index" {
			return validate.Usage("The {0} option requires {1}", "--belongs", "the --index option")
		}
		if len(opts.Addrs) == 0 {
			return validate.Usage("You must specifiy at least one {0} with the {1} option", "address", "--belongs")
		}
		if len(opts.Blocks) == 0 {
			return validate.Usage("You must specifiy at least one {0} with the {1} option", "block identifier", "--belongs")
		}
		if opts.Globals.Verbose {
			return validate.Usage("Choose either {0} or {1}, not both.", "--verbose", "--belongs")
		}

	} else if len(opts.Addrs) > 0 {
		return validate.Usage("You may only specify an address with the --belongs option")
	}

	if opts.Mode != "index" && opts.Truncate != utils.NOPOS {
		return validate.Usage("The {0} option is only available {1}.", "--truncate", "in index mode")
	}

	if opts.Mode == "manifest" && opts.Repair {
		opts.Pin = true
	}

	if !opts.Globals.Verbose && opts.Globals.ToFile {
		return validate.Usage("You may not use the {0} option without {1}.", "--to_file", "--verbose")
	}

	err = validate.ValidateIdentifiers(
		opts.Globals.Chain,
		opts.Blocks,
		validate.ValidBlockIdWithRangeAndDate,
		1,
		&opts.BlockIds,
	)
	if err != nil {
		if invalidLiteral, ok := err.(*validate.InvalidIdentifierLiteralError); ok {
			return invalidLiteral
		}
		if errors.Is(err, validate.ErrTooManyRanges) {
			return validate.Usage("Specify only a single block range at a time.")
		}
		return err
	}

	// Note this does not return if a migration is needed
	migrate.CheckBackLevelIndex(opts.Globals.Chain, true)

	return opts.Globals.Validate()
}
