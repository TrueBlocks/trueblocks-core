package articulate

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/topics"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func parseTransferEvent(log *types.Log) (function *types.Function) {
	if len(log.Topics) < 3 || log.Topics[0] != topics.TransferTopic {
		// TODO: Too short topics happens (sometimes) because the ABI says that the data is not
		// TODO: index, but it is or visa versa. In either case, we get the same topic0. We need to
		// TODO: attempt both with and without indexed parameters. See issues/1366.
		return nil
	}

	function = &types.Function{}
	function.Name = "Transfer"
	function.FunctionType = "event"
	function.Encoding = topics.TransferTopic.Hex()
	function.Inputs = []types.Parameter{
		{
			Name:          "_from",
			ParameterType: "address",
			Value:         log.Topics[1].Hex(),
		},
		{
			Name:          "_to",
			ParameterType: "address",
			Value:         log.Topics[2].Hex(),
		},
		{
			Name:          "_amount",
			ParameterType: "uint256",
			Value:         log.Data,
		},
	}
	return
}

func parseEnsTransferEvent(log *types.Log) (function *types.Function) {
	if len(log.Topics) < 2 || log.Topics[0] != topics.EnsTransferTopic {
		// TODO: Too short topics happens (sometimes) because the ABI says that the data is not
		// TODO: index, but it is or visa versa. In either case, we get the same topic0. We need to
		// TODO: attempt both with and without indexed parameters. See issues/1366.
		return nil
	}

	function = &types.Function{}
	function.Name = "Transfer"
	function.FunctionType = "event"
	function.Encoding = topics.EnsTransferTopic.Hex()
	function.Inputs = []types.Parameter{
		{
			Name:          "_node",
			ParameterType: "bytes32",
			Value:         log.Topics[1].Hex(),
		},
		{
			Name:          "_owner",
			ParameterType: "address",
			Value:         log.Data,
		},
	}
	return
}

func parseApprovalEvent(log *types.Log) (function *types.Function) {
	if len(log.Topics) < 3 || log.Topics[0] != topics.ApprovalTopic {
		// TODO: Too short topics happens (sometimes) because the ABI says that the data is not
		// TODO: index, but it is or visa versa. In either case, we get the same topic0. We need to
		// TODO: attempt both with and without indexed parameters. See issues/1366.
		return nil
	}

	function = &types.Function{}
	function.Name = "Approval"
	function.FunctionType = "event"
	function.Encoding = topics.ApprovalTopic.Hex()
	function.Inputs = []types.Parameter{
		{
			Name:          "_owner",
			ParameterType: "address",
			Value:         log.Topics[1].Hex(),
		},
		{
			Name:          "_spender",
			ParameterType: "address",
			Value:         log.Topics[2].Hex(),
		},
		{
			Name:          "_amount",
			ParameterType: "uint256",
			Value:         log.Data,
		},
	}
	return
}
