package main

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

func getFilename(baseName string, g *sdk.Globals) string {
	app := ""
	if g.Ether {
		app += "-ether"
	}
	if g.Raw {
		app += "-raw"
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

	return "usesSdk-output/" + baseName + app + ".json"
}

func ReportError(fn string, err error) {
	logger.Error(fmt.Errorf("NO %s: %v", fn, err))
}

func ReportOkay(fn string) {
	logger.Info(colors.Green, "OK ", fn, colors.Off)
}

func ShowHeader(msg string, opts fmt.Stringer) {
	if opts != nil {
		logger.Info(msg, opts.String())
	} else {
		logger.Info(msg)
	}
}

func noCache(in []sdk.Globals) []sdk.Globals {
	out := []sdk.Globals{}
	for _, g := range in {
		if !g.Cache && !g.Decache {
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

func noRaw(in []sdk.Globals) []sdk.Globals {
	out := []sdk.Globals{}
	for _, g := range in {
		if !g.Raw {
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
		Raw: true,
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
		Raw:   true,
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
	{
		Raw:   true,
		Cache: true,
	},
	{
		Raw:     true,
		Decache: true,
	},
	{
		Raw:     true,
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
	{
		Ether: true,
		Raw:   true,
		Cache: true,
	},
	{
		Ether:   true,
		Raw:     true,
		Decache: true,
	},
	{
		Ether:   true,
		Raw:     true,
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
	// 	Raw:     true,
	// 	Cache:   true,
	// 	Decache: true,
	// },
	{
		Raw:     true,
		Cache:   true,
		Verbose: true,
	},
	{
		Raw:     true,
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
	// 	Raw:     true,
	// 	Cache:   true,
	// 	Decache: true,
	// },
	{
		Ether:   true,
		Raw:     true,
		Cache:   true,
		Verbose: true,
	},
	{
		Ether:   true,
		Raw:     true,
		Decache: true,
		Verbose: true,
	},
	// {
	// 	Ether:   true,
	// 	Cache:   true,
	// 	Decache: true,
	// 	Verbose: true,
	// },
	// {
	// 	Raw:     true,
	// 	Cache:   true,
	// 	Decache: true,
	// 	Verbose: true,
	// },
	// {
	// 	Ether:   true,
	// 	Raw:     true,
	// 	Cache:   true,
	// 	Decache: true,
	// 	Verbose: true,
	// },
}
