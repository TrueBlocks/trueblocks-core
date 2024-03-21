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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	namesPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// namesCmd represents the names command
var namesCmd = &cobra.Command{
	Use:     usageNames,
	Short:   shortNames,
	Long:    longNames,
	Version: versionText,
	PreRun: outputHelpers.PreRunWithJsonWriter("names", func() *globals.GlobalOptions {
		return &namesPkg.GetOptions().Globals
	}),
	RunE: file.RunWithFileSupport("names", namesPkg.RunNames, namesPkg.ResetOptions),
	PostRun: outputHelpers.PostRunWithJsonWriter(func() *globals.GlobalOptions {
		return &namesPkg.GetOptions().Globals
	}),
}

const usageNames = `names [flags] <term> [term...]

Arguments:
  terms - a space separated list of one or more search terms (required)`

const shortNames = "query addresses or names of well-known accounts"

const longNames = `Purpose:
  Query addresses or names of well-known accounts.`

const notesNames = `
Notes:
  - The tool will accept up to three terms, each of which must match against any field in the database.
  - The --match_case option enables case sensitive matching.`

func init() {
	var capabilities caps.Capability // capabilities for chifra names
	capabilities = capabilities.Add(caps.Default)

	namesCmd.Flags().SortFlags = false

	namesCmd.Flags().BoolVarP(&namesPkg.GetOptions().Expand, "expand", "e", false, "expand search to include all fields (search name, address, and symbol otherwise)")
	namesCmd.Flags().BoolVarP(&namesPkg.GetOptions().MatchCase, "match_case", "m", false, "do case-sensitive search")
	namesCmd.Flags().BoolVarP(&namesPkg.GetOptions().All, "all", "a", false, "include all (including custom) names in the search")
	namesCmd.Flags().BoolVarP(&namesPkg.GetOptions().Custom, "custom", "c", false, "include only custom named accounts in the search")
	namesCmd.Flags().BoolVarP(&namesPkg.GetOptions().Prefund, "prefund", "p", false, "include prefund accounts in the search")
	namesCmd.Flags().BoolVarP(&namesPkg.GetOptions().Addr, "addr", "s", false, "display only addresses in the results (useful for scripting, assumes --no_header)")
	namesCmd.Flags().BoolVarP(&namesPkg.GetOptions().Tags, "tags", "g", false, "export the list of tags and subtags only")
	namesCmd.Flags().BoolVarP(&namesPkg.GetOptions().Clean, "clean", "C", false, "clean the data (addrs to lower case, sort by addr)")
	namesCmd.Flags().BoolVarP(&namesPkg.GetOptions().Regular, "regular", "r", false, "only available with --clean, cleans regular names database")
	namesCmd.Flags().BoolVarP(&namesPkg.GetOptions().DryRun, "dry_run", "d", false, "only available with --clean or --autoname, outputs changes to stdout instead of updating databases")
	namesCmd.Flags().StringVarP(&namesPkg.GetOptions().Autoname, "autoname", "A", "", "an address assumed to be a token, added automatically to names database if true")
	namesCmd.Flags().BoolVarP(&namesPkg.GetOptions().Create, "create", "", false, "create a new name record (hidden)")
	namesCmd.Flags().BoolVarP(&namesPkg.GetOptions().Update, "update", "", false, "edit an existing name (hidden)")
	namesCmd.Flags().BoolVarP(&namesPkg.GetOptions().Delete, "delete", "", false, "delete a name, but do not remove it (hidden)")
	namesCmd.Flags().BoolVarP(&namesPkg.GetOptions().Undelete, "undelete", "", false, "undelete a previously deleted name (hidden)")
	namesCmd.Flags().BoolVarP(&namesPkg.GetOptions().Remove, "remove", "", false, "remove a previously deleted name (hidden)")
	if os.Getenv("TEST_MODE") != "true" {
		_ = namesCmd.Flags().MarkHidden("create")
		_ = namesCmd.Flags().MarkHidden("update")
		_ = namesCmd.Flags().MarkHidden("delete")
		_ = namesCmd.Flags().MarkHidden("undelete")
		_ = namesCmd.Flags().MarkHidden("remove")
	}
	globals.InitGlobals("names", namesCmd, &namesPkg.GetOptions().Globals, capabilities)

	namesCmd.SetUsageTemplate(UsageWithNotes(notesNames))
	namesCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(namesCmd)
}
