// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

// EXISTING_CODE
import (
	"context"
	"encoding/json"
	"sort"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

// EXISTING_CODE

type Status struct {
	CachePath     string       `json:"cachePath,omitempty"`
	Caches        []CacheItem  `json:"caches"`
	Chain         string       `json:"chain,omitempty"`
	ChainConfig   string       `json:"chainConfig,omitempty"`
	ChainId       string       `json:"chainId,omitempty"`
	Chains        []Chain      `json:"chains"`
	ClientVersion string       `json:"clientVersion,omitempty"`
	HasEsKey      bool         `json:"hasEsKey,omitempty"`
	HasPinKey     bool         `json:"hasPinKey,omitempty"`
	IndexPath     string       `json:"indexPath,omitempty"`
	IsApi         bool         `json:"isApi,omitempty"`
	IsArchive     bool         `json:"isArchive,omitempty"`
	IsScraping    bool         `json:"isScraping,omitempty"`
	IsTesting     bool         `json:"isTesting,omitempty"`
	IsTracing     bool         `json:"isTracing,omitempty"`
	NetworkId     string       `json:"networkId,omitempty"`
	Progress      string       `json:"progress,omitempty"`
	RootConfig    string       `json:"rootConfig,omitempty"`
	RpcProvider   string       `json:"rpcProvider,omitempty"`
	Version       string       `json:"version,omitempty"`
	Calcs         *StatusCalcs `json:"calcs,omitempty"`
	// EXISTING_CODE
	Meta  *MetaData `json:"meta,omitempty"`
	Diffs *MetaData `json:"diffs,omitempty"`
	// EXISTING_CODE
}

func (s Status) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Status) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	props := NewModelProps(chain, format, verbose, extraOpts)

	rawNames := []Labeler{}
	model := s.RawMap(props, &rawNames)
	for k, v := range s.CalcMap(props) {
		model[k] = v
	}

	var order = []string{}
	// EXISTING_CODE
	order = []string{
		"cachePath",
		"chainConfig",
		"clientVersion",
		"hasEsKey",
		"hasPinKey",
		"indexPath",
		"isApi",
		"isArchive",
		"isTesting",
		"isTracing",
		"rootConfig",
		"rpcProvider",
		"trueblocksVersion",
	}

	if extraOpts["caches"] == true || len(s.Caches) > 0 {
		order = append(order, "caches")
	}

	if extraOpts["chains"] == true {
		order = append(order, "chains")
	}
	// EXISTING_CODE

	for _, item := range rawNames {
		key := item.name + "Name"
		if _, exists := model[key]; exists {
			order = append(order, key)
		}
	}
	order = reorderFields(order)

	return Model{
		Data:  model,
		Order: order,
	}
}

// RawMap returns a map containing only the raw/base fields for this Status.
func (s *Status) RawMap(p *ModelProps, needed *[]Labeler) map[string]any {
	model := map[string]any{
		// EXISTING_CODE
		"cachePath":         s.CachePath,
		"chainConfig":       s.ChainConfig,
		"clientVersion":     s.ClientVersion,
		"hasEsKey":          s.HasEsKey,
		"hasPinKey":         s.HasPinKey,
		"indexPath":         s.IndexPath,
		"isApi":             s.IsApi,
		"isArchive":         s.IsArchive,
		"isTesting":         s.IsTesting,
		"isTracing":         s.IsTracing,
		"rootConfig":        s.RootConfig,
		"rpcProvider":       s.RpcProvider,
		"trueblocksVersion": s.Version,
		// EXISTING_CODE
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return labelAddresses(p, model, needed)
}

// CalcMap returns a map containing the calculated/derived fields for this type.
func (s *Status) CalcMap(p *ModelProps) map[string]any {
	_ = p // delint
	model := map[string]any{
		// EXISTING_CODE
		// EXISTING_CODE
	}

	// EXISTING_CODE
	testMode := p.ExtraOpts["testMode"] == true

	var caches []CacheItem
	if p.ExtraOpts["caches"] == true {
		chain, _ := p.ExtraOpts["chain"].(string)
		var modeTypes []walk.CacheType
		if mt, ok := p.ExtraOpts["modeTypes"].([]walk.CacheType); ok {
			modeTypes = mt
		}
		caches = s.GetCaches(chain, testMode, modeTypes)
	} else if len(s.Caches) > 0 {
		caches = s.Caches
	}

	if len(caches) > 0 {
		if testMode {
			for i := 0; i < len(caches); i++ {
				caches[i].Path = "--paths--"
				caches[i].LastCached = "--lastCached--"
				caches[i].NFiles = 123
				caches[i].NFolders = 456
				caches[i].SizeInBytes = 789
			}
		}
		model["caches"] = caches
	}

	if p.ExtraOpts["chains"] == true {
		chains := s.GetChains(testMode)
		model["chains"] = chains
	}
	// EXISTING_CODE

	return model
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Status) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	s.Calcs = nil
	// EXISTING_CODE
	// EXISTING_CODE
}

