package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
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

	return "usesSdk/" + baseName + app + ".json"
}

func Report(fn string) {
	logger.Info("----------------------------------------------------")
	logger.Info(fn)
	// logger.Info(opts.String())
	// logger.Info("----------------------------------------------------")
}

func doCache(on bool, in []sdk.Globals) []sdk.Globals {
	out := []sdk.Globals{}
	for _, g := range in {
		if g.Cache == on && g.Decache == on {
			out = append(out, g)
		}
	}
	return out
}

func noCache(in []sdk.Globals) []sdk.Globals {
	return doCache(false, in)
}

func yesCache(in []sdk.Globals) []sdk.Globals {
	return doCache(true, in)
}

func doEther(on bool, in []sdk.Globals) []sdk.Globals {
	out := []sdk.Globals{}
	for _, g := range in {
		if g.Ether == on {
			out = append(out, g)
		}
	}
	return out
}

func noEther(in []sdk.Globals) []sdk.Globals {
	return doEther(false, in)
}

func yesEther(in []sdk.Globals) []sdk.Globals {
	return doEther(true, in)
}

func doRaw(on bool, in []sdk.Globals) []sdk.Globals {
	out := []sdk.Globals{}
	for _, g := range in {
		if g.Raw == on {
			out = append(out, g)
		}
	}
	return out
}

func noRaw(in []sdk.Globals) []sdk.Globals {
	return doRaw(false, in)
}

func yesRaw(in []sdk.Globals) []sdk.Globals {
	return doRaw(true, in)
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
