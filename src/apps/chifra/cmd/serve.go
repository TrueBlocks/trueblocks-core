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

	servePkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd/serve"
	"github.com/spf13/cobra"
)

// serveCmd represents the serve command
var serveCmd = &cobra.Command{
	Use:   usageServe,
	Short: shortServe,
	Long:  longServe,
	RunE:  servePkg.Run,
	Args:  servePkg.Validate,
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
	serveCmd.SetOut(os.Stderr)

	serveCmd.Flags().SortFlags = false
	serveCmd.PersistentFlags().SortFlags = false
	serveCmd.Flags().StringVarP(&servePkg.Options.Port, "port", "p", ":8080", "specify the server's port")
	serveCmd.Flags().SortFlags = false
	serveCmd.PersistentFlags().SortFlags = false

	serveCmd.SetUsageTemplate(UsageWithNotes(notesServe))
	rootCmd.AddCommand(serveCmd)
}