// StatusCalcs holds lazy-loaded calculated fields for Status
type StatusCalcs struct {
	// EXISTING_CODE
	Caches []CacheItem `json:"caches,omitempty"`
	Chains []Chain     `json:"chains,omitempty"`
	// EXISTING_CODE
}

func (s *Status) EnsureCalcs(p *ModelProps, fieldFilter []string) error {
	_ = fieldFilter // delint
	if s.Calcs != nil {
		return nil
	}

	calcMap := s.CalcMap(p)
	if len(calcMap) == 0 {
		return nil
	}

	jsonBytes, err := json.Marshal(calcMap)
	if err != nil {
		return err
	}

	s.Calcs = &StatusCalcs{}
	return json.Unmarshal(jsonBytes, s.Calcs)
}

// EXISTING_CODE
func (s *Status) ShallowCopy() Status {
	caches := s.Caches
	s.Caches = nil
	ret := *s
	s.Caches = caches
	ret.Chain = s.Chain
	return ret
}

// GetChains returns a slice of Chain objects for the status
func (s *Status) GetChains(testMode bool) []Chain {
	var chains []Chain
	if testMode {
		ch := Chain{
			Chain:         "testChain",
			ChainId:       12345,
			LocalExplorer: "http://localhost:8080",
			RpcProvider:   "http://localhost:8545",
			Symbol:        "ETH",
		}
		chains = append(chains, ch)
	} else {
		chainArray := config.GetChains()
		for _, chain := range chainArray {
			ch := Chain{
				Chain:          chain.Chain,
				ChainId:        base.MustParseUint64(chain.ChainId),
				LocalExplorer:  chain.LocalExplorer,
				RemoteExplorer: chain.RemoteExplorer,
				RpcProvider:    chain.GetRpcProvider(),
				IpfsGateway:    chain.IpfsGateway,
				Symbol:         chain.Symbol,
			}
			chains = append(chains, ch)
		}
		sort.Slice(chains, func(i, j int) bool {
			if chains[i].ChainId == chains[j].ChainId {
				return chains[i].Chain < chains[j].Chain
			}
			return chains[i].ChainId < chains[j].ChainId
		})
	}
	return chains
}

