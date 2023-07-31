package rpcClient

// TODO: BOGUS This needs to be implemented in a cross-chain, cross-client manner

// IsTracingNode returns true if the node is an archive node. Note currently always returns true.
func IsTracingNode(testMode bool, chain string) bool {
	// TODO: We can test this with a unit test
	if testMode && chain == "non-tracing" {
		return false
	}
	return true
}
