package articulate

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum/common"
)

// Because these three topics make up almost all of the logs in the entire history
// of the chain, we get significant speed-ups if we handle these items without
// regular processing.
var transferTopic = common.HexToHash(
	"0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef",
)
var ensTransferTopic = common.HexToHash(
	"0xd4735d920b0f87494915f556dd9b54c8f309026070caea5c737245152564d266",
)
var approvalTopic = common.HexToHash(
	"0x8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b925",
)

func ParseTransferEvent(log *types.SimpleLog) (function *types.SimpleFunction) {
	if log.Topics[0] != transferTopic {
		return nil
	}
	if len(log.Topics) < 3 {
		return nil
	}

	function = &types.SimpleFunction{}
	function.Name = "Transfer"
	function.FunctionType = "event"
	function.Encoding = transferTopic.Hex()
	function.Inputs = []types.SimpleParameter{
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

func ParseEnsTransferEvent(log *types.SimpleLog) (function *types.SimpleFunction) {
	if log.Topics[0] != ensTransferTopic {
		return nil
	}
	if len(log.Topics) < 2 {
		return nil
	}

	function = &types.SimpleFunction{}
	function.Name = "Transfer"
	function.FunctionType = "event"
	function.Encoding = transferTopic.Hex()
	function.Inputs = []types.SimpleParameter{
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

func ParseApprovalEvent(log *types.SimpleLog) (function *types.SimpleFunction) {
	if log.Topics[0] != approvalTopic {
		return nil
	}
	if len(log.Topics) < 3 {
		return nil
	}

	function = &types.SimpleFunction{}
	function.Name = "Transfer"
	function.FunctionType = "event"
	function.Encoding = transferTopic.Hex()
	function.Inputs = []types.SimpleParameter{
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
