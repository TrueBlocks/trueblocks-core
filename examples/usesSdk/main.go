package main

import (
	"os"
)

func main() {
	DoReceipts()
	DoLogs()
	DoTraces()
	DoState()
	DoAbis()
	DoBlocks()

	// DoNames()
	// DoTransactions()
	// DoTokens()
	// DoWhen()
	// DoList()
	// DoExport()
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
	"1001001",
}

var testAddrs = []string{
	"0x054993ab0f2b1acc0fdc65405ee203b4271bebe6",
}

var testTransactions = []string{
	"1718497.*",
}

var firsts = []string{"46147.0", "50111.0", "52029.0"}

func init() {
	os.Setenv("TB_NO_USERQUERY", "true")
	os.Setenv("TB_SDK_FUZZER", "true")
}
