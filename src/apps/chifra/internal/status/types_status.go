// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package statusPkg

// EXISTING_CODE
import (
	"fmt"
	"io"
	"sort"
	"strings"
	"text/template"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
)

// EXISTING_CODE

type simpleStatus struct {
	CachePath     string              `json:"cachePath,omitempty"`
	Caches        []simpleCacheItem   `json:"caches"`
	Chain         string              `json:"chain,omitempty"`
	ChainConfig   string              `json:"chainConfig,omitempty"`
	ChainId       string              `json:"chainId,omitempty"`
	Chains        []types.SimpleChain `json:"chains"`
	ClientVersion string              `json:"clientVersion,omitempty"`
	HasEsKey      bool                `json:"hasEsKey,omitempty"`
	HasPinKey     bool                `json:"hasPinKey,omitempty"`
	IndexPath     string              `json:"indexPath,omitempty"`
	IsApi         bool                `json:"isApi,omitempty"`
	IsArchive     bool                `json:"isArchive,omitempty"`
	IsScraping    bool                `json:"isScraping,omitempty"`
	IsTesting     bool                `json:"isTesting,omitempty"`
	IsTracing     bool                `json:"isTracing,omitempty"`
	NetworkId     string              `json:"networkId,omitempty"`
	Progress      string              `json:"progress,omitempty"`
	RootConfig    string              `json:"rootConfig,omitempty"`
	RpcProvider   string              `json:"rpcProvider,omitempty"`
	Version       string              `json:"version,omitempty"`

	// EXISTING_CODE
	Meta  *rpc.MetaData `json:"meta,omitempty"`
	Diffs *rpc.MetaData `json:"diffs,omitempty"`
	// EXISTING_CODE
}

func (s *simpleStatus) Raw() *types.RawModeler {
	return nil
}

