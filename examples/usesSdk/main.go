package main

import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

func main() {
	// DoNames()
	// DoBlocks()
	// DoTransactions()
	//+
	DoReceipts()
	//+
	DoLogs()
	//+
	DoTraces()
	//+
	DoState()
	// DoTokens()
	// DoAbis()
	// DoWhen()
	//- DoList()
	//- DoExport()
	// DoMonitors()
	// DoConfig()
	// DoStatus()
	// DoDaemon()
	// // DoScrape()
	//- DoChunks()
	// DoInit()
	// DoExplore()
	// DoSlurp()
}

var testBlocks = []string{
	"46147",
}

var testAddrs = []string{
	"0x054993ab0f2b1acc0fdc65405ee203b4271bebe6",
}

var testTransactions = []string{
	"1718497.*",
}

var firsts = []string{"46147.0", "50111.0", "52029.0"}

func init() {
	// These test cases drop files in the usesSDK folder. This line
	// creates the folder in the cwd.
	file.EstablishFolder("usesSDK/logs")
	file.EstablishFolder("usesSDK/receipts")
	file.EstablishFolder("usesSDK/traces")
	file.EstablishFolder("usesSDK/state")
	os.Setenv("TB_NO_USERQUERY", "true")
}
