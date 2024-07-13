package configPkg

import (
	"context"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

func (opts *ConfigOptions) HandleShow() error {
	_ = context.TODO()
	if opts.Mode == "edit" {
		return opts.HandleEdit()
	}

	if opts.Globals.TestMode {
		logger.Info("Can not process this command in test mode.")
		return nil
	}

	fmt.Printf("%s", file.AsciiFileToString(config.PathToConfigFile()))
	return nil
}
