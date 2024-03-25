package rpc

import (
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/prefunds"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// TODO: Some of this code may be chain-specific - for example,
// some chains have no pre-allocation.

// IsNodeArchive tries to return true if the node is an archive
// node with the following caveteat: we assume that the node has
// been initialized with a pre-allocation. This is a reasonable
// assumption for most chains, but not all. For example, if a
// chain does not have a pre-allocation, this function will return
// false, when in fact the node may be an archive node.
func (conn *Connection) IsNodeArchive() bool {
	thePath := filepath.Join(config.MustGetPathToChainConfig(conn.Chain), "allocs.csv")
	largest, err := prefunds.GetLargestPrefund(conn.Chain, thePath)
	if err != nil {
		return false
	}

	bal, err := conn.GetBalanceAt(largest.Address, 0)
	if err != nil {
		return false
	}

	return bal.Cmp((*base.MyWei)(&largest.Balance)) == 0
}

// ErrTraceBlockMissing is returned when the node does not support the trace_block endpoint.
var ErrTraceBlockMissing = "trace_block is missing"

// IsNodeTracing returns true if the node exposes the `block_trace` RPC endpoing. It
// queries block 1 (which we presume exists). The function returns false if
// block_trace an error.
func (conn *Connection) IsNodeTracing() bool {
	first := utils.Max(1, base.KnownBlock(conn.Chain, base.FirstTrace))
	_, err := conn.GetTracesByBlockNumber(first)
	return err == nil
}
