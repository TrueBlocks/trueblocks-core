package main

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"

func main() {
	// DoSlurp()
	// DoNames()
	DoBlocks()
	// DoTransactions()
	// DoReceipts()
	// DoLogs()
	// DoTraces()
	// DoState()
	// DoTokens()
	DoAbis()
	// DoWhen()
	// DoList()
	// DoExport()
	// DoMonitors()
	// DoConfig()
	// DoStatus()
	// // DoDaemon()
	// // DoScrape()
	// DoChunks()
	// // DoInit()
	// DoExplore()
}

var testBlocks = []string{
	"5001001",
}

var testAddrs = []string{
	"0x054993ab0f2b1acc0fdc65405ee203b4271bebe6",
}

func init() {
	// These test cases drop files in the usesSDK folder. This line
	// creates the folder in the cwd.
	file.EstablishFolder("usesSDK")
}
