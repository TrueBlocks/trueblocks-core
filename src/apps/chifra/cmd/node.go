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
	nodePkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/node"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// nodeCmd represents the node command
var nodeCmd = &cobra.Command{
	Use:     usageNode,
	Short:   shortNode,
	Long:    longNode,
	Version: versionText,
	PreRun: outputHelpers.PreRunWithJsonWriter("node", func() *globals.GlobalOptions {
		return &nodePkg.GetOptions().Globals
	}),
	RunE:    file.RunWithFileSupport("node", nodePkg.RunNode, nodePkg.ResetOptions),
	PostRun: outputHelpers.PostRunWithJsonWriter(func() *globals.GlobalOptions {
		return &nodePkg.GetOptions().Globals
	}),
}

const usageNode = `node [flags]`

const shortNode = "initalize and control long-running tools such as the API, scrapers, and monitors"

const longNode = `Purpose:
  Initalize and control long-running tools such as the API, scrapers, and monitors.`

const notesNode = ``

func init() {
	nodeCmd.Flags().SortFlags = false

	nodeCmd.Flags().StringVarP(&nodePkg.GetOptions().Scrape, "scrape", "s", "", `start the scraper, initialize it with either just blooms or entire index, generate for new blocks
One of [ off | blooms | full-index ]`)
	nodeCmd.Flags().BoolVarP(&nodePkg.GetOptions().Monitor, "monitor", "m", false, "instruct the node to start the monitors tool")
	nodeCmd.Flags().StringVarP(&nodePkg.GetOptions().Api, "api", "a", "on", `instruct the node to start the API server
One of [ off | on ]`)
	globals.InitGlobals(nodeCmd, &nodePkg.GetOptions().Globals)

	nodeCmd.SetUsageTemplate(UsageWithNotes(notesNode))
	nodeCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(nodeCmd)
}
