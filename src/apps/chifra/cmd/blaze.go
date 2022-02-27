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

	blazePkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/blaze"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// blazeCmd represents the blaze command
var blazeCmd = &cobra.Command{
	Use:     usageBlaze,
	Short:   shortBlaze,
	Long:    longBlaze,
	Version: versionText,
	RunE:    blazePkg.RunBlaze,
}

var usageBlaze = `blaze [flags]`

var shortBlaze = "private service to process indexing individual blocks"

var longBlaze = `Purpose:
  Private service to process indexing individual blocks.`

var notesBlaze = ``

func init() {
	blazeCmd.Flags().SortFlags = false

	blazeCmd.Flags().Uint64VarP(&blazePkg.GetOptions().StartBlock, "start_block", "s", 0, "first block to visit")
	blazeCmd.Flags().Uint64VarP(&blazePkg.GetOptions().BlockCnt, "block_cnt", "n", 2000, "maximum number of blocks to process per pass")
	blazeCmd.Flags().Uint64VarP(&blazePkg.GetOptions().BlockChanCnt, "block_chan_cnt", "b", 10, "number of concurrent block processing channels")
	blazeCmd.Flags().Uint64VarP(&blazePkg.GetOptions().AddrChanCnt, "addr_chan_cnt", "d", 20, "number of concurrent address processing channels")
	blazeCmd.Flags().Uint64VarP(&blazePkg.GetOptions().RipeBlock, "ripe_block", "r", 0, "blocks prior to this value are written to 'ripe' folder")
	globals.InitGlobals(blazeCmd, &blazePkg.GetOptions().Globals)

	blazeCmd.SetUsageTemplate(UsageWithNotes(notesBlaze))
	blazeCmd.SetOut(os.Stderr)

	chifraCmd.AddCommand(blazeCmd)
}
