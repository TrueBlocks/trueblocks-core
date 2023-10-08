package configPkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

func (opts *ConfigOptions) HandleShow() error {
	configFile := config.PathToConfigFile()
	fmt.Printf("%s", file.AsciiFileToString(configFile))
	return nil
}
