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
	servePkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/server"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// serveCmd represents the serve command
var serveCmd = &cobra.Command{
	Use:     usageServe,
	Short:   shortServe,
	Long:    longServe,
	Version: "GHC-TrueBlocks//0.17.0-alpha",
	RunE:    servePkg.RunServe,
}

var usageServe = `serve [flags]`

var shortServe = "serve the TrueBlocks API using the flame server"

var longServe = `Purpose:
  Serve the TrueBlocks API using the flame server.`

var notesServe = `
Notes:
  - To start API open terminal window and run chifra serve.
  - See the API documentation for more information.`

func init() {
	serveCmd.Flags().SortFlags = false

	serveCmd.Flags().StringVarP(&servePkg.Options.Port, "port", "p", ":8080", "specify the server's port")
	globals.InitGlobals(serveCmd, &servePkg.Options.Globals)

	serveCmd.SetUsageTemplate(UsageWithNotes(notesServe))
	serveCmd.SetOut(os.Stderr)

	chifraCmd.AddCommand(serveCmd)
}