// GetCaches returns a slice of CacheItem objects for the status
func (s *Status) GetCaches(chain string, testMode bool, modeTypes []walk.CacheType) []CacheItem {
	if testMode {
		mockCaches := []CacheItem{
			{
				CacheItemType: "abisCache",
				Path:          "/path/to/abis",
				NFiles:        100,
				NFolders:      1,
				SizeInBytes:   10000,
				LastCached:    "2006-01-02 15:04:05",
				Items:         make([]any, 0),
			},
			{
				CacheItemType: "monitorsCache",
				Path:          "/path/to/monitors",
				NFiles:        50,
				NFolders:      5,
				SizeInBytes:   5000,
				LastCached:    "2006-01-02 15:04:05",
				Items:         make([]any, 0),
			},
			{
				CacheItemType: "namesCache",
				Path:          "/path/to/names",
				NFiles:        200,
				NFolders:      2,
				SizeInBytes:   20000,
				LastCached:    "2006-01-02 15:04:05",
				Items:         make([]any, 0),
			},
		}
		return mockCaches
	}

	// Define all cache types to scan
	var cacheTypesToScan []walk.CacheType

	// If specific mode types are provided, use those; otherwise use all cache types
	if len(modeTypes) > 0 {
		cacheTypesToScan = modeTypes
	} else {
		cacheTypesToScan = []walk.CacheType{
			walk.Cache_Abis,
			walk.Cache_Monitors,
			walk.Cache_Names,
			walk.Cache_Tmp,
			walk.Cache_Blocks,
			walk.Cache_Logs,
			walk.Cache_Receipts,
			walk.Cache_Results,
			walk.Cache_Slurps,
			walk.Cache_State,
			walk.Cache_Statements,
			walk.Cache_Tokens,
			walk.Cache_Traces,
			walk.Cache_Transactions,
			walk.Cache_Withdrawals,
			walk.Index_Bloom,
			walk.Index_Final,
			walk.Index_Ripe,
			walk.Index_Staging,
			walk.Index_Unripe,
			walk.Index_Maps,
		}
	}

	now := time.Now()
	filenameChan := make(chan walk.CacheFileInfo)
	var nRoutines int

	// Create a map to hold cache items as we process files
	counterMap := make(map[walk.CacheType]*CacheItem)
	nRoutines = len(cacheTypesToScan)

	// Define CacheWalker type inside the function
	type CacheWalker struct {
		ctx    context.Context
		cancel context.CancelFunc
		nSeen  uint64
	}

	// Initialize a cache item for each cache type
	for _, mT := range cacheTypesToScan {
		counterMap[mT] = &CacheItem{
			CacheItemType: walk.WalkCacheName(mT),
			Items:         make([]any, 0),
			LastCached:    now.Format("2006-01-02 15:04:05"),
		}

		// Setup a walker for each cache type
		var t CacheWalker
		t.ctx, t.cancel = context.WithCancel(context.Background())
		go walk.WalkCacheFolder(t.ctx, chain, mT, &t, filenameChan)
	}

	// Process each file/folder found
	for result := range filenameChan {
		cT := result.Type

		switch cT {
		case walk.Cache_NotACache:
			nRoutines--
			if nRoutines == 0 {
				close(filenameChan)
			}

		default:
			isIndex := func(cT walk.CacheType) bool {
				m := map[walk.CacheType]bool{
					walk.Index_Bloom:   true,
					walk.Index_Final:   true,
					walk.Index_Ripe:    true,
					walk.Index_Staging: true,
					walk.Index_Unripe:  true,
					walk.Index_Maps:    true,
				}
				return m[cT]
			}

			// Skip certain cache types in test mode
			if testMode && isIndex(cT) && (cT != walk.Index_Bloom && cT != walk.Index_Final) {
				continue
			} else if testMode && cT == walk.Cache_Results {
				continue
			}

			if walk.IsCacheType(result.Path, cT, !result.IsDir /* checkExt */) {
				if result.IsDir {
					counterMap[cT].NFolders++
					counterMap[cT].Path = walk.GetRootPathFromCacheType(chain, cT)
				} else {
					result.Data.(*CacheWalker).nSeen++
					counterMap[cT].NFiles++
					counterMap[cT].SizeInBytes += file.FileSize(result.Path)
				}
			}
		}
	}

	// Collect all cache items into a slice
	var caches []CacheItem
	for _, cT := range cacheTypesToScan {
		if counter, ok := counterMap[cT]; ok {
			caches = append(caches, *counter)
		}
	}

	return caches
}

// EXISTING_CODE
