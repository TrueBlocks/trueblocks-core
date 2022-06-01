// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package statePkg

import (
	"errors"
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *StateOptions) ValidateState() error {
	opts.TestLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	for _, part := range opts.Parts {
		err := validate.ValidateEnum("--parts", part, "[none|some|all|balance|nonce|code|storage|deployed|accttype]")
		if err != nil {
			return err
		}
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

			for _, addr := range opts.Addrs {
				if validate.IsValidAddress(addr) {
					return validate.Usage("The {0} option is not available{1}.", "--call", " when an address is present")
				}
			}

			parts := strings.Split(opts.Call, "!")
			contract := parts[0]
			if len(opts.ProxyFor) > 0 {
				contract = opts.ProxyFor
			}
			if !validate.IsSmartContract(contract) {
				return validate.Usage("The first argument for the --call option must be a smart contract.")
			}

			if len(parts) < 2 {
				// TODO: Remove this and present ABI in non-test mode
				// non-test mode on the terminal does something we want to preserve in the C++ code -- it
				// presents the abi for this contract. We can do that in Go, so we only fail during testing
				fmt.Println(opts.Call, len(parts))
				if opts.Globals.TestMode || opts.Globals.ApiMode {
					return validate.Usage("You must provide either a four-byte code or a function signature for the smart contract.")
				}
			}

			// command is either a fourbyte or a function signature
			command := parts[1]
			if !validate.IsValidFourByte(command) {
				if !strings.Contains(command, "(") || !strings.Contains(command, ")") {
					return validate.Usage("The provided value ({0}) must be either a four-byte nor a function signature.", command)
				}
			}

		} else {
			if len(opts.ProxyFor) > 0 {
				return validate.Usage("The {0} option is only available with the {1} option.", "--proxy_for", "--call")
			}

			err := validate.ValidateAtLeastOneAddr(opts.Addrs)
			if err != nil {
				return err
			}

		}
	}

	// Blocks are available for both commands
	if len(opts.Blocks) > 0 {
		err := validate.ValidateIdentifiers(
			opts.Globals.Chain,
			opts.Blocks,
			validate.ValidBlockIdWithRange,
			1,
			nil,
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
	}

	return opts.Globals.ValidateGlobals()
}