func (s *simpleStatus) Model(chain, format string, verbose bool, extraOptions map[string]any) types.Model {
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
			chainArray := config.GetChains()
			for _, chain := range chainArray {
				ch := types.SimpleChain{
					Chain:          chain.Chain,
					ChainId:        utils.MustParseUint(chain.ChainId),
					LocalExplorer:  chain.LocalExplorer,
					RemoteExplorer: chain.RemoteExplorer,
					RpcProvider:    chain.RpcProvider,
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

func ToProgress(chain string, diagnose bool, meta *rpc.MetaData) string {
	nTs, _ := tslib.NTimestamps(chain) // when the file has one record, the block is zero, etc.
	if nTs > 0 {
		nTs--
	}
	format := "%d, %d, %d, %d ts: %d"
	return fmt.Sprintf(format, meta.Latest, meta.Finalized, meta.Staging, meta.Unripe, nTs)
}

func (opts *StatusOptions) GetSimpleStatus(diagnose bool) (*simpleStatus, error) {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode

	meta, err := opts.Conn.GetMetaData(false)
	if err != nil {
		return nil, err
	}
	diffs := &rpc.MetaData{
		Latest:    0,
		Finalized: meta.Latest - meta.Finalized,
		Staging:   meta.Latest - meta.Staging,
		Unripe:    meta.Latest - meta.Unripe,
		Ripe:      meta.Latest - meta.Ripe,
	}

	vers, err := opts.Conn.GetClientVersion()
	if err != nil {
		return nil, err
	}

	provider := config.GetChain(chain).RpcProvider
	s := &simpleStatus{
		ClientVersion: vers,
		Version:       version.LibraryVersion,
		RpcProvider:   provider,
		RootConfig:    config.PathToRootConfig(),
		ChainConfig:   config.MustGetPathToChainConfig(chain),
		CachePath:     config.PathToCache(chain),
		IndexPath:     config.PathToIndex(chain),
		Progress:      ToProgress(chain, diagnose, meta),
		IsTesting:     testMode,
		IsApi:         opts.Globals.IsApiMode(),
		IsArchive:     opts.Conn.IsNodeArchive(),
		IsTracing:     opts.Conn.IsNodeTracing(),
		HasEsKey:      validate.HasArticulationKey(true),
		HasPinKey:     len(config.GetKey("pinata").ApiKey) > 0 || len(config.GetKey("pinata").Secret) > 0,
		Chain:         chain,
		NetworkId:     fmt.Sprint(meta.NetworkId),
		ChainId:       fmt.Sprint(meta.ChainId),
		Meta:          meta,
		Diffs:         diffs,
		// Finalized: meta.Latest - meta.Finalized,
		// Staging:   meta.Latest - meta.Staging,
		// Unripe:    meta.Latest - meta.Unripe,
		// Ripe:      meta.Latest - meta.Ripe,
	}

	if testMode {
		s.ClientVersion = "Client version"
		s.Version = "GHC-TrueBlocks//vers-beta--git-hash---git-ts-"
		s.RpcProvider = "--providers--"
		s.RootConfig = "--paths--"
		s.ChainConfig = "--paths--"
		s.CachePath = "--paths--"
		s.IndexPath = "--paths--"
		s.Progress = "--client--, --final--, --staging--, --unripe-- ts: --ts--"
		s.HasPinKey = false // the test machine doesn't have a key
	}

	return s, nil
}

func (s *simpleStatus) toTemplate(w io.Writer, testMode, diagnose, logTimerOn bool, format string) bool {
	if format == "json" {
		return false
	}

	var timeDatePart string
	if logTimerOn {
		now := time.Now()
		timeDatePart = now.Format("02-01|15:04:05.000 ")
	} else {
		timeDatePart = "INFO[DATE|TIME] "
	}

	table := templateStr
	table = strings.Replace(table, "[CLIENT]", getClientTemplate(), -1)
	table = strings.Replace(table, "[VERSION]", getVersionTemplate(), -1)
	table = strings.Replace(table, "[IDS]", getIdTemplate(), -1)
	table = strings.Replace(table, "[PROGRESS]", s.getProgress(testMode, diagnose), -1)
	table = strings.Replace(table, "INFO ", timeDatePart+colors.Green, -1)
	table = strings.Replace(table, "[RED]", colors.Red, -1)
	table = strings.Replace(table, "[GREEN]", colors.Green, -1)
	table = strings.Replace(table, "[OFF]", colors.Off, -1)
	table = strings.Replace(table, ":", ":"+colors.Off, -1)

	t, err := template.New("status").Parse(table)
	if err != nil {
		logger.Fatal("should not happen ==> bad template.", err)
		return false
	}

	_ = t.Execute(w, s)
	return true
}

func getClientTemplate() string {
	archive := "{{if .IsArchive}}[GREEN]archive[OFF]{{else}}[RED]no archive[OFF]{{end}}"
	testing := "{{if .IsTesting}}[GREEN]testing[OFF], {{end}}"
	tracing := "{{if .IsTracing}}[GREEN]tracing[OFF]{{else}}[RED]no tracing[OFF]{{end}}"
	return "{{.ClientVersion}} (" + archive + ", " + testing + tracing + ")"
}

func getVersionTemplate() string {
	esKey := "{{if .HasEsKey}}[GREEN]eskey[OFF]{{else}}[RED]no eskey[OFF]{{end}}"
	pinKey := "{{if .HasPinKey}}[GREEN]pinkey[OFF]{{else}}[RED]no pinkey[OFF]{{end}}"
	return "{{.Version}} (" + esKey + ", " + pinKey + ")"
}

func getIdTemplate() string {
	networkId := "{{if eq .NetworkId \"0\"}}[RED]{{.NetworkId}}[OFF]{{else}}[GREEN]{{.NetworkId}}[OFF]{{end}}"
	chainId := "{{if eq .ChainId \"0\"}}[RED]{{.ChainId}}[OFF]{{else}}[GREEN]{{.ChainId}}[OFF]{{end}}"
	return networkId + "/" + chainId
}

func (s *simpleStatus) getProgress(testMode, diagnose bool) string {
	if diagnose {
		if testMode {
			return "--diagnostics--"
		}
		nTs, _ := tslib.NTimestamps(s.Meta.Chain) // when the file has one record, the block is zero, etc.
		if nTs > 0 {
			nTs--
		}
		nTsDiff := s.Meta.Latest - nTs
		if nTs > s.Meta.Latest {
			nTsDiff = 0
		}
		ret := `
INFO [OFF]  Chain Head [GREEN]{{.Meta.Latest}}[OFF]
INFO [OFF]  Finalized  [GREEN]{{.Meta.Finalized}}[OFF] ([GREEN]{{.Diffs.Finalized}}[OFF] behind head)
INFO [OFF]  Stage      [GREEN]{{.Meta.Staging}}[OFF] ([GREEN]{{.Diffs.Staging}}[OFF] behind head)
INFO [OFF]  Indexing   [GREEN]{{.Meta.Unripe}}[OFF] ([GREEN]{{.Diffs.Unripe}}[OFF] behind head)
INFO [OFF]  Timestamps [GREEN]{nTs}[OFF] ([GREEN]{nTsDiff}[OFF] behind head)
`
		ret = strings.Replace(ret, "{nTs}", fmt.Sprint(nTs), -1)
		ret = strings.Replace(ret, "{nTsDiff}", fmt.Sprint(nTsDiff), -1)
		return ret
	}

	return "          {{.Progress}}"
}

const templateStr = `INFO Client:            [CLIENT]
INFO TrueBlocks:        [VERSION]
INFO RPC Provider:      {{.RpcProvider}} - {{.Chain}} ([IDS])
INFO Root Config Path:  {{.RootConfig}}
INFO Chain Config Path: {{.ChainConfig}}
INFO Cache Path:        {{.CachePath}}
INFO Index Path:        {{.IndexPath}}
INFO Progress:[PROGRESS]
`

/*
TODO: Better diagnostics (see #3209)
TODO: When Node synced, successfull chifra init but scraper not running
TODO:
TODO: WARN: Index is behind the chain head, but scraper is not running.
TODO: WARN: This means chifra is not aware of latest transactions and can return incomplete data.
TODO: WARN: Run `chifra daemon --scrape index` to solve the issue.
TODO:
TODO: Node is not syncing
TODO:
TODO: WARN: Your node seems to be syncing. During this state it can refuse queries from chifra
TODO: WARN: or return incomplete data. If you observe such issues, please wait until your node
TODO: WARN: is fully synced and your index catches up to the chain head.
*/

// EXISTING_CODE
