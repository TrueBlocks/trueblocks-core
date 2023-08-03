package rpcClient

// TODO: BOGUS This needs to be implemented in a cross-chain, cross-client manner

// IsNodeTracing returns true if the node is an archive node. Note currently always returns true.
func (conn *Connection) IsNodeTracing(testMode bool) bool {
	// TODO: We can test this with a unit test
	if testMode && conn.Chain == "non-tracing" {
		return false
	}
	return true
}
