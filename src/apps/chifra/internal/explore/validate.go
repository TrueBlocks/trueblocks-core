// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package explorePkg

import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/ethereum/go-ethereum"
)

type ExploreType uint8

const (
	ExploreNone ExploreType = iota
	ExploreAddress
	ExploreName
	ExploreEnsName
	ExploreTx
	ExploreBlock
	ExploreFourByte
)

type ExploreUrl struct {
	term     string
	termType ExploreType
}

var urls []ExploreUrl

func (opts *ExploreOptions) validateExplore() error {
	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if opts.Globals.Cache {
		return validate.Usage("The {0} option is not available for this command.", "--cache")
	}

	if opts.Google && opts.Local {
		return validate.Usage("The {0} option is not available{1}.", "--local", " with the --google option")
	}

	for _, arg := range opts.Terms {
		arg = strings.ToLower(arg)

		if base.IsValidAddress(arg) {
			if strings.Contains(arg, ".eth") {
				urls = append(urls, ExploreUrl{arg, ExploreEnsName})
			} else {
				urls = append(urls, ExploreUrl{arg, ExploreAddress})
			}
			// We got a valid address, we're done checking
			continue
		}

		// The argument is not an address, so we can't use --google
		if opts.Google {
			return validate.Usage("The {0} option requires {1}.", "--google", "an address term")
		}

		valid, _ := validate.IsValidTransId(opts.Globals.Chain, []string{arg}, validate.ValidTransId)
		if valid {
			txHash, err := rpcClient.Id_2_TxHash(opts.Globals.Chain, arg, validate.IsBlockHash)
			if err == nil {
				urls = append(urls, ExploreUrl{txHash, ExploreTx})
				continue
			}
			// an error here is okay since we can't distinquish between tx hashes and block hashes...
		}

		valid, _ = validate.IsValidBlockId(opts.Globals.Chain, []string{arg}, validate.ValidBlockIdWithRangeAndDate)
		if valid {
			blockHash, err := rpcClient.Id_2_BlockHash(opts.Globals.Chain, arg, validate.IsBlockHash)
			if err == nil {
				urls = append(urls, ExploreUrl{blockHash, ExploreBlock})
				continue
			}
			// An error here is not okay because we have a valid hash but it's not a valid on-chain
			// thingy, so we must have been told why by the node
			return fmt.Errorf("block at %s returned an error: %w", arg, ethereum.NotFound)
		}

		if validate.IsValidFourByte(arg) {
			urls = append(urls, ExploreUrl{arg, ExploreFourByte})
			continue
		}

		return validate.Usage("The {0} option ({1}) {2}.", "term", arg, "is not valid")
	}

	if len(urls) == 0 {
		urls = append(urls, ExploreUrl{"", ExploreNone})
	}

	return opts.Globals.Validate()
}

func (t ExploreType) String() string {
	switch t {
	case ExploreNone:
		return "ExploreNone"
	case ExploreAddress:
		return "ExploreAddress"
	case ExploreName:
		return "ExploreName"
	case ExploreEnsName:
		return "ExploreEnsName"
	case ExploreTx:
		return "ExploreTx"
	case ExploreBlock:
		return "ExploreBlock"
	case ExploreFourByte:
		return "ExploreFourByte"
	default:
		return fmt.Sprintf("%d", t)
	}
}
