package main

import (
	"os"
	"strings"
)

type FuzzFuncs func()

func main() {
	fuzzFuncs := getFuzzFuncs()
	for _, fuzzer := range fuzzFuncs {
		fuzzer()
	}
}

func getFuzzFuncs() []FuzzFuncs {
	enabled := os.Getenv("TB_TEST_FILTER")
	if len(enabled) == 0 {
		if len(os.Args) > 1 {
			enabled = os.Args[1]
		} else {
			enabled = "receipts|abis|blocks|chunks|config|export|list|logs|names|slurp|state|status|tokens|traces|transactions|when"
		}
	}

	for _, cmd := range strings.Split(enabled, "|") {
		if _, ok := allCommands[cmd]; ok {
			allCommands[cmd] = true
		}
	}

	fuzzFuncs := []FuzzFuncs{}
	for k, v := range allCommands {
		if v {
			fuzzFuncs = append(fuzzFuncs, commandMap[k])
		}
	}
	return fuzzFuncs
}

var allCommands = map[string]bool{
	"receipts":     false,
	"abis":         false,
	"blocks":       false,
	"chunks":       false,
	"config":       false,
	"export":       false,
	"list":         false,
	"logs":         false,
	"names":        false,
	"slurp":        false,
	"state":        false,
	"status":       false,
	"tokens":       false,
	"traces":       false,
	"transactions": false,
	"when":         false,
	"init":         false,
	"monitors":     false,
	"daemon":       false,
	// "scrape":           false,
	// "explore":          false,
}

var commandMap = map[string]FuzzFuncs{
	"receipts":     DoReceipts,
	"abis":         DoAbis,
	"blocks":       DoBlocks,
	"chunks":       DoChunks,
	"config":       DoConfig,
	"export":       DoExport,
	"list":         DoList,
	"logs":         DoLogs,
	"names":        DoNames,
	"slurp":        DoSlurp,
	"state":        DoState,
	"status":       DoStatus,
	"tokens":       DoTokens,
	"traces":       DoTraces,
	"transactions": DoTransactions,
	"when":         DoWhen,
	"init":         DoInit,
	"monitors":     DoMonitors,
	"daemon":       DoDaemon,
	// "scrape":           DoScrape,
	// "explore":          DoExplore,
}
