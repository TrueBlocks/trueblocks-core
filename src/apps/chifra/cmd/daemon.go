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

	daemonPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/daemon"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/caps"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// daemonCmd represents the daemon command
var daemonCmd = &cobra.Command{
	Use:     usageDaemon,
	Short:   shortDaemon,
	Long:    longDaemon,
	Version: versionText,
	PreRun: outputHelpers.PreRunWithJsonWriter("daemon", func() *globals.GlobalOptions {
		return &daemonPkg.GetOptions().Globals
	}),
	RunE: file.RunWithFileSupport("daemon", daemonPkg.RunDaemon, daemonPkg.ResetOptions),
	PostRun: outputHelpers.PostRunWithJsonWriter(func() *globals.GlobalOptions {
		return &daemonPkg.GetOptions().Globals
	}),
	Aliases: []string{
		"serve",
	},
}

const usageDaemon = `daemon [flags]`

const shortDaemon = "initalize and control long-running processes such as the API and the scrapers"

const longDaemon = `Purpose:
  Initalize and control long-running processes such as the API and the scrapers.`

const notesDaemon = `
Notes:
  - To start API open terminal window and run chifra daemon.
  - See the API documentation (https://trueblocks.io/api) for more information.`

func init() {
	var capabilities = caps.Default // Additional global caps for chifra daemon
	// EXISTING_CODE
	capabilities = capabilities.Remove(caps.Chain)
	capabilities = capabilities.Remove(caps.NoHeader)
	capabilities = capabilities.Remove(caps.Output)
	capabilities = capabilities.Remove(caps.Append)
	// capabilities = capabilities.Remove(caps.Fmt)
	// capabilities = capabilities.Remove(caps.File)
	// EXISTING_CODE

	daemonCmd.Flags().SortFlags = false

	daemonCmd.Flags().StringVarP(&daemonPkg.GetOptions().Port, "port", "p", ":8080", "specify the server's port")
	daemonCmd.Flags().StringVarP(&daemonPkg.GetOptions().Api, "api", "a", "on", `instruct the node to start the API server
One of [ off | on ]`)
	daemonCmd.Flags().StringVarP(&daemonPkg.GetOptions().Scrape, "scrape", "s", "", `start the scraper, initialize it with either just blooms or entire index, generate for new blocks
One of [ off | blooms | index ]`)
	daemonCmd.Flags().BoolVarP(&daemonPkg.GetOptions().Monitor, "monitor", "m", false, "instruct the node to start the monitors tool")
	daemonCmd.Flags().BoolVarP(&daemonPkg.GetOptions().Grpc, "grpc", "g", false, "run gRPC server to serve names")
	globals.InitGlobals(daemonCmd, &daemonPkg.GetOptions().Globals, capabilities)

	daemonCmd.SetUsageTemplate(UsageWithNotes(notesDaemon))
	daemonCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(daemonCmd)
}
