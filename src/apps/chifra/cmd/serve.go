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
 * Parts of this file were generated with makeClass --gocmds. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */
package cmd

import (
	"os"

	"github.com/spf13/cobra"
)

// serveCmd represents the serve command
var serveCmd = &cobra.Command{
	Use:   usageServe,
	Short: shortServe,
	Long:  longServe,
	Run:   runServe,
	Args:  ValidatePositionals(validateServeArgs),
}

var usageServe = `serve [flags]`

var shortServe = "serve the TrueBlocks API using the flame server"

var longServe = `Purpose:
  Serve the TrueBlocks API using the flame server.`

var notesServe = ``

type serveOptionsType struct {
}

var ServeOpts serveOptionsType

func init() {
	serveCmd.SetOut(os.Stderr)

	serveCmd.Flags().SortFlags = false
	serveCmd.PersistentFlags().SortFlags = false
	serveCmd.Flags().SortFlags = false
	serveCmd.PersistentFlags().SortFlags = false

	serveCmd.SetUsageTemplate(HelpWithNotes(notesServe))
	rootCmd.AddCommand(serveCmd)
}
