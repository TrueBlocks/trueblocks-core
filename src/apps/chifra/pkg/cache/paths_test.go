package cache

import (
	"fmt"
	"path/filepath"
	"strconv"
	"strings"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func Test_getPaths(t *testing.T) {
	e := "blocks/00/00/92/000092590.bin"
	g := getPathByBlock(Cache_Blocks, 92590)
	if g != e {
		t.Error("getPathByBlock", "wanted", e, "got", g)
	}

	e = "txs/00/00/92/000092590-00012.bin"
	g = getPathByBlockAndTransactionIndex(Cache_Transactions, 92590, 12)
	if g != e {
		t.Error("getPathByBlockAndTransactionIndex", "wanted", e, "got", g)
	}

	e = "traces/00/00/92/000092590-00012.bin"
	g = getPathByBlockAndTransactionIndex(Cache_Traces, 92590, 12)
	if g != e {
		t.Error("getPathByBlockAndTransactionIndex", "wanted", e, "got", g)
	}
}

func Test_cmdToCacheType(t *testing.T) {
	e := Cache_Blocks
	g := CmdToCacheType("blocks")
	if e != g {
		t.Error("cmdToCacheType", "wanted", e, "got", g)
	}

	e = Cache_NotACache
	g = CmdToCacheType("no")
	if e != g {
		t.Error("cmdToCacheType", "wanted", e, "got", g)
	}
}

func Test_cacheLayout(t *testing.T) {
	indexPath := strings.Trim(config.GetPathToIndex(utils.GetTestChain()), "/")
	cachePath := strings.Trim(config.GetPathToCache(utils.GetTestChain()), "/")

	for _, tt := range tests {
		if !tt.on {
			continue
		}
		tt.name = tt.expected.cacheType.String() + " path test"
		tt.expected.rootPart = strings.ReplaceAll(tt.expected.rootPart, "[{CachePath}]", cachePath)
		tt.expected.rootPart = strings.ReplaceAll(tt.expected.rootPart, "[{IndexPath}]", indexPath)

		t.Run(tt.name, func(t *testing.T) {
			gotten := getCacheData(tt.expected.chain, tt.expected.cacheType, tt.param)

			e := tt.expected.rootPart
			g := gotten.rootPart
			// fmt.Println(e, g, e == g)
			if e != g {
				t.Error(tt.name, "rootPart", "wanted", e, "got", g)
			}

			e = tt.expected.typePart
			g = gotten.typePart
			// fmt.Println(e, g, e == g)
			if e != g {
				t.Error(tt.name, "typePart", "wanted", e, "got", g)
			}

			e = tt.expected.extraPart
			g = gotten.extraPart
			// fmt.Println(e, g, e == g)
			if e != g {
				t.Error(tt.name, "extraPart", "wanted", e, "got", g)
			}

			e = tt.expected.extension
			g = gotten.extension
			// fmt.Println(e, g, e == g)
			if e != g {
				t.Error(tt.name, "extension", "wanted", e, "got", g)
			}

			e = tt.expected.theAnswer
			g = gotten.GetFullPath(tt.param)
			// fmt.Println(e, g, e == g)
			if e != g {
				t.Error(tt.name, "theAnswer", "wanted", e, "got", g)
			}
		})
	}
}

func Test_GetPathFromCacheType(t *testing.T) {
	indexPath := strings.Trim(config.GetPathToIndex(utils.GetTestChain()), "/")
	cachePath := strings.Trim(config.GetPathToCache(utils.GetTestChain()), "/")

	for _, tt := range tests {
		if !tt.on {
			continue
		}
		tt.name = tt.expected.cacheType.String() + " root path test"
		p := GetRootPathFromCacheType(tt.expected.chain, tt.expected.cacheType)
		p = strings.Replace(p, indexPath, "", -1)
		p = strings.Replace(p, cachePath, "", -1)
		p = filepath.Clean(p)
		a := "/" + strings.Replace(tt.expected.theAnswer, filepath.Base(tt.expected.theAnswer), "", -1)
		a = filepath.Clean(a)
		// fmt.Println("Path:", p)
		// fmt.Println("theAnswer:", a)
		// fmt.Println("passes:", strings.Contains(a, p))
		if !strings.Contains(a, p) {
			t.Error(tt.name, "wanted", a, "got", p)
		}
	}
}

type cacheItem struct {
	cacheType CacheType
	chain     string
	rootPart  string
	typePart  string
	extraPart string
	extension string
	theAnswer string
}

func (c *cacheItem) GetFullPath(param string) string {
	var ret string
	switch c.cacheType {
	case Index_Ripe:
		fallthrough
	case Index_Staging:
		fallthrough
	case Index_Unripe:
		fallthrough
	case Cache_Blocks:
		_, paddedBn, _ := getDirStructureByBlock(mustParseUint(param), 0)
		param = paddedBn
	case Cache_Traces:
		fallthrough
	case Cache_Transactions:
		parts := strings.Split(param, ".")
		_, paddedBn, paddedTx := getDirStructureByBlock(mustParseUint(parts[0]), mustParseUint(parts[1]))
		param = paddedBn + "." + paddedTx
	}

	indexPath := strings.Trim(config.GetPathToIndex(utils.GetTestChain()), "/")
	cachePath := strings.Trim(config.GetPathToCache(utils.GetTestChain()), "/")

	ret = filepath.Join(c.rootPart, c.typePart, c.extraPart, param) + c.extension
	ret = strings.Replace(ret, cachePath+"/", "", -1)
	ret = strings.Replace(ret, indexPath+"/", "", -1)

	// fmt.Println(c, ret)
	return ret
}

func getCacheData(chain string, cT CacheType, param string) *cacheItem {
	path := strings.Trim(GetRootPathFromCacheType(chain, cT), "/")
	parts := strings.Split(path, "/")
	if len(parts) < 1 {
		return nil
	}

	root := strings.Join(parts[:len(parts)-1], "/")
	subFolder := parts[len(parts)-1]
	ext := ""
	if len(cacheTypeToExt[cT]) > 0 {
		ext = "." + cacheTypeToExt[cT]
	}
	extra := ""
	if cT == Cache_Blocks {
		extra, _, _ = getDirStructureByBlock(mustParseUint(param), 0)
	} else if cT == Cache_Traces || cT == Cache_Transactions {
		parts := strings.Split(param, ".")
		extra, _, _ = getDirStructureByBlock(mustParseUint(parts[0]), mustParseUint(parts[1]))
	}

	return &cacheItem{
		chain:     chain,
		cacheType: cT,
		rootPart:  root,
		typePart:  subFolder,
		extraPart: extra,
		extension: ext,
	}
}

func mustParseUint(input any) (result uint64) {
	result, _ = strconv.ParseUint(fmt.Sprint(input), 0, 64)
	return
}

var tests = []struct {
	on       bool
	name     string
	param    string
	expected cacheItem
	wantErr  bool
}{
	{
		on:    true,
		param: "0xf503017d7baf7fbc0fff7492b751025c6a78179b",
		expected: cacheItem{
			cacheType: Cache_Abis,
			rootPart:  `[{CachePath}]`,
			typePart:  "abis",
			extraPart: "",
			extension: ".json",
			theAnswer: "abis/0xf503017d7baf7fbc0fff7492b751025c6a78179b.json",
		},
		wantErr: false,
	},
	{
		on:    true,
		param: "1001001",
		expected: cacheItem{
			cacheType: Cache_Blocks,
			rootPart:  `[{CachePath}]`,
			typePart:  "blocks",
			extraPart: "00/10/01",
			extension: ".bin",
			theAnswer: "blocks/00/10/01/001001001.bin",
		},
		wantErr: false,
	},
	{
		on:    true,
		param: "0xf503017d7baf7fbc0fff7492b751025c6a78179b",
		expected: cacheItem{
			cacheType: Cache_Monitors,
			rootPart:  `[{CachePath}]`,
			typePart:  "monitors",
			extraPart: "",
			extension: ".mon.bin",
			theAnswer: "monitors/0xf503017d7baf7fbc0fff7492b751025c6a78179b.mon.bin",
		},
		wantErr: false,
	},
	{
		on:    true,
		param: "names",
		expected: cacheItem{
			cacheType: Cache_Names,
			rootPart:  `[{CachePath}]`,
			typePart:  "names",
			extraPart: "",
			extension: ".bin",
			theAnswer: "names/names.bin",
		},
		wantErr: false,
	},
	{
		// TODO: BOGUS TURN THIS ON
		on:    false,
		param: "0xf503017d7baf7fbc0fff7492b751025c6a78179b|9320587|31",
		expected: cacheItem{
			cacheType: Cache_Recons,
			rootPart:  `[{CachePath}]`,
			typePart:  "recons",
			extraPart: "f503/017d",
			extension: ".bin",
			theAnswer: "recons/f503/017d/7baf7fbc0fff7492b751025c6a78179b/009320587.00031.bin",
		},
		wantErr: false,
	},
	{
		on:    true,
		param: "0xf503017d7baf7fbc0fff7492b751025c6a78179b",
		expected: cacheItem{
			cacheType: Cache_Slurps,
			rootPart:  `[{CachePath}]`,
			typePart:  "slurps",
			extraPart: "",
			extension: ".bin",
			theAnswer: "slurps/0xf503017d7baf7fbc0fff7492b751025c6a78179b.bin",
		},
		wantErr: false,
	},
	{
		on:    true,
		param: "junk.fil",
		expected: cacheItem{
			cacheType: Cache_Tmp,
			rootPart:  `[{CachePath}]`,
			typePart:  "tmp",
			extraPart: "",
			extension: "",
			theAnswer: "tmp/junk.fil",
		},
		wantErr: false,
	},
	{
		on:    true,
		param: "1001001.20",
		expected: cacheItem{
			cacheType: Cache_Traces,
			rootPart:  `[{CachePath}]`,
			typePart:  "traces",
			extraPart: "00/10/01",
			extension: ".bin",
			theAnswer: "traces/00/10/01/001001001.00020.bin",
		},
		wantErr: false,
	},
	{
		on:    true,
		param: "1001001.20",
		expected: cacheItem{
			cacheType: Cache_Transactions,
			rootPart:  `[{CachePath}]`,
			typePart:  "txs",
			extraPart: "00/10/01",
			extension: ".bin",
			theAnswer: "txs/00/10/01/001001001.00020.bin",
		},
		wantErr: false,
	},
	{
		on:    true,
		param: "0010000000-0010200000",
		expected: cacheItem{
			cacheType: Index_Bloom,
			rootPart:  `[{IndexPath}]`,
			typePart:  "blooms",
			extraPart: "",
			extension: ".bloom",
			theAnswer: "blooms/0010000000-0010200000.bloom",
		},
		wantErr: false,
	},
	{
		on:    true,
		param: "0010000000-0010200000",
		expected: cacheItem{
			cacheType: Index_Final,
			rootPart:  `[{IndexPath}]`,
			typePart:  "finalized",
			extraPart: "",
			extension: ".bin",
			theAnswer: "finalized/0010000000-0010200000.bin",
		},
		wantErr: false,
	},
	{
		on:    true,
		param: "1002003",
		expected: cacheItem{
			cacheType: Index_Ripe,
			rootPart:  `[{IndexPath}]`,
			typePart:  "ripe",
			extraPart: "",
			extension: ".txt",
			theAnswer: "ripe/001002003.txt",
		},
		wantErr: false,
	},
	{
		on:    true,
		param: "16500001",
		expected: cacheItem{
			cacheType: Index_Staging,
			rootPart:  `[{IndexPath}]`,
			typePart:  "staging",
			extraPart: "",
			extension: ".txt",
			theAnswer: "staging/016500001.txt",
		},
		wantErr: false,
	},
	{
		on:    true,
		param: "2",
		expected: cacheItem{
			cacheType: Index_Unripe,
			rootPart:  `[{IndexPath}]`,
			typePart:  "unripe",
			extraPart: "",
			extension: ".txt",
			theAnswer: "unripe/000000002.txt",
		},
		wantErr: false,
	},
	{
		// TODO: BOGUS TURN THIS ON
		param: "1-2",
		expected: cacheItem{
			cacheType: Index_Maps,
			rootPart:  `[{IndexPath}]`,
			typePart:  "maps",
			extraPart: "",
			extension: ".bin",
			theAnswer: "maps/000000001-000000002.bin",
		},
		wantErr: false,
	},
}
