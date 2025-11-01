package configPkg

import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/utils"
)

func (opts *ConfigOptions) HandleEdit(rCtx *output.RenderCtx) error {
	_ = rCtx
	if opts.Globals.TestMode {
		logger.Info("Can not process this command in test mode.")
		return nil
	}

	editor := os.Getenv("EDITOR")
	configFile := config.PathToConfigFile()
	utils.System(editor + " \"" + configFile + "\"")
	return nil
}
