// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package monitorsPkg

import (
	"errors"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *MonitorsOptions) validateMonitors() error {
	chain := opts.Globals.Chain

	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if !config.IsChainConfigured(chain) {
		return validate.Usage("chain {0} is not properly configured.", chain)
	}

	if len(opts.Addrs) > 0 && (opts.List || opts.Count) {
		return validate.Usage("Do not provide addresses with {0} or {1}.", "--list", "--count")
	}

	if opts.List {
		// All other options are ignored

	} else {
		// Count dominates if present
		if !opts.Count {
			if opts.Watch {
				if opts.Globals.IsApiMode() {
					return validate.Usage("The {0} options is not available from the API", "--watch")
				}

				if len(opts.Globals.File) > 0 {
					return validate.Usage("The {0} option is not allowed with the {1} option. Use {2} instead.", "--file", "--watch", "--commands")
				}

				if len(opts.Commands) == 0 {
					return validate.Usage("The {0} option requires {1}.", "--watch", "a --commands file")
				} else {
					cmdFile, err := filepath.Abs(opts.Commands)
					if err != nil || !file.FileExists(cmdFile) {
						return validate.Usage("The {0} option requires {1} to exist.", "--watch", opts.Commands)
					}
					if file.FileSize(cmdFile) == 0 {
						logger.Fatal(validate.Usage("The file you specified ({0}) was found but contained no commands.", cmdFile).Error())
					}
				}

				if len(opts.Watchlist) == 0 {
					return validate.Usage("The {0} option requires {1}.", "--watch", "a --watchlist file")
				} else {
					if opts.Watchlist != "existing" {
						watchList, err := filepath.Abs(opts.Watchlist)
						if err != nil || !file.FileExists(watchList) {
							return validate.Usage("The {0} option requires {1} to exist.", "--watch", opts.Watchlist)
						}
						if file.FileSize(watchList) == 0 {
							logger.Fatal(validate.Usage("The file you specified ({0}) was found but contained no addresses.", watchList).Error())
						}
					}
				}

				if err := index.IsInitialized(chain, config.ExpectedVersion()); err != nil {
					if (errors.Is(err, index.ErrNotInitialized) || errors.Is(err, index.ErrIncorrectHash)) && !opts.Globals.IsApiMode() {
						logger.Fatal(err)
					}
					return err
				}

				if opts.BatchSize < 1 {
					return validate.Usage("The {0} option must be greater than zero.", "--batch_size")
				}
			} else {
				if opts.BatchSize != 8 {
					return validate.Usage("The {0} option is not available{1}.", "--batch_size", " without --watch")
				} else {
					opts.BatchSize = 0
				}

				if opts.RunCount > 0 {
					return validate.Usage("The {0} option is not available{1}.", "--run_count", " without --watch")
				}

				if opts.Sleep != 14 {
					return validate.Usage("The {0} option is not available{1}.", "--sleep", " without --watch")
				}

				// We validate some of the simpler curd commands here and the rest in HandleCrud
				if opts.Undelete {
					if opts.Delete || opts.Remove {
						return validate.Usage("The --undelete option may not be used with --delete or --remove.")
					}
				}

				if !opts.Clean && len(opts.Addrs) == 0 {
					return validate.Usage("You must provide at least one Ethereum address for this command.")
				}

				if !opts.Clean && !opts.Delete && !opts.Undelete && !opts.Remove && !opts.Globals.Decache {
					return validate.Usage("Please provide either --clean or one of the CRUD commands.")
				}

				if !opts.Globals.IsApiMode() && !opts.Clean {
					if len(opts.Globals.File) > 0 {
						// Do nothing
					} else {
						err := validate.ValidateAtLeastOneNonSentinal(opts.Addrs)
						if err != nil {
							return err
						}
					}
				}
			}
		}
	}

	return opts.Globals.Validate()
}
