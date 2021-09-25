package cmd

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

import (
	"os"

	"github.com/spf13/cobra"
)

type serveOptionsType struct {
}

var ServeOpts serveOptionsType

// serveCmd represents the serve command
var serveCmd = &cobra.Command{
	Use:   `serve`,
	Short: "serve the TrueBlocks API using the flame server",
	Long: `Purpose:
  Serve the TrueBlocks API using the flame server.`,
	Run: runServe,
}

func init() {
	serveCmd.Flags().SortFlags = false
	serveCmd.PersistentFlags().SortFlags = false
	serveCmd.SetOut(os.Stderr)
	rootCmd.AddCommand(serveCmd)
}

func runServe(cmd *cobra.Command, args []string) {
	options := ""
	for _, arg := range args {
		options += " " + arg
	}
    arguments := ""
	PassItOn("/Users/jrush/.local/bin/chifra/flame", options, arguments)
}
