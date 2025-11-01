// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package cmd

// EXISTING_CODE
import (
	"os"

	configPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/internal/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/file"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// configCmd represents the config command
var configCmd = &cobra.Command{
	Use:     usageConfig,
	Long:    longConfig,
	Version: versionText,
	PreRun: outputHelpers.PreRunWithJsonWriter("config", func() *globals.GlobalOptions {
		return &configPkg.GetOptions().Globals
	}),
	RunE: file.RunWithFileSupport("config", configPkg.RunConfig, configPkg.ResetOptions),
	PostRun: outputHelpers.PostRunWithJsonWriter(func() *globals.GlobalOptions {
		return &configPkg.GetOptions().Globals
	}),
}

const usageConfig = `config <mode> [flags]

Arguments:
  mode - either show or edit the configuration
    One of [ show | edit ]`

const longConfig = `Purpose:
  Report on and edit the configuration of the TrueBlocks system.`

const notesConfig = ``

func init() {
	var capabilities caps.Capability // capabilities for chifra config
	capabilities = capabilities.Add(caps.Default)

	configCmd.Flags().SortFlags = false

	configCmd.Flags().BoolVarP(&configPkg.GetOptions().Paths, "paths", "a", false, `show the configuration paths for the system`)
	configCmd.Flags().BoolVarP(&configPkg.GetOptions().Dump, "dump", "d", false, `dump the configuration to stdout`)
	globals.InitGlobals("config", configCmd, &configPkg.GetOptions().Globals, capabilities)

	configCmd.SetUsageTemplate(UsageWithNotes(notesConfig))
	configCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(configCmd)
}
