package rpc

import (
	"os"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/prefunds"
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

	return bal.Cmp(&largest.Balance) == 0
}

// IsNodeTracing returns true if the node exposes the `block_trace` RPC endpoint.
// It queries block 1 or a user supplied block (which we presume exists). The function
// returns false if block_trace returns an error or doesn't exist.
func (conn *Connection) IsNodeTracing() (error, bool) {
	firstTrace := base.Max(1, base.KnownBlock(conn.Chain, base.FirstTrace))
	varName := "TB_" + strings.ToUpper(conn.Chain) + "_FIRSTTRACE"
	if len(os.Getenv(varName)) > 0 {
		firstTrace = base.Max(firstTrace, base.MustParseValue(os.Getenv(varName)))
	}
	_, err := conn.GetTracesByBlockNumber(firstTrace)
	return err, err == nil
}
