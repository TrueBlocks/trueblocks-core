package rpc

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type Paginator struct {
	Page    int
	PerPage int
}

func (conn *Connection) SlurpTxsByAddress(chain, source, addr, requestType string, paginator *Paginator) ([]types.SimpleSlurp, int, error) {
	switch source {
	case "key":
		return []types.SimpleSlurp{}, 0, nil
	case "etherscan":
		fallthrough
	default:
		return conn.getTxsByAddressEs(chain, addr, requestType, paginator)
	}
}

func (conn *Connection) SlurpTxCountByAddress(chain, source, addr, requestType string, paginator *Paginator) (int, error) {
	switch source {
	case "key":
		return 0, nil
	case "etherscan":
		fallthrough
	default:
		return conn.getTxCountByAddressEs(chain, addr, requestType, paginator)
	}
}
