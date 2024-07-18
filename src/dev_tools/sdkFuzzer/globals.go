package main

import (
	"fmt"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/sdk/v3"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

var fuzzAddresses = []string{"0x054993ab0f2b1acc0fdc65405ee203b4271bebe6"}
var fuzzBlocks = []string{"46147", "1001001"}
var fuzzSmartContract = []string{"0x3d9819210A31b4961b30EF54bE2aeD79B9c9Cd3B"}
var fuzzTransactions = []string{"50111.*"}

var firsts = []string{"46147.0", "50111.0", "52029.0"}
var fuzzAssets = []string{"", "0x89d24a6b4ccb1b6faa2625fe562bdd9a23260359"}
var fuzzBelongs = []string{"", "0x0101f3be8ebb4bbd39a2e3b9a3639d4259832fd9"}
var fuzzEmitters = []string{"", "0x5564886ca2c518d1964e5fcea4f423b41db9f561"}
var fuzzFourbytes = []string{}
var fuzzHints = []string{"", "Reward"}
var fuzzProxyFors = []string{"", "0xbb2b8038a1640196fbe3e38816f3e67cba72d940"}
var fuzzTopics = []string{"", "0xa6697e974e6a320f454390be03f74955e8978f1a6971ea6730542e37b66179bc"}

func init() {
	os.Setenv("TB_NO_USERQUERY", "true")
	os.Setenv("TB_SDK_FUZZER", "true")
}

func getFilename(baseName string, g *sdk.Globals) string {
	app := ""
	if g.Ether {
		app += "-ether"
	}
	if g.Cache {
		app += "-cache"
	}
	if g.Decache {
		app += "-decache"
	}
	if g.Verbose {
		app += "-verbose"
	}

	return "sdkFuzzer-output/" + baseName + app + ".json"
}

var spaces = strings.Repeat(" ", 30)

func ShowHeader(msg string, opts fmt.Stringer) {
	logger.Info()
	if opts != nil {
		logger.Info(colors.Yellow+msg, opts.String(), colors.Off)
	} else {
		logger.Info(colors.Yellow+msg, colors.Off)
	}
}

func ReportError(fn string, opts fmt.Stringer, err error) {
	logger.Error(fmt.Errorf("NO %s (%s): %v%s", fn, opts.String(), err, spaces))
	logger.Error(fmt.Errorf("NO %s (%s): %v%s", fn, opts.String(), err, spaces))
}

func ReportError2(fn string, err error) {
	logger.Error(fmt.Errorf("NO %s: %v%s", fn, err, spaces))
}

func ReportOkay(fn string) {
	// logger.Progress(true, colors.Green, "OK ", fn, colors.Off, spaces)
	logger.Info(colors.Green, "OK ", fn, colors.Off, spaces)
}

func noCache(in []sdk.Globals) []sdk.Globals {
	out := []sdk.Globals{}
	for _, g := range in {
		if !(g.Cache || g.Decache) {
			out = append(out, g)
		}
	}
	return out
}

func noEther(in []sdk.Globals) []sdk.Globals {
	out := []sdk.Globals{}
	for _, g := range in {
		if !g.Ether {
			out = append(out, g)
		}
	}
	return out
}

func noVerbose(in []sdk.Globals) []sdk.Globals {
	out := []sdk.Globals{}
	for _, g := range in {
		if !g.Verbose {
			out = append(out, g)
		}
	}
	return out
}

var globals = []sdk.Globals{
	{},
	{
		Ether: true,
	},
	{
		Cache: true,
	},
	{
		Decache: true,
	},
	{
		Verbose: true,
	},
	{
		Ether: true,
		Cache: true,
	},
	{
		Ether:   true,
		Decache: true,
	},
	{
		Ether:   true,
		Verbose: true,
	},
	// {
	// 	Cache:   true,
	// 	Decache: true,
	// },
	{
		Cache:   true,
		Verbose: true,
	},
	{
		Decache: true,
		Verbose: true,
	},
	// {
	// 	Ether:   true,
	// 	Cache:   true,
	// 	Decache: true,
	// },
	{
		Ether:   true,
		Cache:   true,
		Verbose: true,
	},
	{
		Ether:   true,
		Decache: true,
		Verbose: true,
	},
	// {
	// 	Cache:   true,
	// 	Decache: true,
	// 	Verbose: true,
	// },
	// {
	// 	Ether:   true,
	// 	Cache:   true,
	// 	Decache: true,
	// 	Verbose: true,
	// },
}

func Wait() {
	// reader := bufio.NewReader(os.Stdin)
	// fmt.Fprintf(os.Stderr, colors.Yellow+"%s"+colors.Off, "Waiting...")
	// _, _ = reader.ReadString('\n')
}
