package statusPkg

import (
	"fmt"
	"io"
	"strings"
	"text/template"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
)

type simpleStatus struct {
	CachePath     string `json:"cachePath,omitempty"`
	Chain         string `json:"chain,omitempty"`
	ChainConfig   string `json:"chainConfig,omitempty"`
	ChainId       string `json:"chainId,omitempty"`
	ClientVersion string `json:"clientVersion,omitempty"`
	HasEsKey      bool   `json:"hasEsKey,omitempty"`
	HasPinKey     bool   `json:"hasPinKey,omitempty"`
	// TODO: BOGUS - add this it used to be in the old code
	// Host          string `json:"host,omitempty"`
	IndexPath string `json:"indexPath,omitempty"`
	// TODO: BOGUS - add this it used to be in the old code
	// IsApi         bool   `json:"isApi,omitempty"`
	IsArchive   bool   `json:"isArchive,omitempty"`
	IsTesting   bool   `json:"isTesting,omitempty"`
	IsTracing   bool   `json:"isTracing,omitempty"`
	NetworkId   string `json:"networkId,omitempty"`
	Progress    string `json:"progress,omitempty"`
	RootConfig  string `json:"rootConfig,omitempty"`
	RPCProvider string `json:"rpcProvider,omitempty"`
	Version     string `json:"trueblocksVersion,omitempty"`
}

func (s *simpleStatus) Raw() *types.RawModeler {
	return nil
}

func (s *simpleStatus) Model(showHidden bool, format string, extraOptions map[string]any) types.Model {
	// isApi := extraOptions != nil && extraOptions["isApi"] == true
	model := map[string]interface{}{
		"cachePath":     s.CachePath,
		"chainConfig":   s.ChainConfig,
		"clientVersion": s.ClientVersion,
		"hasEsKey":      s.HasEsKey,
		"hasPinKey":     s.HasPinKey,
		// "host":              s.Host,
		"indexPath": s.IndexPath,
		// "isApi":             isApi,
		"isArchive":         s.IsArchive,
		"isTesting":         s.IsTesting,
		"isTracing":         s.IsTracing,
		"rootConfig":        s.RootConfig,
		"rpcProvider":       s.RPCProvider,
		"trueblocksVersion": s.Version,
	}

	order := []string{
		"cachePath",
		"chainConfig",
		"clientVersion",
		"hasEsKey",
		"hasPinKey",
		// "host",
		"indexPath",
		// "isApi",
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

	return types.Model{
		Data:  model,
		Order: order,
	}
}

func ToProgress(chain string, meta *rpcClient.MetaData) string {
	nTs, _ := tslib.NTimestamps(chain)
	format := "%d, %d, %d, %d ts: %d"
	return fmt.Sprintf(format, meta.Latest, meta.Finalized, meta.Staging, meta.Unripe, nTs)
}

func (opts *StatusOptions) GetSimpleStatus() (*simpleStatus, error) {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	meta, err := rpcClient.GetMetaData(chain, false)
	if err != nil {
		return nil, err
	}
	vers, err := rpcClient.GetVersion(chain)
	if err != nil {
		return nil, err
	}

	s := &simpleStatus{
		ClientVersion: vers,
		Version:       version.LibraryVersion,
		RPCProvider:   config.GetRpcProvider(chain),
		RootConfig:    config.GetPathToRootConfig(),
		ChainConfig:   config.GetPathToChainConfig(chain),
		CachePath:     config.GetPathToCache(chain),
		IndexPath:     config.GetPathToIndex(chain),
		Progress:      ToProgress(chain, meta),
		IsTesting:     testMode,
		IsArchive:     rpcClient.IsArchiveNode(testMode, chain),
		IsTracing:     rpcClient.IsTracingNode(testMode, chain),
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

func (s *simpleStatus) ToTemplate(w io.Writer, testMode bool, format string) bool {
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

	t, err := template.New("status").Parse(table)
	if err != nil {
		return false
	}

	t.Execute(w, s)
	return true
}

func getVersionTemplate() string {
	return `{{.Version}} ({{if .HasEsKey}}eskey, {{else}}no eskey, {{end}}{{if .HasPinKey}}pinKey{{else}}no pinKey{{end}})`
}

func getClientTemplate() string {
	return `{{.ClientVersion}} ({{if .IsArchive}}archive, {{else}}not archive, {{end}}{{if .IsTesting}}testing, {{end}}{{if .IsTracing}}tracing{{else}}not tracing{{end}})`
}

const templateStr = `INFO Client:            {CLIENT_STRING}
INFO TrueBlocks:        {VERSION_STRING}
INFO RPC Provider:      {{.RPCProvider}} - {{.Chain}} ({{.NetworkId}}/{{.ChainId}})
INFO Root Config Path:  {{.RootConfig}}
INFO Chain Config Path: {{.ChainConfig}}
INFO Cache Path:        {{.CachePath}}
INFO Index Path:        {{.IndexPath}}
INFO Progress:          {{.Progress}}
`
