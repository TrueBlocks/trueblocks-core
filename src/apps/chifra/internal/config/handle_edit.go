package configPkg

import (
	"os"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ConfigOptions) HandleEdit() error {
	editor := os.Getenv("EDITOR")
	configFolder := config.GetPathToRootConfig()
	configFile := filepath.Join(configFolder + "trueBlocks.toml")
	utils.System(editor + " \"" + configFile + "\"")
	return nil
}
