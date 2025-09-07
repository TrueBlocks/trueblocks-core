package types

type ChainField string

// Fields in the Chain struct available for sorting.
const (
	ChainChain          ChainField = "chain"
	ChainChainId        ChainField = "chainId"
	ChainIpfsGateway    ChainField = "ipfsGateway"
	ChainLocalExplorer  ChainField = "localExplorer"
	ChainRemoteExplorer ChainField = "remoteExplorer"
	ChainRpcProvider    ChainField = "rpcProvider"
	ChainSymbol         ChainField = "symbol"
)

// GetSortFieldsChain returns a []string of valid sort fields for the type.
func GetSortFieldsChain() []string {
	return []string{"chain", "chainId", "ipfsGateway", "localExplorer", "remoteExplorer", "rpcProvider", "symbol"}
}

// ChainBy returns a comparison function for sorting Chain instances by the given field.
// These comparison functions may be strung together by the CmdChains function.
func ChainBy(field ChainField, order SortOrder) func(p1, p2 Chain) bool {
	switch field {
	case ChainChain: // string
		return func(p1, p2 Chain) bool {
			if order == Ascending {
				return p1.Chain < p2.Chain
			}
			return p1.Chain > p2.Chain
		}
	case ChainChainId: // uint64
		return func(p1, p2 Chain) bool {
			if order == Ascending {
				return p1.ChainId < p2.ChainId
			}
			return p1.ChainId > p2.ChainId
		}
	case ChainIpfsGateway: // string
		return func(p1, p2 Chain) bool {
			if order == Ascending {
				return p1.IpfsGateway < p2.IpfsGateway
			}
			return p1.IpfsGateway > p2.IpfsGateway
		}
	case ChainLocalExplorer: // string
		return func(p1, p2 Chain) bool {
			if order == Ascending {
				return p1.LocalExplorer < p2.LocalExplorer
			}
			return p1.LocalExplorer > p2.LocalExplorer
		}
	case ChainRemoteExplorer: // string
		return func(p1, p2 Chain) bool {
			if order == Ascending {
				return p1.RemoteExplorer < p2.RemoteExplorer
			}
			return p1.RemoteExplorer > p2.RemoteExplorer
		}
	case ChainRpcProvider: // string
		return func(p1, p2 Chain) bool {
			if order == Ascending {
				return p1.RpcProvider < p2.RpcProvider
			}
			return p1.RpcProvider > p2.RpcProvider
		}
	case ChainSymbol: // string
		return func(p1, p2 Chain) bool {
			if order == Ascending {
				return p1.Symbol < p2.Symbol
			}
			return p1.Symbol > p2.Symbol
		}

	}
	logger.ShouldNotHappen("in ChainBy")
	return nil
}

// ChainCmp accepts a slice and variadic comparison functions and returns a functions
// that can be used to sort the slice.
func ChainCmp(slice []Chain, orders ...func(p1, p2 Chain) bool) func(i, j int) bool {
	return func(i, j int) bool {
		p1, p2 := slice[i], slice[j]
		for _, order := range orders {
			if order(p1, p2) {
				return true
			}
			if order(p2, p1) {
				return false
			}
		}
		return false
	}
}
