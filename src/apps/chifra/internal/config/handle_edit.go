package configPkg

import (
	"context"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ConfigOptions) HandleEdit() error {
	_ = context.TODO()
	if opts.Globals.TestMode {
		logger.Info("Can not process this command in test mode.")
		return nil
	}

	editor := os.Getenv("EDITOR")
	configFile := config.PathToConfigFile()
	utils.System(editor + " \"" + configFile + "\"")
	return nil
}
