package configPkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

func (opts *ConfigOptions) HandleShow() error {
	fn := config.PathToRootConfig() + "trueBlocks.toml"
	fmt.Printf("%s", file.AsciiFileToString(fn))
	return nil
}
