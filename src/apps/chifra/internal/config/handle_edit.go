package configPkg

import (
	"os"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ConfigOptions) HandleEdit() error {
	testMode := opts.Globals.TestMode
	if testMode {
		logger.Info("would have opened config file for edit")
	} else {
		editor := os.Getenv("EDITOR")
		configFolder := config.GetPathToRootConfig()
		configFile := filepath.Join(configFolder + "trueBlocks.toml")
		utils.System(editor + " \"" + configFile + "\"")
	}

	return nil
}
