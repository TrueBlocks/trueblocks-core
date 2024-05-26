package monitorsPkg

import (
	"context"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/usage"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *MonitorsOptions) HandleDecache() error {
	for _, addr := range opts.Addrs {
		if !opts.Globals.IsApiMode() && !usage.QueryUser(getWarning(addr), "Not decaching") {
			return nil
		}
		os.Setenv("TB_NO_USERQUERY", "true")
		_ = utils.System("chifra export --decache " + addr)
	}

	message := "Decaching complete"
	logger.Info(message)
	if opts.Globals.IsApiMode() {
		fetchData := func(modelChan chan types.Modeler[types.Message], errorChan chan error) {
			modelChan <- &types.Message{
				Msg: message,
			}
		}
		_ = output.StreamMany(context.Background(), fetchData, opts.Globals.OutputOpts())
	}

	return nil
}

func getWarning(addr string) string {
	return strings.Replace("Are sure you want to decache {0}{1} (Yn)?", "{0}", addr, -1)
}
