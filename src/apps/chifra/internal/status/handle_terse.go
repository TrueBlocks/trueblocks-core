package statusPkg

import (
	"context"
	"fmt"
	"os"
	"strings"
	"text/template"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
)

func (opts *StatusOptions) HandleStatusTerse() error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode

	meta, err := rpcClient.GetMetaData(chain, false)
	if err != nil {
		msg := fmt.Sprintf("%sCould not load RPC provider: %s%s", colors.Red, err, colors.Off)
		logger.InfoTable("Progress:", msg)
		logger.Fatal("")
		return err
	}
	vers, err := rpcClient.GetVersion(chain)
	if err != nil {
		return err
	}

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {
		s := simpleStatus{
			ClientVersion: vers,
			Version:       version.LibraryVersion,
			RPCProvider:   config.GetRpcProvider(chain),
			RootConfig:    config.GetPathToRootConfig(),
			ChainConfig:   config.GetPathToChainConfig(chain),
			CachePath:     config.GetPathToCache(chain),
			IndexPath:     config.GetPathToIndex(chain),
			Progress:      metaToProgress(chain, meta),
			IsTesting:     testMode,
			IsArchive:     rpcClient.IsArchiveNode(testMode, chain),
			IsTracing:     rpcClient.IsTracingNode(testMode, chain),
			HasEsKey:      config.HasEsKeys(chain),
			HasPinKey:     config.HasPinningKeys(chain),
			Chain:         chain,
			NetworkId:     fmt.Sprint(meta.NetworkId),
			ChainId:       fmt.Sprint(meta.ChainId),
		}

		if opts.toLogger(&s) {
			return
		}

		modelChan <- &s
	}

	extra := map[string]any{
		"testMode": testMode,
	}
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}

type simpleStatus struct {
	ClientVersion string `json:"clientVersion"`
	Version       string `json:"trueblocksVersion"`
	RPCProvider   string `json:"rpcProvider"`
	RootConfig    string `json:"rootConfig"`
	ChainConfig   string `json:"chainConfig"`
	CachePath     string `json:"cachePath"`
	IndexPath     string `json:"indexPath"`
	Progress      string `json:"progress"`
	IsTesting     bool   `json:"isTesting"`
	IsArchive     bool   `json:"isArchive"`
	IsTracing     bool   `json:"isTracing"`
	HasEsKey      bool   `json:"hasEsKey"`
	HasPinKey     bool   `json:"hasPinKey"`
	Chain         string `json:"chain"`
	ChainId       string `json:"chainId"`
	NetworkId     string `json:"networkId"`
}

func (s *simpleStatus) Raw() *types.RawModeler {
	return nil
}

func (s *simpleStatus) Model(showHidden bool, format string, extraOptions map[string]any) types.Model {
	return types.Model{
		Data: map[string]interface{}{
			"clientVersion":     s.ClientVersion,
			"trueblocksVersion": s.Version,
			"rpcProvider":       s.RPCProvider,
			"rootConfig":        s.RootConfig,
			"chainConfig":       s.ChainConfig,
			"cachePath":         s.CachePath,
			"indexPath":         s.IndexPath,
			"progress":          s.Progress,
			"isTesting":         s.IsTesting,
			"isArchive":         s.IsArchive,
			"isTracing":         s.IsTracing,
			"hasEsKey":          s.HasEsKey,
			"hasPinKey":         s.HasPinKey,
		},
		Order: []string{
			"clientVersion",
			"trueblocksVersion",
			"rpcProvider",
			"rootConfig",
			"chainConfig",
			"cachePath",
			"indexPath",
			"progress",
			"isTesting",
			"isArchive",
			"isTracing",
			"hasEsKey",
			"hasPinKey",
		},
	}
}

func metaToProgress(chain string, meta *rpcClient.MetaData) string {
	nTs, _ := tslib.NTimestamps(chain)
	format := "%d, %d, %d,  %d, ts: %d"
	if !utils.IsTerminal() {
		format = "[%d %d %d %d %d]"
	}
	return fmt.Sprintf(format, meta.Latest, meta.Finalized, meta.Staging, meta.Unripe, nTs)
}

func (opts *StatusOptions) toLogger(s *simpleStatus) bool {
	testMode := opts.Globals.TestMode
	timeDatePart := "INFO[DATE|TIME]"
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
	} else {
		now := time.Now()
		timeDatePart = now.Format("02-01|15:04:05.000")
	}

	if (!utils.IsTerminal() && !testMode) || opts.Globals.Format == "json" {
		return false
	}

	table := `INFO Client:            {CLIENT_STRING}
INFO TrueBlocks:        {VERSION_STRING}
INFO RPC Provider:      {{.RPCProvider}} - {{.Chain}} ({{.NetworkId}}/{{.ChainId}})
INFO Root Config Path:  {{.RootConfig}}
INFO Chain Config Path: {{.ChainConfig}}
INFO Cache Path:        {{.CachePath}}
INFO Index Path:        {{.IndexPath}}
INFO Progress:          {{.Progress}}
`
	table = strings.Replace(table, "INFO ", "INFO "+colors.Green, -1)
	table = strings.Replace(table, ":", ":"+colors.Off, -1)
	table = strings.Replace(table, "{CLIENT_STRING}", getClientTemplate(), -1)
	table = strings.Replace(table, "{VERSION_STRING}", getVersionTemplate(), -1)
	table = strings.Replace(table, "INFO", timeDatePart, -1)

	t, err := template.New("status").Parse(table)
	if err != nil {
		return false
	}
	t.Execute(os.Stderr, s)
	return true

	// logger.InfoTable("Client:           ", s.ClientVersion)
	// logger.InfoTable("TrueBlocks:       ", s.Version)
	// logger.InfoTable("RPC Provider:     ", s.RPCProvider)
	// logger.InfoTable("Root Config Path: ", s.RootConfig)
	// logger.InfoTable("Chain Config Path:", s.ChainConfig)
	// logger.InfoTable("Cache Path:       ", s.CachePath)
	// logger.InfoTable("Index Path:       ", s.IndexPath)
	// logger.InfoTable("Progress:         ", s.Progress)
	// logger.InfoTable("IsTesting:        ", s.IsTesting)
	// logger.InfoTable("IsArchive:        ", s.IsArchive)
	// logger.InfoTable("IsTracing:        ", s.IsTracing)
	// logger.InfoTable("HasEsKey:         ", s.HasEsKey)
	// logger.InfoTable("HasPinningKeys:   ", s.HasPinKey)
	// return true
}

func getVersionTemplate() string {
	ret := `{{.Version}} (
{{if .HasEsKey}}eskey, {{else}}no eskey, {{end}}
{{if .HasPinKey}}pinKey{{else}}no pinKey{{end}}
)`
	return strings.Replace(ret, "\n", "", -1)
}

func getClientTemplate() string {
	ret := `{{.ClientVersion}} (
{{if .IsArchive}}archive, {{else}}not archive, {{end}}
{{if .IsTesting}}testing, {{end}}
{{if .IsTracing}}tracing{{else}}not tracing{{end}}
)`
	return strings.Replace(ret, "\n", "", -1)
}
