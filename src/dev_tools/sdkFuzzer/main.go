package main

import (
	"os"
)

func main() {
	// DoAbis()
	DoBlocks()
	// DoChunks() // this needs a lot of work
	// DoConfig()
	// DoExport()
	// DoList()
	// DoLogs()
	// DoNames() // this does not test crud commands
	// DoReceipts()
	// DoSlurp()
	// DoState()
	// DoStatus()
	// DoTokens()
	// DoTraces()
	// DoTransactions()
	// DoWhen()

	// DoInit()
	// DoMonitors()

	// DoDaemon - do not test daemon in fuzzer

	// DoScrape - no sdk routines for scrape
	// DoExplore - no sdk routines for explore
}

func Wait() {
	// reader := bufio.NewReader(os.Stdin)
	// fmt.Fprintf(os.Stderr, colors.Yellow+"%s"+colors.Off, "Waiting...")
	// _, _ = reader.ReadString('\n')
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
