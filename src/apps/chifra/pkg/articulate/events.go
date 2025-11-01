package articulate

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/topics"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/types"
)

// parseTransferEvent converts a normalized transfer log to a Function
func parseTransferEvent(log *types.Log) *types.Function {
	return &types.Function{
		Name:         "Transfer",
		FunctionType: "event",
		Encoding:     topics.TransferTopic.Hex(),
		Inputs: []types.Parameter{
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
		},
	}
}

// parseEnsTransferEvent converts a normalized transfer log to a Function
func parseEnsTransferEvent(log *types.Log) *types.Function {
	return &types.Function{
		Name:         "Transfer",
		FunctionType: "event",
		Encoding:     topics.EnsTransferTopic.Hex(),
		Inputs: []types.Parameter{
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
		},
	}
}

// parseApprovalEvent converts a normalized transfer log to a Function
func parseApprovalEvent(log *types.Log) *types.Function {
	return &types.Function{
		Name:         "Approval",
		FunctionType: "event",
		Encoding:     topics.ApprovalTopic.Hex(),
		Inputs: []types.Parameter{
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
		},
	}
}
