package provider

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
)

type Query struct {
	Chain      string
	Addresses  []base.Address
	BlockRange []identifiers.Identifier
	// Etherscan calls them "modules"
	Resources []string
}

func (q *Query) InRange(bn base.Blknum) (bool, error) {
	if len(q.BlockRange) == 0 {
		return true, nil
	}

	br := q.BlockRange[0]
	if strings.Contains(br.Orig, "-") && !strings.Contains(br.Orig, ":") {
		return br.Start.Number <= uint(bn) && uint(bn) <= br.End.Number, nil
	}

	if blockNums, err := br.ResolveBlocks(q.Chain); err != nil {
		return false, err
	} else {
		for _, num := range blockNums {
			if num == bn {
				return true, nil
			}
		}
		return false, nil
	}
}

func (q *Query) Dup() *Query {
	return &Query{
		Chain:      q.Chain,
		Addresses:  q.Addresses,
		BlockRange: q.BlockRange,
		Resources:  q.Resources,
	}
}
