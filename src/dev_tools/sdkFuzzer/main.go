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
	DoWhen()
	DoList()
	DoNames() // does not test crud commands
	DoConfig()
	DoTransactions()
	DoTokens()
	// DoSlurp()

	// DoChunks()
	// DoExport()
	// DoStatus()

	// DoInit()
	// DoMonitors()

	// DoDaemon - do not test daemon in fuzzer

	// DoScrape - no sdk routines for scrape
	// DoExplore - no sdk routines for explore
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
