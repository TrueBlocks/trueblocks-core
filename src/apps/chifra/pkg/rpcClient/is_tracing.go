package rpcClient

// TODO: BOGUS This needs to be implemented in a cross-chain, cross-client manner

func IsNodeTracing(testMode bool, chain string) bool {
	// TODO: We can test this with a unit test
	if testMode && chain == "non-tracing" {
		return false
	}
	return true
}
