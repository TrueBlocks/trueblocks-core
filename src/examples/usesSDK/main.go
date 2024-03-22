package main

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"

func main() {
	file.EstablishFolder("usesSDK")
	// DoList()
	// DoExport()
	// DoMonitors()
	// DoNames()
	// DoAbis()
	// DoBlocks()
	// DoTransactions()
	// DoReceipts()
	// DoLogs()
	// DoTraces()
	// DoWhen()
	DoState()
	// DoTokens()
	// // DoConfig()
	// DoStatus()
	// DoDaemon()
	// // DoScrape()
	// DoChunks()
	// // DoInit()
	// DoExplore()
	// DoSlurp()
}

var testBlocks = []string{
	"13800-1000000:monthly",
	"14011011",
}

var testAddrs = []string{
	"0x054993ab0f2b1acc0fdc65405ee203b4271bebe6",
}
