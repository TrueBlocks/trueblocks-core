// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package explorePkg

import (
	"errors"
	"fmt"
	"strings"

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
			continue
		}

		if opts.Google {
			return validate.Usage("The {0} option requires {1}.", "--google", "an address term")
		}

		valid, _ := validate.IsValidTransId([]string{arg}, validate.ValidTransId)
		if valid {
			txHash, err := id_2_TxHash(arg)
			if err == nil {
				urls = append(urls, ExploreUrl{txHash, ExploreTx})
				continue
			}
		}

		valid, _ = validate.IsValidBlockId([]string{arg}, validate.ValidBlockId)
		if valid {
			// TODO: The block number needs to be resolved (for example from a hash)
			// TODO: or a special block
			blockHash, err := id_2_BlockHash(arg)
			if err == nil {
				urls = append(urls, ExploreUrl{blockHash, ExploreBlock})
				continue
			}
		}

		if validate.IsValidFourByte(arg) {
			urls = append(urls, ExploreUrl{arg, ExploreFourByte})
			continue
		}

		return validate.Usage("The {0} option ({1}) must {2}.", "term", arg, "be a valid term")
	}

	if len(urls) == 0 {
		urls = append(urls, ExploreUrl{"", ExploreNone})
	}

	return opts.Globals.ValidateGlobals()
}

func id_2_TxHash(arg string) (string, error) {
	if strings.Contains(arg, ".") {
		parts := strings.Split(arg, ".")
		// we've already checked validity so we can assert(len(parts) == 2)
		if validate.IsBlockHash(parts[0]) {
			// call eth_getTransactionByBlockHashAndIndex
			return "0x5c504ed432cb51138bcf09aa5e8a410dd4a1e204ef84bfed1be16dfba1b22060", nil
		}
		// call eth_getTransactionByBlockNumberAndIndex
		return "0x5c504ed432cb51138bcf09aa5e8a410dd4a1e204ef84bfed1be16dfba1b22060", nil
	}
	// call eth_getTransactionByHash
	if arg == "0x5c504ed432cb51138bcf09aa5e8a410dd4a1e204ef84bfed1be16dfba1b22060" {
		return arg, nil
	}
	return "", errors.New("not a transaction")
}

func id_2_BlockHash(arg string) (string, error) {
	if arg != "0x5c504ed432cb51138bcf09aa5e8a410dd4a1e204ef84bfed1be16dfba1b22060" {
		return arg, nil
	}
	return "", errors.New("not a block hash")
}

// TODO: Turn off OPT_FMT OPT_VERBOSE
// TODO: Read base URLs from config file
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

func (u *ExploreUrl) getUrl(opts *ExploreOptions) string {
	if opts.Google {
		return "https://www.google.com/search?q=" + u.term + "+-etherscan+-etherchain+-bloxy+-bitquery+-ethplorer+-tokenview+-anyblocks+-explorer"
	}

	if u.termType == ExploreFourByte {
		return "https://www.4byte.directory/signatures/?bytes4_signature=" + u.term
	}

	if u.termType == ExploreEnsName {
		return "https://etherscan.io/enslookup-search?search=" + u.term
	}

	url := "https://etherscan.io/"
	query := ""
	switch u.termType {
	case ExploreNone:
		return url
	case ExploreTx:
		query = "tx/" + u.term
	case ExploreBlock:
		query = "block/" + u.term
	case ExploreName:
		// TODO: we must resolve the name if possible or fail
		fallthrough
	case ExploreAddress:
		fallthrough
	default:
		query = "address/" + u.term
	}

	if opts.Local {
		url = "http://localhost:1234/"
		query = strings.Replace(query, "tx/", "explorer/transactions/", -1)
		query = strings.Replace(query, "block/", "explorer/blocks/", -1)
		query = strings.Replace(query, "address/", "dashboard/accounts?address=", -1)
	}

	return url + query
}
