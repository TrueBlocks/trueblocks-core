// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */
package main

// EXISTING_CODE
import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// EXISTING_CODE

// DoAbis tests the Abis sdk function
func DoAbis() {
	file.EstablishFolder("sdkFuzzer-output/abis")
	opts := sdk.AbisOptions{}
	ShowHeader("DoAbis", opts)

	globs := noEther(globals)
	known := []bool{false, true}
	proxyFor := fuzzProxyFors
	hint := fuzzHints
	// Fuzz Loop
	// EXISTING_CODE
	_ = hint
	opts.Addrs = fuzzSmartContract
	for _, k := range known {
		for _, p := range proxyFor {
			for _, g := range globs {
				baseFn := "abis/abis"
				opts.Known = k
				isKnown := k
				isProxy := len(p) > 0
				isCache := g.Cache || g.Decache
				if (isKnown || isProxy) && isCache {
					continue
				}
				if k {
					baseFn += "-known"
				}
				if len(p) > 0 {
					baseFn += "-proxy"
					opts.ProxyFor = base.HexToAddress(p)
				}
				opts.Globals = g
				fn := getFilename(baseFn, &opts.Globals)
				TestAbis("abis", "", fn, &opts)
			}
		}
	}

	ShowHeader("DoAbis-Encode", opts)

	sigs := []string{
		"	event Transfer(address indexed from, address indexed to, uint256 amount);",
		"function transfer(address dst, uint amt) external returns (bool)",
		"transfer(address,uint)",
		"transfer",
		"x",
	}

	opts = sdk.AbisOptions{}
	globs = noCache(globs)
	for _, s := range sigs {
		baseFn := "abis/abis-encode" + s[:base.Max(1, base.Min(5, len(s)))]
		for _, g := range globs {
			opts.Globals = g
			fn := getFilename(baseFn, &opts.Globals)
			TestAbis("encode", s, fn, &opts)
		}
	}

	opts = sdk.AbisOptions{}
	ShowHeader("DoAbis-Find", opts)

	finds := []string{"0x1aa3a008"}
	hints := []string{"", "register"}
	globs = noVerbose(globs)

	for _, f := range finds {
		for _, h := range hints {
			opts = sdk.AbisOptions{}
			baseFn := "abis/abis-encode"
			if len(f) > 0 {
				baseFn += "-find"
			}
			if len(h) > 0 {
				baseFn += "-hint"
				opts.Hint = []string{h}
			}
			for _, g := range globs {
				opts.Globals = g
				fn := getFilename(baseFn, &opts.Globals)
				TestAbis("find", f, fn, &opts)
			}
		}
	}
	// EXISTING_CODE
	Wait()
}

func TestAbis(which, value, fn string, opts *sdk.AbisOptions) {
	fn = strings.Replace(fn, ".json", "-"+which+".json", 1)
	// EXISTING_CODE
	// EXISTING_CODE

	switch which {
	case "abis":
		if abis, _, err := opts.Abis(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Function](fn, abis); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "find":
		if find, _, err := opts.AbisFind([]string{value}); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Function](fn, find); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "encode":
		if encode, _, err := opts.AbisEncode(value); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Function](fn, encode); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	default:
		ReportError(fn, opts, fmt.Errorf("unknown which: %s", which))
		logger.Fatal("Quitting...")
		return
	}
}

// EXISTING_CODE
// EXISTING_CODE
