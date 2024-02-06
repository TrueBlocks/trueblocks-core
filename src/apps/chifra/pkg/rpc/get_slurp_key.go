package rpc

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (conn *Connection) getTxsByAddressKey(chain, addr string, paginator *Paginator) ([]types.SimpleSlurp, int, error) {
	return []types.SimpleSlurp{}, 0, nil
}
