// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package statePkg

import (
	"errors"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/node"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *StateOptions) validateState() error {
	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	err := validate.ValidateEnumSlice("--parts", opts.Parts, "[none|some|all|balance|nonce|code|proxy|deployed|accttype]")
	if err != nil {
		return err
	}

	if strings.Contains(strings.Join(opts.Parts, " "), "nonce") {
		return validate.Usage("The {0} value is currently not available{1}.", "nonce", " with the --parts option")
	}

	if len(opts.Globals.File) > 0 {
		// do nothing for now

	} else {
		if len(opts.Call) > 0 {
			if len(opts.Parts) > 0 {
				return validate.Usage("The {0} option is not available{1}.", "--parts", " with the --call option")
			}

			if opts.Changes {
				return validate.Usage("The {0} option is not available{1}.", "--changes", " with the --call option")
			}

			if opts.NoZero {
				return validate.Usage("The {0} option is not available{1}.", "--no_zero", " with the --call option")
			}

			// for _, addr := range opts.Addrs {
			// 	if validate.IsValidAddress(addr) {
			// 		return validate.Usage("The {0} option is not available{1}.", "--call", " when an address is present")
			// 	}
			// }

			if len(opts.Addrs) > 1 {
				return validate.Usage("only one address allowed")
			}

			// parts := strings.Split(opts.Call, "!")
			contract := opts.Addrs[0]
			if len(opts.ProxyFor) > 0 {
				contract = opts.ProxyFor
			}

			ok, err := validate.IsSmartContract(opts.Globals.Chain, contract)
			if err != nil {
				return err
			}
			if !ok {
				return validate.Usage("address for the --call option must be a smart contract.")
			}

			// if len(parts) < 2 {
			// 	// TODO: Remove this and present ABI in non-test mode
			// 	// non-test mode on the terminal does something we want to preserve in the C++ code -- it
			// 	// presents the abi for this contract. We can do that in Go, so we only fail during testing
			// 	if opts.Globals.TestMode || opts.Globals.IsApiMode() {
			// 		return validate.Usage("You must provide either a four-byte code or a function signature for the smart contract.")
			// 	}
			// } else {
			// 	// command is either a fourbyte or a function signature
			// 	command := parts[1]
			// 	if !validate.IsValidFourByte(command) {
			// 		if !strings.Contains(command, "(") || !strings.Contains(command, ")") {
			// 			return validate.Usage("The provided value ({0}) must be either a four-byte nor a function signature.", command)
			// 		}
			// 	}
			// }

		} else {
			if len(opts.ProxyFor) > 0 {
				return validate.Usage("The {0} option is only available with the {1} option.", "--proxy_for", "--call")
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

		latest := rpcClient.BlockNumber(config.GetRpcProvider(opts.Globals.Chain))
		// TODO: Should be configurable
		if bounds.First < (latest-250) && !node.IsArchiveNode(opts.Globals.Chain) {
			return validate.Usage("The {0} requires {1}.", "query for historical state", "an archive node")
		}
	}

	return opts.Globals.Validate()
}
