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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	slurpPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/slurp"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// slurpCmd represents the slurp command
var slurpCmd = &cobra.Command{
	Use:     usageSlurp,
	Short:   shortSlurp,
	Long:    longSlurp,
	Version: versionText,
	PreRun: outputHelpers.PreRunWithJsonWriter("slurp", func() *globals.GlobalOptions {
		return &slurpPkg.GetOptions().Globals
	}),
	RunE:    file.RunWithFileSupport("slurp", slurpPkg.RunSlurp, slurpPkg.ResetOptions),
	PostRun: outputHelpers.PostRunWithJsonWriter(func() *globals.GlobalOptions {
		return &slurpPkg.GetOptions().Globals
	}),
}

const usageSlurp = `slurp [flags] <address> [address...] [block...]

Arguments:
  addrs - one or more addresses to slurp from Etherscan (required)
  blocks - an optional range of blocks to slurp`

const shortSlurp = "fetch data from EtherScan for any address"

const longSlurp = `Purpose:
  Fetch data from EtherScan for any address.`

const notesSlurp = `
Notes:
  - Portions of this software are Powered by Etherscan.io APIs.`

func init() {
	slurpCmd.Flags().SortFlags = false

	slurpCmd.Flags().StringSliceVarP(&slurpPkg.GetOptions().Types, "types", "t", nil, `which types of transactions to request
One or more of [ ext | int | token | nfts | miner | uncles | all ]`)
	slurpCmd.Flags().BoolVarP(&slurpPkg.GetOptions().Appearances, "appearances", "p", false, "show only the blocknumber.tx_id appearances of the exported transactions")
	globals.InitGlobals(slurpCmd, &slurpPkg.GetOptions().Globals)

	slurpCmd.SetUsageTemplate(UsageWithNotes(notesSlurp))
	slurpCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(slurpCmd)
}
