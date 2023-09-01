// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package statusPkg

// EXISTING_CODE
import (
	"fmt"
	"io"
	"sort"
	"strconv"
	"strings"
	"text/template"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
)

// EXISTING_CODE

type simpleStatus struct {
	CachePath     string            `json:"cachePath,omitempty"`
	Caches        []simpleCacheItem `json:"caches,omitempty"`
	Chain         string            `json:"chain,omitempty"`
	ChainConfig   string            `json:"chainConfig,omitempty"`
	ChainId       string            `json:"chainId,omitempty"`
	ClientVersion string            `json:"clientVersion,omitempty"`
	HasEsKey      bool              `json:"hasEsKey,omitempty"`
	HasPinKey     bool              `json:"hasPinKey,omitempty"`
	IndexPath     string            `json:"indexPath,omitempty"`
	IsApi         bool              `json:"isApi,omitempty"`
	IsArchive     bool              `json:"isArchive,omitempty"`
	IsTesting     bool              `json:"isTesting,omitempty"`
	IsTracing     bool              `json:"isTracing,omitempty"`
	NetworkId     string            `json:"networkId,omitempty"`
	Progress      string            `json:"progress,omitempty"`
	RootConfig    string            `json:"rootConfig,omitempty"`
	RPCProvider   string            `json:"rpcProvider,omitempty"`
	Version       string            `json:"trueblocksVersion,omitempty"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *simpleStatus) Raw() *types.RawModeler {
	return nil
}

func (s *simpleStatus) Model(verbose bool, format string, extraOptions map[string]any) types.Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]interface{}{
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
		"rpcProvider":       s.RPCProvider,
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

	if extraOptions != nil && extraOptions["showProgress"] == true {
		model["progress"] = s.Progress
		order = append(order, "progress")
	}

	testMode := extraOptions["testMode"] == true
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

	if extraOptions["chains"] == true {
		var chains []types.SimpleChain
		if extraOptions["testMode"] == true {
			ch := types.SimpleChain{
				Chain:         "testChain",
				ChainId:       12345,
				LocalExplorer: "http://localhost:8080",
				RpcProvider:   "http://localhost:8545",
				Symbol:        "ETH",
			}
			chains = append(chains, ch)
		} else {
			chainArray := config.GetChainArray()
			for _, chain := range chainArray {
				ch := types.SimpleChain{
					Chain:          chain.Chain,
					ChainId:        mustParseUint(chain.ChainId),
					LocalExplorer:  chain.LocalExplorer,
					RemoteExplorer: chain.RemoteExplorer,
					RpcProvider:    chain.RpcProvider,
					ApiProvider:    chain.ApiProvider,
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

	return types.Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
//

func ToProgress(chain string, meta *rpc.MetaData) string {
	nTs, _ := tslib.NTimestamps(chain)
	format := "%d, %d, %d, %d ts: %d"
	return fmt.Sprintf(format, meta.Latest, meta.Finalized, meta.Staging, meta.Unripe, nTs)
}

func (opts *StatusOptions) GetSimpleStatus() (*simpleStatus, error) {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode

	meta, err := opts.Conn.GetMetaData(false)
	if err != nil {
		return nil, err
	}

	vers, err := opts.Conn.GetClientVersion()
	if err != nil {
		return nil, err
	}

	provider, _ := config.GetRpcProvider(chain)
	s := &simpleStatus{
		ClientVersion: vers,
		Version:       version.LibraryVersion,
		RPCProvider:   provider,
		RootConfig:    config.GetPathToRootConfig(),
		ChainConfig:   config.GetPathToChainConfig(chain),
		CachePath:     config.GetPathToCache(chain),
		IndexPath:     config.GetPathToIndex(chain),
		Progress:      ToProgress(chain, meta),
		IsTesting:     testMode,
		IsApi:         opts.Globals.IsApiMode(),
		IsArchive:     opts.Conn.IsNodeArchive(),
		IsTracing:     opts.Conn.IsNodeTracing(),
		HasEsKey:      config.HasEsKeys(chain),
		HasPinKey:     config.HasPinningKeys(chain),
		Chain:         chain,
		NetworkId:     fmt.Sprint(meta.NetworkId),
		ChainId:       fmt.Sprint(meta.ChainId),
	}

	if testMode {
		s.ClientVersion = "Client version"
		s.Version = "GHC-TrueBlocks//vers-beta--git-hash---git-ts-"
		s.RPCProvider = "--providers--"
		s.RootConfig = "--paths--"
		s.ChainConfig = "--paths--"
		s.CachePath = "--paths--"
		s.IndexPath = "--paths--"
		s.Progress = "--client--, --final--, --staging--, --unripe-- ts: --ts--"
		s.HasPinKey = false // the test machine doesn't have a key
	}

	return s, nil
}

func (s *simpleStatus) toTemplate(w io.Writer, testMode bool, format string) bool {
	if format == "json" {
		return false
	}

	var timeDatePart string
	if testMode {
		timeDatePart = "INFO[DATE|TIME]"
	} else {
		now := time.Now()
		timeDatePart = now.Format("02-01|15:04:05.000")
	}

	table := strings.Replace(templateStr, "INFO ", "INFO "+colors.Green, -1)
	table = strings.Replace(table, ":", ":"+colors.Off, -1)
	table = strings.Replace(table, "{CLIENT_STRING}", getClientTemplate(), -1)
	table = strings.Replace(table, "{VERSION_STRING}", getVersionTemplate(), -1)
	table = strings.Replace(table, "INFO", timeDatePart, -1)
	table = strings.Replace(table, "[RED]", colors.Red, -1)
	table = strings.Replace(table, "[GREEN]", colors.Green, -1)
	table = strings.Replace(table, "[OFF]", colors.Off, -1)

	t, err := template.New("status").Parse(table)
	if err != nil {
		logger.Fatal("Should not happen. Bad template.", err)
		return false
	}

	_ = t.Execute(w, s)
	return true
}

func getBoolTemplate(name string) (ret string) {
	if strings.HasPrefix(name, "Has") {
		ret = "{{if .[NAME]}}[GREEN][NAMEL][OFF]{{else}}[RED]no [NAMEL][OFF]{{end}}"
		ret = strings.Replace(strings.Replace(ret, "[NAMEL]", strings.ToLower(name), -1), "has", "", -1)
	} else {
		ret = "{{if .[NAME]}}[GREEN][NAMEL][OFF]{{else}}[RED]not [NAMEL][OFF]{{end}}"
		ret = strings.Replace(strings.Replace(ret, "[NAMEL]", strings.ToLower(name), -1), "is", "", -1)
	}
	ret = strings.Replace(ret, "[NAME]", name, -1)
	return
}

func getClientTemplate() string {
	archive := getBoolTemplate("IsArchive")
	tracing := getBoolTemplate("IsTracing")
	return "{{.ClientVersion}} (" + archive + ", {{if .IsTesting}}[GREEN]testing[OFF], {{end}}" + tracing + ")"
}

func getVersionTemplate() string {
	esKey := getBoolTemplate("HasEsKey")
	pinKey := getBoolTemplate("HasPinKey")
	return "{{.Version}} (" + esKey + ", " + pinKey + ")"
}

const templateStr = `INFO Client:            {CLIENT_STRING}
INFO TrueBlocks:        {VERSION_STRING}
INFO RPC Provider:      {{.RPCProvider}} - {{.Chain}} ({{if eq .NetworkId "0"}}[RED]{{.NetworkId}}[OFF]{{else}}[GREEN]{{.NetworkId}}[OFF]{{end}}/{{if eq .ChainId "0"}}[RED]{{.ChainId}}[OFF]{{else}}[GREEN]{{.ChainId}}[OFF]{{end}})
INFO Root Config Path:  {{.RootConfig}}
INFO Chain Config Path: {{.ChainConfig}}
INFO Cache Path:        {{.CachePath}}
INFO Index Path:        {{.IndexPath}}
INFO Progress:          {{.Progress}}
`

func mustParseUint(input any) (result uint64) {
	result, _ = strconv.ParseUint(fmt.Sprint(input), 0, 64)
	return
}

// EXISTING_CODE
