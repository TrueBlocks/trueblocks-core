package configPkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

func (opts *ConfigOptions) HandleShow() error {
	if opts.Globals.TestMode {
		logger.Info("Can not process this command in test mode.")
		return nil
	}

	fmt.Printf("%s", file.AsciiFileToString(config.PathToConfigFile()))
	return nil
}
