package cmd

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/spf13/cobra"
)

func validateGlobalFlags(cmd *cobra.Command, args []string) error {
	if len(RootOpts.file) > 0 && !utils.FileExists(RootOpts.file) {
		return validate.Usage("file {0} not found", RootOpts.file)
	}

	err := validate.ValidateEnum("--fmt", RootOpts.fmt, "[json|txt|csv|api]")
	if err != nil {
		return err
	}
	return nil
}
