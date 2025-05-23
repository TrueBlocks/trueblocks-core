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
	"encoding/json"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
)

// EXISTING_CODE

type Status struct {
	CachePath     string      `json:"cachePath,omitempty"`
	Caches        []CacheItem `json:"caches"`
	Chain         string      `json:"chain,omitempty"`
	ChainConfig   string      `json:"chainConfig,omitempty"`
	ChainId       string      `json:"chainId,omitempty"`
	Chains        []Chain     `json:"chains"`
	ClientVersion string      `json:"clientVersion,omitempty"`
	HasEsKey      bool        `json:"hasEsKey,omitempty"`
	HasPinKey     bool        `json:"hasPinKey,omitempty"`
	IndexPath     string      `json:"indexPath,omitempty"`
	IsApi         bool        `json:"isApi,omitempty"`
	IsArchive     bool        `json:"isArchive,omitempty"`
	IsScraping    bool        `json:"isScraping,omitempty"`
	IsTesting     bool        `json:"isTesting,omitempty"`
	IsTracing     bool        `json:"isTracing,omitempty"`
	NetworkId     string      `json:"networkId,omitempty"`
	Progress      string      `json:"progress,omitempty"`
	RootConfig    string      `json:"rootConfig,omitempty"`
	RpcProvider   string      `json:"rpcProvider,omitempty"`
	Version       string      `json:"version,omitempty"`
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
	_ = chain
	_ = format
	_ = verbose
	_ = extraOpts
	var model = map[string]any{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]any{
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
	}
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

	testMode := extraOpts["testMode"] == true
	if len(s.Caches) > 0 {
		if testMode {
			for i := 0; i < len(s.Caches); i++ {
				s.Caches[i].Path = "--paths--"
				s.Caches[i].LastCached = "--lastCached--"
				s.Caches[i].NFiles = 123
				s.Caches[i].NFolders = 456
				s.Caches[i].SizeInBytes = 789
			}
		}
		model["caches"] = s.Caches
		order = append(order, "caches")
	}

	if extraOpts["chains"] == true {
		var chains []Chain
		if extraOpts["testMode"] == true {
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
		model["chains"] = chains
		order = append(order, "chains")
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Status) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	// EXISTING_CODE
	// EXISTING_CODE
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

// EXISTING_CODE
