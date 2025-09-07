// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package explorePkg

import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/ethereum/go-ethereum"
)

func (opts *ExploreOptions) validateExplore() error {
	chain := opts.Globals.Chain

	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if !config.IsChainConfigured(chain) {
		return validate.Usage("chain {0} is not properly configured.", chain)
	}

	if opts.Globals.IsApiMode() {
		return validate.Usage("The {0} option is not available{1}.", "explore", " in api mode")
	}

	if opts.Google && opts.Local {
		return validate.Usage("The {0} option is not available{1}.", "--local", " with the --google option")
	}

	if opts.Dalle && opts.Local {
		return validate.Usage("The {0} option is not available{1}.", "--local", " with the --dalle option")
	}

	if len(opts.Terms) == 0 && (opts.Dalle || opts.Google) {
		return validate.Usage("The {0} options require {1}.", "--dalle and --google", "an address term")
	}

	for _, arg := range opts.Terms {
		arg = strings.ToLower(arg)

		if base.IsValidAddress(arg) {
			if strings.Contains(arg, ".eth") {
				opts.Destinations = append(opts.Destinations, types.NewDestination(arg, types.DestinationEnsName))
			} else {
				opts.Destinations = append(opts.Destinations, types.NewDestination(arg, types.DestinationAddress))
			}
			// We got a valid address, we're done checking
			continue
		}

		// The argument is not an address, so we can't use --google
		if opts.Google || opts.Dalle {
			continue
		}

		valid, _ := validate.IsValidTransId(chain, []string{arg}, validate.ValidTransId)
		if valid {
			txHash, err := opts.idToTxHash(arg, validate.IsBlockHash)
			if err == nil {
				opts.Destinations = append(opts.Destinations, types.NewDestination(txHash, types.DestinationTx))
				continue
			}
			// an error here is okay since we can't distinguish between tx hashes and block hashes...
		}

		valid, _ = validate.IsValidBlockId(chain, []string{arg}, validate.ValidBlockIdWithRangeAndDate)
		if valid {
			blockHash, err := opts.idToBlockHash(chain, arg, validate.IsBlockHash)
			if err == nil {
				opts.Destinations = append(opts.Destinations, types.NewDestination(blockHash.Hex(), types.DestinationBlock))
				continue
			}
			// An error here is not okay because we have a valid hash but it's not a valid on-chain
			// thingy, so we must have been told why by the node
			return fmt.Errorf("block at %s returned an error: %w", arg, ethereum.NotFound)
		}

		if validate.IsValidFourByte(arg) {
			opts.Destinations = append(opts.Destinations, types.NewDestination(arg, types.DestinationFourByte))
			continue
		}

		return validate.Usage("The {0} option ({1}) {2}.", "term", arg, "is not valid")
	}

	if len(opts.Destinations) == 0 {
		if opts.Google || opts.Dalle {
			return validate.Usage("The {0} options require {1}.", "--dalle and --google", "an address term")
		}
		opts.Destinations = append(opts.Destinations, types.NewDestination("", types.DestinationNone))
	}

	return opts.Globals.Validate()
}

func (opts *ExploreOptions) idToBlockHash(chain, arg string, isBlockHash func(arg string) bool) (base.Hash, error) {
	_ = chain // linter
	if isBlockHash(arg) {
		return opts.Conn.GetBlockHashByHash(arg)
	}
	return opts.Conn.GetBlockHashByNumber(base.MustParseBlknum(arg))
}

// idToTxHash takes a valid identifier (txHash/blockHash, blockHash.txId, blockNumber.txId)
// and returns the transaction hash represented by that identifier. (If it's a valid transaction.
// It may not be because transaction hashes and block hashes are both 32-byte hex)
func (opts *ExploreOptions) idToTxHash(arg string, isBlockHash func(arg string) bool) (string, error) {
	// simple case first
	if !strings.Contains(arg, ".") {
		// We know it's a hash, but we want to know if it's a legitimate tx on chain
		return opts.Conn.GetTransactionHashByHash(arg)
	}

	parts := strings.Split(arg, ".")
	if len(parts) != 2 {
		logger.ShouldNotHappen("valid transaction identifiers with a `.` must have two and only two parts")
	}

	if isBlockHash(parts[0]) {
		return opts.Conn.GetTransactionHashByHashAndID(parts[0], base.MustParseTxnum(parts[1]))
	}

	blockNum := base.MustParseBlknum(parts[0])
	txId := base.MustParseTxnum(parts[1])
	hash, err := opts.Conn.GetTransactionHashByNumberAndID(blockNum, base.Txnum(txId))
	return hash.Hex(), err
}
