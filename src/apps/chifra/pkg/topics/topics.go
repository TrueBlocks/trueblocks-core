package topics

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"

// TransferTopic is here because these three topics make up almost all of the logs in the entire history
// of the chain, we get significant speed-ups if we handle these items without regular processing.
var TransferTopic = base.HexToHash(
	"0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef",
)
var ApprovalTopic = base.HexToHash(
	"0x8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b925",
)
var EnsTransferTopic = base.HexToHash(
	"0xd4735d920b0f87494915f556dd9b54c8f309026070caea5c737245152564d266",
)

var ApprovalFourbyte = base.HexToHash("0x095ea7b3")

var KnownTopics = map[base.Hash]bool{
	TransferTopic: true,
	ApprovalTopic: true,
}
