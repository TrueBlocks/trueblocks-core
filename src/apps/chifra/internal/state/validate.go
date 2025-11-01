// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package statePkg

import (
	"errors"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/call"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/validate"
)

func (opts *StateOptions) validateState() error {
	chain := opts.Globals.Chain

	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if !config.IsChainConfigured(chain) {
		return validate.Usage("chain {0} is not properly configured.", chain)
	}

	err := validate.ValidateEnumSlice("--parts", opts.Parts, "[balance|nonce|code|proxy|deployed|accttype|some|all]")
	if err != nil {
		return err
	}

	if len(opts.Globals.File) > 0 {
		// do nothing for now

	} else {
		if opts.Call && opts.Send {
			return validate.Usage("The {0} and {1} options are mutually exclusive.", "--call", "--send")
		}

		if opts.Call || opts.Send {
			if len(opts.Calldata) == 0 {
				return validate.Usage("The {0} option requires at least one {1} command.", "--call", "--calldata")
			}

			if len(opts.Parts) > 0 {
				return validate.Usage("The {0} option is not available{1}.", "--parts", " with the --call or --send option")
			}

			if opts.Changes {
				return validate.Usage("The {0} option is not available{1}.", "--changes", " with the --call or --send option")
			}

			if opts.NoZero {
				return validate.Usage("The {0} option is not available{1}.", "--no_zero", " with the --call or --send option")
			}

			if len(opts.Addrs) != 1 {
				return validate.Usage("Exactly one address is required for the {0} option.", "--call or --send")
			}

			callAddress := opts.GetAddressOrProxy()
			isContract := false
			if err = opts.Conn.IsContractAtLatest(base.HexToAddress(callAddress.Hex())); err == nil {
				isContract = true
			}

			if opts.Send {
				// Sends with no call data are okay to either smart contracts or regular accounts.
				if isContract {
					if err := opts.checkCommands(callAddress, opts.Send); err != nil {
						return err
					}
				}

			} else if opts.Call {
				if isContract {
					if err := opts.checkCommands(callAddress, opts.Send); err != nil {
						return err
					}
				} else {
					if errors.Is(err, rpc.ErrNotAContract) {
						return validate.Usage("The address for the --call option must be a smart contract.")
					}
					return err
				}
			}

		} else {
			if len(opts.Calldata) > 0 {
				return validate.Usage("The {0} option is only available with the {1} option.", "--calldata", "--call or --send")
			}

			if opts.Articulate {
				return validate.Usage("The {0} option is only available with the {1} option.", "--articulate", "--call or --send")
			}

			proxy := opts.ProxyForAddr
			if !proxy.IsZero() {
				return validate.Usage("The {0} option is only available with the {1} option.", "--proxy_for", "--call or --send")
			}

			err := validate.ValidateAtLeastOneAddr(opts.Addrs)
			if err != nil {
				return err
			}

			err = validate.ValidateAddresses(opts.Addrs)
			if err != nil {
				return err
			}
		}
	}

	// Blocks are optional, but if they are present, they must be valid
	if len(opts.Blocks) > 0 {
		bounds, err := validate.ValidateIdentifiersWithBounds(
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

		latest := opts.Conn.GetLatestBlockNumber()
		if bounds.First < (latest-250) && !opts.Conn.IsNodeArchive() {
			return validate.Usage("The {0} requires {1}.", "query for historical state", "an archive node")
		}
	}

	return opts.Globals.Validate()
}

func (opts *StateOptions) checkCommands(callAddress base.Address, isSend bool) error {
	if len(opts.Calldata) == 0 && !isSend {
		return validate.Usage("The {0} option requires at least one command.", "--call")
	}

	// an empty list of commands is okay...
	for _, cmd := range opts.Calls {
		if _, suggestions, err := call.NewContractCall(opts.Conn, callAddress, cmd); err != nil {
			message := fmt.Sprintf("the --calldata value provided (%s) was not found: %s", cmd, err)
			if len(suggestions) > 0 {
				message += " Suggestions: "
				for index, suggestion := range suggestions {
					if index > 0 {
						message += " "
					}
					message += fmt.Sprintf("%d: %s.", index+1, suggestion)
				}
			}
			return errors.New(message)
		}
	}
	return nil
}
