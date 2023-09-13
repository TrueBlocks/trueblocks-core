// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"errors"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinning"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *ChunksOptions) validateChunks() error {
	chain := opts.Globals.Chain

	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if len(opts.Mode) == 0 {
		return validate.Usage("Please choose at least one of {0}.", "[manifest|index|blooms|addresses|appearances|stats]")
	}

	err := validate.ValidateEnum("mode", opts.Mode, "[manifest|index|blooms|addresses|appearances|stats]")
	if err != nil {
		return err
	}

	isIndexOrManifest := opts.Mode == "index" || opts.Mode == "manifest"
	if !isIndexOrManifest {
		if err = opts.isDisallowed(!isIndexOrManifest /* i.e., true */, opts.Mode); err != nil {
			return err
		}

		if opts.Check {
			return validate.Usage("The {0} option is not available in {1} mode.", "--check", opts.Mode)
		}
	}

	if opts.Mode == "manifest" {
		if opts.Pin {
			if opts.Remote {
				pinataKey, pinataSecret, estuaryKey := config.GetPinningKeys(chain)
				if (pinataKey == "" || pinataSecret == "") && estuaryKey == "" {
					return validate.Usage("The {0} option requires {1}.", "--pin --remote", "an api key")
				}

			} else if !pinning.LocalDaemonRunning() {
				return validate.Usage("The {0} option requires {1}.", "--pin", "a locally running IPFS daemon or --remote")

			}
		}
	} else {
		if opts.Publish {
			return validate.Usage("The {0} option is available only in {1} mode.", "--publish", "index or manifest")
		}
	}

	if opts.Pin && opts.Publish {
		return validate.Usage("The {0} and {1} options are mutually exclusive.", "--pin", "--publish")
	}

	if opts.Deep {
		if opts.Mode == "index" {
			// do nothing
		} else if opts.Mode == "manifest" {
			if !opts.Remote && !pinning.LocalDaemonRunning() {
				return validate.Usage("The {0} option requires {1}.", "manifest --deep", "a locally running IPFS daemon or --remote")
			}
		} else {
			return validate.Usage("The {0} option requires mode {1}.", "--deep", "index or manifest")
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
			err := validate.ValidateAtLeastOneAddr(opts.Belongs)
			if err != nil {
				return err
			}
			if opts.Globals.Verbose {
				return validate.Usage("Choose either {0} or {1}, not both.", "--verbose", "--belongs")
			}
			if len(opts.Blocks) == 0 {
				return validate.Usage("You must specify at least one {0} with the {1} option", "block identifier", "--belongs")
			}
			if opts.Globals.Format != "json" {
				return validate.Usage("The {0} option only works with {1}", "--belongs", "--fmt json")
			}
		}
	}

	if err = opts.isDisallowed(opts.Globals.IsApiMode(), "API"); err != nil {
		return err
	}

	if err = opts.isDisallowed(opts.Globals.TestMode, "test"); err != nil {
		return err
	}

	err = validate.ValidateIdentifiers(
		chain,
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

	if opts.FirstBlock != 0 || opts.LastBlock != utils.NOPOS || opts.MaxAddrs != utils.NOPOS {
		if opts.FirstBlock >= opts.LastBlock {
			msg := fmt.Sprintf("first_block (%d) must be strictly earlier than last_block (%d).", opts.FirstBlock, opts.LastBlock)
			return validate.Usage(msg)
		}
		if len(opts.Belongs) == 0 && opts.Mode != "addresses" && opts.Mode != "appearances" {
			return validate.Usage("some options are only available with {1}.", "the addresses, the appearances, or the index --belongs modes")
		}
		// TODO: We should check that the first and last blocks are inside the ranges implied by the block ids
		// if len(opts.BlockIds) > 0 {
		// }
	}

	// Note that this does not return if the index is not initialized
	if err := index.IndexIsInitialized(chain); err != nil {
		if opts.Globals.IsApiMode() {
			return err
		} else {
			logger.Fatal(err)
		}
	}

	return opts.Globals.Validate()
}

func (opts *ChunksOptions) isDisallowed(test bool, mode string) error {
	if test {
		if opts.Pin {
			return validate.Usage("The {0} option is not available in {1} mode.", "--pin", mode)
		}
		if opts.Publish {
			return validate.Usage("The {0} option is not available in {1} mode.", "--publish", mode)
		}
		if opts.Remote {
			return validate.Usage("The {0} option is not available in {1} mode.", "--remote", mode)
		}
		if opts.Truncate != utils.NOPOS {
			return validate.Usage("The {0} option is not available in {1} mode.", "--truncate", mode)
		}
	}
	return nil
}
