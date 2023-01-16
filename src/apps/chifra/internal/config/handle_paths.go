package configPkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/bykof/gostradamus"
)

// HandlePaths handles the paths command for the command line. Returns error only as per cobra.
func (opts *ConfigOptions) HandlePaths() (error, bool) {
    // TODO: This needs to be a SimpleType and use StreamMany
    timeStr := gostradamus.Now().Format("2006/01/02 15:04:05")
    fmt.Printf("\nchifra status --paths:\n")
    fmt.Println("  ", timeStr, colors.Green+"Config Path: "+colors.Off, config.GetPathToRootConfig())
    fmt.Println("  ", timeStr, colors.Green+"Cache Path:  "+colors.Off, config.GetPathToCache(opts.Globals.Chain))
    fmt.Println("  ", timeStr, colors.Green+"Index Path:  "+colors.Off, config.GetPathToIndex(opts.Globals.Chain))
    fmt.Println()
    return nil, false
}
