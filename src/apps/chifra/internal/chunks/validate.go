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
		return validate.Usage("Please choose at least one of {0}.", "[status|manifest|index|blooms|addresses|appearances|stats]")
	}

	err := validate.ValidateEnum("mode", opts.Mode, "[status|manifest|index|blooms|addresses|appearances|stats]")
	if err != nil {
		return err
	}

	if opts.Publish {
		return validate.Usage("The {0} option is not yet enabled", "--publish")
	}

	if opts.Mode != "index" && opts.Mode != "manifest" {
		if opts.Pin {
			return validate.Usage("The {0} option is available only in {1} mode.", "--pin", "index or manifest")
		}
		if opts.Remote {
			return validate.Usage("The {0} option is available only in {1} mode.", "--remote", "index or manifest")
		}
		if opts.Check {
			return validate.Usage("The {0} option is available only in {1} mode.", "--check", "index or manifest")
		}
		if opts.Repair {
			return validate.Usage("The {0} option is only available in {1} mode", "--repair", "index or manifest")
		}

	} else {
		if opts.Pin {
			if opts.Remote {
				pinataKey, pinataSecret, estuaryKey := scrape.PinningKeys(opts.Globals.Chain)
				if (pinataKey == "" || pinataSecret == "") && estuaryKey == "" {
					return validate.Usage("The {0} option requires {1}.", "--pin --remote", "an api key")
				}

			} else if !pinning.LocalDaemonRunning() {
				return validate.Usage("The {0} option requires {1}.", "--pin", "a locally running IPFS daemon or --remote")

			}
		}

		if opts.Repair {
			if opts.Mode == "manifest" {
				if opts.Globals.Format != "json" {
					return validate.Usage("The {0} option requires {1}.", "manifest --repair", "--fmt json")
				}
			} else { // mode == "index"
				if len(opts.Blocks) == 0 {
					return validate.Usage("The {0} option requires {1}.", "index --repair", "at least one block identifier")
				}
			}
		}
	}

	if opts.Mode != "manifest" {
		if opts.Publish {
			return validate.Usage("The {0} option is available only in {1} mode.", "--publish", "index or manifest")
		}
	}

	if opts.Mode != "index" {
		if opts.Truncate != utils.NOPOS {
			return validate.Usage("The {0} option is only available {1}.", "--truncate", "in index mode")
		}
		if len(opts.Belongs) > 0 {
			return validate.Usage("The {0} option requires {1}.", "--belongs", "the index mode")
		}
	} else {
		if len(opts.Belongs) > 0 {
			if opts.Belongs[0] == "" {
				return validate.Usage("The {0} option requires {1}.", "--belongs", "an address")
			}
			if opts.Globals.Verbose {
				return validate.Usage("Choose either {0} or {1}, not both.", "--verbose", "--belongs")
			}
			if len(opts.Blocks) == 0 {
				return validate.Usage("You must specifiy at least one {0} with the {1} option", "block identifier", "--belongs")
			}
		}
	}

	if opts.Globals.TestMode {
		if opts.Repair {
			return validate.Usage("The --repair option is not available in test mode")
		}
		if opts.Pin {
			return validate.Usage("The --pin option is not available in test mode")
		}
		if opts.Publish {
			return validate.Usage("The --publish option is not available in test mode")
		}
		if opts.Truncate != utils.NOPOS {
			return validate.Usage("The --truncate option is not available in test mode")
		}
	}

	if opts.Globals.Verbose || opts.Globals.LogLevel > 0 {
		if opts.Globals.ToFile {
			return validate.Usage("You may not use the {0} option without {1}.", "--to_file", "--verbose")
		}
		if opts.Mode == "addresses" && opts.Globals.Format == "json" {
			return validate.Usage("Do not use {0} with {1}", "--format json", "--verbose in the addresses mode")
		}
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
