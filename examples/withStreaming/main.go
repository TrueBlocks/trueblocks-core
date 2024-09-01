package main

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func main() {
	namesMap, _ = names.LoadNamesMap("mainnet", types.Custom|types.Prefund|types.Regular, nil)

	// TestStreamAbis()
	TestStreamBlocks()
	// TestStreamChunks()
	// TestStreamConfig()
	// TestStreamExport()
	// TestStreamInit()
	// TestStreamList()
	// TestStreamLogs()
	// TestStreamMonitors()
	// TestStreamNames()
	// TestStreamReceipts()
	// TestStreamSlurp()
	// TestStreamState()
	// TestStreamStatus()
	// TestStreamTokens()
	// TestStreamTraces()
	// TestStreamTransactions()
	// TestStreamWhen()
	// TestStreamProgress()
}

var namesMap = map[base.Address]types.Name{}
