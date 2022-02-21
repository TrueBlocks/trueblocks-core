// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package explorePkg

import (
	"fmt"
	"strconv"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
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

func (opts *ExploreOptions) ValidateExplore() error {
	opts.TestLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if opts.Google && opts.Local {
		return validate.Usage("The {0} option is not available{1}.", "--local", " with the --google option")
	}

	for _, arg := range opts.Terms {
		arg = strings.ToLower(arg)

		if validate.IsValidAddress(arg) {
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
			txHash, err := id_2_TxHash(opts.Globals.Chain, arg)
			if err == nil {
				urls = append(urls, ExploreUrl{txHash, ExploreTx})
				continue
			}
			// an error here is okay since we can't distinquish between tx hashes and block hashes...
		}

		valid, _ = validate.IsValidBlockId(opts.Globals.Chain, []string{arg}, validate.ValidBlockIdWithRangeAndDate)
		if valid {
			blockHash, err := id_2_BlockHash(opts.Globals.Chain, arg)
			if err == nil {
				urls = append(urls, ExploreUrl{blockHash, ExploreBlock})
				continue
			}
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

	return opts.Globals.ValidateGlobals()
}

// id_2_TxHash takes a valid identifier (txHash/blockHash, blockHash.txId, blockNumber.txId)
// and returns the transaction hash represented by that identifier. (If it's a valid transaction.
// It may not be becuase transaction hashes and block hashes are both 32-byte hex)
func id_2_TxHash(chain, arg string) (string, error) {
	provider := config.GetRpcProvider(chain)

	// simple case first
	if !strings.Contains(arg, ".") {
		// We know it's a hash, but we want to know if it's a legitimate tx on chain
		return rpcClient.TxHashFromHash(provider, arg)
	}

	parts := strings.Split(arg, ".")
	if len(parts) != 2 {
		panic("Programmer error - valid transaction identifiers with a `.` must have two and only two parts")
	}

	if validate.IsBlockHash(parts[0]) {
		txId, err := strconv.ParseUint(parts[1], 10, 64)
		if err != nil {
			return "", nil
		}
		return rpcClient.TxHashFromHashAndId(provider, parts[0], txId)
	}

	blockNum, err := strconv.ParseUint(parts[0], 10, 64)
	if err != nil {
		return "", nil
	}
	txId, err := strconv.ParseUint(parts[1], 10, 64)
	if err != nil {
		return "", nil
	}

	return rpcClient.TxHashFromNumberAndId(provider, blockNum, txId)
}

func id_2_BlockHash(chain, arg string) (string, error) {
	provider := config.GetRpcProvider(chain)
	if validate.IsBlockHash(arg) {
		return rpcClient.BlockHashFromHash(provider, arg)
	}

	blockNum, err := strconv.ParseUint(arg, 10, 64)
	if err != nil {
		return "", nil
	}
	return rpcClient.BlockHashFromNumber(provider, blockNum)
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
