package rpc

import (
	"fmt"
	"strconv"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type Paginator struct {
	Page    int
	PerPage int
}

func (conn *Connection) SlurpTxsByAddress(chain, source, addr, requestType string, paginator *Paginator) ([]types.SimpleSlurp, int, error) {
	switch source {
	case "key":
		return conn.getTxsByAddressKey(chain, addr, paginator)
	case "etherscan":
		fallthrough
	default:
		return conn.getTxsByAddressEs(chain, addr, requestType, paginator)
	}
}

func mustParseInt(input any) (result int64) {
	result, _ = strconv.ParseInt(fmt.Sprint(input), 0, 64)
	return
}

var ss = strings.Repeat(" ", 40)
