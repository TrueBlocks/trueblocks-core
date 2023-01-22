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
	RunE:    file.RunWithFileSupport("config", configPkg.RunConfig, configPkg.ResetOptions),
	PostRun: outputHelpers.PostRunWithJsonWriter(func() *globals.GlobalOptions {
		return &configPkg.GetOptions().Globals
	}),
	Aliases: []string{
		"status",
	},
}

const usageConfig = `config <mode> [mode...] [flags]

Arguments:
  modes - either show or edit the configuration
	One or more of [ show | edit ]`

const shortConfig = "report on and edit the configuration of the TrueBlocks system"

const longConfig = `Purpose:
  Report on and edit the configuration of the TrueBlocks system.`

const notesConfig = `
Notes:
  - The 'status' alias is deprecated and will be removed shortly.`

func init() {
	configCmd.Flags().SortFlags = false

	configCmd.Flags().StringSliceVarP(&configPkg.GetOptions().Module, "module", "", nil, `the type of information to show or edit
One or more of [ index | monitors | names | abis | caches | some | all ]`)
	configCmd.Flags().BoolVarP(&configPkg.GetOptions().Details, "details", "d", false, "include details about items found in monitors, slurps, abis, or price caches")
	configCmd.Flags().StringSliceVarP(&configPkg.GetOptions().Types, "types", "t", nil, `for caches module only, which type(s) of cache to report
One or more of [ blocks | txs | traces | slurps | all ]`)
	configCmd.Flags().Uint64VarP(&configPkg.GetOptions().Depth, "depth", "p", 0, "for caches module only, number of levels deep to report (hidden)")
	configCmd.Flags().BoolVarP(&configPkg.GetOptions().Terse, "terse", "e", false, "show a terse summary report for mode show (hidden)")
	configCmd.Flags().BoolVarP(&configPkg.GetOptions().Paths, "paths", "a", false, "show the configuration paths for the system (hidden)")
	configCmd.Flags().Uint64VarP(&configPkg.GetOptions().FirstBlock, "first_block", "F", 0, "first block to process (inclusive -- testing only) (hidden)")
	configCmd.Flags().Uint64VarP(&configPkg.GetOptions().LastBlock, "last_block", "L", 0, "last block to process (inclusive -- testing only) (hidden)")
	if os.Getenv("TEST_MODE") != "true" {
		configCmd.Flags().MarkHidden("depth")
		configCmd.Flags().MarkHidden("terse")
		configCmd.Flags().MarkHidden("paths")
		configCmd.Flags().MarkHidden("first_block")
		configCmd.Flags().MarkHidden("last_block")
	}
	globals.InitGlobals(configCmd, &configPkg.GetOptions().Globals)

	configCmd.SetUsageTemplate(UsageWithNotes(notesConfig))
	configCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(configCmd)
}
