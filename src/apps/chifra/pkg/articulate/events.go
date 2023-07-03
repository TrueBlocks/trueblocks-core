package articulate

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// Because these three topics make up almost all of the logs in the entire history
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

// static bool isDepositTransfer(const string_q& logNeedle) {
//     return logNeedle == str_2_Topic("0xe1fffcc4923d04b559f4d29a8bfc6cda04eb5b0d3c460751c2402c5c5cc9109c");
// }

// //-----------------------------------------------------------------------
// static bool isWithdrawalTransfer(const string_q& logNeedle) {
//     return logNeedle == str_2_Topic("0x7fcf532c15f0a6db0bd6d0e038bea71d30d808c7d98cb3bf7268a95bf5081b65");
// }

// //--------------------------------------------------------------
// static bool isMintTransfer(const string_q& logNeedle) {
//     return logNeedle == str_2_Topic("0x0f6798a560793a54c3bcfe86a93cde1e73087d944c0ea20544137d4121396885");
// }

// //--------------------------------------------------------------
// static bool isBurnTransfer(const string_q& logNeedle) {
//     return logNeedle == str_2_Topic("0xcc16f5dbb4873280815c1ee09dbd06736cffcc184412cf7a71a0fdb75d397ca5");
// }

// //--------------------------------------------------------------
// static bool isStakedTransfer(const string_q& logNeedle) {
//     return logNeedle == str_2_Topic("0x9e71bc8eea02a63969f509818f2dafb9254532904319f9dbda79b67bd34a5f3d");
// }

func ParseTransferEvent(log *types.SimpleLog) (function *types.SimpleFunction) {
	if log.Topics[0] != transferTopic {
		return nil
	}
	if len(log.Topics) < 3 {
		// TODO: This happens (sometimes) because the ABI says that the data is not index, but it is
		// TODO: or visa versa. In either case, we get the same topic0. We need to attempt both with
		// TODO: and without indexed parameters. See issues/1366.
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
		// TODO: This happens (sometimes) because the ABI says that the data is not index, but it is
		// TODO: or visa versa. In either case, we get the same topic0. We need to attempt both with
		// TODO: and without indexed parameters. See issues/1366.
		return nil
	}

	function = &types.SimpleFunction{}
	function.Name = "Transfer"
	function.FunctionType = "event"
	function.Encoding = transferTopic.Hex() // TODO: THIS IS WRONG! SHOULDN'T THIS BE ENSTRANSFERTOPIC?
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
		// TODO: This happens (sometimes) because the ABI says that the data is not index, but it is
		// TODO: or visa versa. In either case, we get the same topic0. We need to attempt both with
		// TODO: and without indexed parameters. See issues/1366.
		return nil
	}

	function = &types.SimpleFunction{}
	function.Name = "Approval"
	function.FunctionType = "event"
	function.Encoding = transferTopic.Hex() // TODO: THIS IS WRONG! SHOULDN'T THIS BE APPROVALTOPIC?
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
