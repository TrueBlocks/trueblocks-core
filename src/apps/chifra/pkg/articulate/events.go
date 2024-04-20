package articulate

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// transferTopic is here because these three topics make up almost all of the logs in the entire history
// of the chain, we get significant speed-ups if we handle these items without
// regular processing.
var transferTopic = base.HexToHash(
	"0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef",
)
var ensTransferTopic = base.HexToHash(
	"0xd4735d920b0f87494915f556dd9b54c8f309026070caea5c737245152564d266",
)
var approvalTopic = base.HexToHash(
	"0x8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b925",
)

func parseTransferEvent(log *types.Log) (function *types.Function) {
	if len(log.Topics) < 3 || log.Topics[0] != transferTopic {
		// TODO: Too short topics happens (sometimes) because the ABI says that the data is not
		// TODO: index, but it is or visa versa. In either case, we get the same topic0. We need to
		// TODO: attempt both with and without indexed parameters. See issues/1366.
		return nil
	}

	function = &types.Function{}
	function.Name = "Transfer"
	function.FunctionType = "event"
	function.Encoding = transferTopic.Hex()
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
	if len(log.Topics) < 2 || log.Topics[0] != ensTransferTopic {
		// TODO: Too short topics happens (sometimes) because the ABI says that the data is not
		// TODO: index, but it is or visa versa. In either case, we get the same topic0. We need to
		// TODO: attempt both with and without indexed parameters. See issues/1366.
		return nil
	}

	function = &types.Function{}
	function.Name = "Transfer"
	function.FunctionType = "event"
	function.Encoding = ensTransferTopic.Hex()
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
	if len(log.Topics) < 3 || log.Topics[0] != approvalTopic {
		// TODO: Too short topics happens (sometimes) because the ABI says that the data is not
		// TODO: index, but it is or visa versa. In either case, we get the same topic0. We need to
		// TODO: attempt both with and without indexed parameters. See issues/1366.
		return nil
	}

	function = &types.Function{}
	function.Name = "Approval"
	function.FunctionType = "event"
	function.Encoding = approvalTopic.Hex()
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
