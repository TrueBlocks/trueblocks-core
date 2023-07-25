// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package cmd

// EXISTING_CODE
import (
	"os"

	configPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// configCmd represents the config command
var configCmd = &cobra.Command{
	Use:     usageConfig,
	Short:   shortConfig,
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

const shortConfig = "report on and edit the configuration of the TrueBlocks system"

const longConfig = `Purpose:
  Report on and edit the configuration of the TrueBlocks system.`

const notesConfig = ``

func init() {
	var capabilities = caps.Default // Additional global caps for chifra config
	// EXISTING_CODE
	// EXISTING_CODE

	configCmd.Flags().SortFlags = false

	configCmd.Flags().BoolVarP(&configPkg.GetOptions().Paths, "paths", "a", false, "show the configuration paths for the system")
	globals.InitGlobals(configCmd, &configPkg.GetOptions().Globals, capabilities)

	configCmd.SetUsageTemplate(UsageWithNotes(notesConfig))
	configCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(configCmd)
}
