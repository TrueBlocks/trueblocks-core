package configPkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
)

func (opts *ConfigOptions) HandleShow(rCtx output.RenderCtx) error {
	if opts.Mode == "edit" {
		return opts.HandleEdit(rCtx)
	}

	if opts.Globals.TestMode {
		logger.Info("Can not process this command in test mode.")
		return nil
	}

	fmt.Printf("%s", file.AsciiFileToString(config.PathToConfigFile()))
	return nil
}
