package main

import (
	"os"
	"sort"
	"strings"
)

type FuzzFuncs struct {
	num  int
	name string
	f    func()
	on   bool
}

func main() {
	fuzzFuncs := getFuzzFuncs()
	for _, fuzzer := range fuzzFuncs {
		fuzzer.f()
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
		if _, ok := commandMap[cmd]; ok {
			f := commandMap[cmd]
			f.on = true
			commandMap[cmd] = f
		}
	}

	fuzzFuncs := []FuzzFuncs{}
	for k, v := range commandMap {
		if v.on {
			fuzzFuncs = append(fuzzFuncs, commandMap[k])
		}
	}
	sort.Slice(fuzzFuncs, func(i, j int) bool {
		return fuzzFuncs[i].num < fuzzFuncs[j].num
	})
	return fuzzFuncs
}

var commandMap = map[string]FuzzFuncs{
	"receipts":     {1, "receipts", DoReceipts, false},
	"abis":         {2, "abis", DoAbis, false},
	"blocks":       {3, "blocks", DoBlocks, false},
	"chunks":       {4, "chunks", DoChunks, false},
	"config":       {5, "config", DoConfig, false},
	"export":       {6, "export", DoExport, false},
	"list":         {7, "list", DoList, false},
	"logs":         {8, "logs", DoLogs, false},
	"names":        {9, "names", DoNames, false},
	"slurp":        {10, "slurp", DoSlurp, false},
	"state":        {11, "state", DoState, false},
	"status":       {12, "status", DoStatus, false},
	"tokens":       {13, "tokens", DoTokens, false},
	"traces":       {14, "traces", DoTraces, false},
	"transactions": {15, "transactions", DoTransactions, false},
	"when":         {16, "when", DoWhen, false},
	"init":         {17, "init", DoInit, false},
	"monitors":     {18, "monitors", DoMonitors, false},
	"daemon":       {19, "daemon", DoDaemon, false},
	// "scrape":    {20, "scrape", DoScrape, false},
	// "explore":   {21, "explore", DoExplore, false},
}
