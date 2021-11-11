/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
/*
 * The file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */
package cmd

import (
	"os"

	pinsPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd/pins"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/spf13/cobra"
)

// pinsCmd represents the pins command
var pinsCmd = &cobra.Command{
	Use:   usagePins,
	Short: shortPins,
	Long:  longPins,
	Run:   pinsPkg.Run,
	Args:  pinsPkg.Validate,
}

var usagePins = `pins [flags]`

var shortPins = "manage pinned index of appearances and associated blooms"

var longPins = `Purpose:
  Manage pinned index of appearances and associated blooms.`

var notesPins = `
Notes:
  - One of --list or --init is required.
  - Re-run chifra init as often as you wish. It will repair or freshen the index.
  - The --share option works only if an IPFS daemon is running.`

func init() {
	pinsCmd.SetOut(os.Stderr)

	pinsCmd.Flags().SortFlags = false
	pinsCmd.PersistentFlags().SortFlags = false
	pinsCmd.Flags().BoolVarP(&pinsPkg.Options.List, "list", "l", false, "list the bloom and index hashes from local cache or IPFS")
	pinsCmd.Flags().BoolVarP(&pinsPkg.Options.Init, "init", "i", false, "download the blooms or index chunks from IPFS")
	pinsCmd.Flags().BoolVarP(&pinsPkg.Options.All, "all", "a", false, "in addition to Bloom filters, download full index chunks")
	pinsCmd.Flags().BoolVarP(&pinsPkg.Options.Share, "share", "S", false, "share downloaded data by pinning it to IPFS (the IPFS daemon must be running)")
	pinsCmd.Flags().Float64VarP(&pinsPkg.Options.Sleep, "sleep", "s", .25, "throttle requests by this many seconds")
	pinsCmd.Flags().BoolVarP(&pinsPkg.Options.Freshen, "freshen", "f", false, "check for new bloom or index chunks and download if available (hidden)")
	pinsCmd.Flags().BoolVarP(&pinsPkg.Options.Remote, "remote", "r", false, "for --list mode only, recover the manifest from IPFS via UnchainedIndex smart contract (hidden)")
	pinsCmd.Flags().BoolVarP(&pinsPkg.Options.Init_All, "init_all", "n", false, "use --init --all instead (hidden)")
	if !utils.IsTestMode() {
		pinsCmd.Flags().MarkHidden("freshen")
		pinsCmd.Flags().MarkHidden("remote")
		pinsCmd.Flags().MarkHidden("init_all")
	}
	pinsCmd.Flags().SortFlags = false
	pinsCmd.PersistentFlags().SortFlags = false

	pinsCmd.SetUsageTemplate(UsageWithNotes(notesPins))
	rootCmd.AddCommand(pinsCmd)
}
